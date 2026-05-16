/*
 * Copyright 2019-2024 New Vector Ltd.
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Element-Commercial
 * Please see LICENSE files in the repository root for full details.
 */

package im.vector.app.core.extensions

import android.content.Context
import androidx.core.content.ContextCompat
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.ProcessLifecycleOwner
import im.vector.app.core.services.VectorSyncAndroidService
import chat.progressive.app.native.ProgressiveNative
import chat.progressive.app.native.ProgressiveNative
import im.vector.app.features.session.VectorSessionStore
import org.matrix.android.sdk.api.session.Session
import org.matrix.android.sdk.api.session.crypto.keysbackup.KeysBackupState
import org.matrix.android.sdk.api.session.crypto.model.MXEventDecryptionResult
import org.matrix.android.sdk.api.session.events.model.Event
import org.matrix.android.sdk.internal.crypto.EventDecryptor
import org.matrix.android.sdk.internal.crypto.RustCryptoService
import timber.log.Timber

fun Session.startSyncing(context: Context) {
    val applicationContext = context.applicationContext

    // Progressive Chat: native C++ Megolm decryptor (Labs-gated via nativeDecryptAttempt)
    EventDecryptor.nativeDecryptAttempt = { event ->
        try {
            ProgressiveNative.ensureLoaded()
            val content = event.content
            if (content == null) return@nativeDecryptAttempt null
            val ciphertext = content["ciphertext"] as? String ?: return@nativeDecryptAttempt null
            val senderKey = content["sender_key"] as? String ?: return@nativeDecryptAttempt null
            val sessionId = content["session_id"] as? String ?: return@nativeDecryptAttempt null
            val roomId = event.roomId ?: return@nativeDecryptAttempt null

            val plaintext = ProgressiveNative.nativeMegolmDecrypt(roomId, senderKey, sessionId, ciphertext)
            if (plaintext.isNullOrEmpty()) return@nativeDecryptAttempt null

            // Build MXEventDecryptionResult from plaintext
            val result = MXEventDecryptionResult()
            result.clearEvent = org.matrix.android.sdk.api.util.JsonDict().apply {
                putAll(org.matrix.android.sdk.api.util.fromJsonString(plaintext) ?: emptyMap())
            }
            result.senderCurve25519Key = senderKey
            result.claimedEd25519Key = content["device_id"] as? String
            Timber.d("PROGRESSIVE native Megolm decrypt: success for $roomId/$sessionId")
            result
        } catch (e: Exception) {
            Timber.w(e, "PROGRESSIVE native decrypt failed, falling back to Rust SDK")
            null
        }
    }

    // Progressive Chat: import room keys into native C++ MegolmSessionManager
    // m.room_key and m.forwarded_room_key to_device events trigger this
    RustCryptoService.nativeKeyImport = { algorithm, roomId, sessionId, sessionKey ->
        try {
            ProgressiveNative.ensureLoaded()
            val ok = ProgressiveNative.nativeMegolmAddSession(roomId, "", sessionId, sessionKey)
            if (ok) Timber.d("PROGRESSIVE native key imported: room=$roomId session=$sessionId")
        } catch (e: Exception) {
            Timber.w(e, "PROGRESSIVE native key import failed")
        }
    }

    if (!syncService().hasAlreadySynced()) {
        // initial sync is done as a service so it can continue below app lifecycle
        VectorSyncAndroidService.newOneShotIntent(
                context = applicationContext,
                sessionId = sessionId
        )
                .let {
                    try {
                        ContextCompat.startForegroundService(applicationContext, it)
                    } catch (ex: Throwable) {
                        // TODO
                        Timber.e(ex)
                    }
                }
    } else {
        val isAtLeastStarted = ProcessLifecycleOwner.get().lifecycle.currentState.isAtLeast(Lifecycle.State.STARTED)
        Timber.v("--> is at least started? $isAtLeastStarted")
        syncService().startSync(isAtLeastStarted)
    }
}

/**
 * Tell is the session has unsaved e2e keys in the backup.
 */
suspend fun Session.hasUnsavedKeys(): Boolean {
    return cryptoService().inboundGroupSessionsCount(false) > 0 &&
            cryptoService().keysBackupService().getState() != KeysBackupState.ReadyToBackUp
}

suspend fun Session.cannotLogoutSafely(): Boolean {
    // has some encrypted chat
    return hasUnsavedKeys() ||
            // has local cross signing keys
            (cryptoService().crossSigningService().allPrivateKeysKnown() &&
                    // That are not backed up
                    !sharedSecretStorageService().isRecoverySetup())
}

fun Session.vectorStore(context: Context) = VectorSessionStore(context, myUserId)
