package im.vector.app.features.jumptodate

import org.json.JSONObject
import timber.log.Timber

/**
 * JNI bridge to the progressive_native C++ library.
 * Handles date validation, MSC3030 URL construction, and response parsing.
 */
object ProgressiveNative {

    private var isLoaded = false

    fun ensureLoaded() {
        if (!isLoaded) {
            try {
                System.loadLibrary("progressive_native")
                isLoaded = true
                Timber.d("progressive_native loaded successfully")
            } catch (e: UnsatisfiedLinkError) {
                Timber.e(e, "Failed to load progressive_native")
                // Fallback: use pure Kotlin implementation
            }
        }
    }

    /**
     * Validates the date string (YYYY-MM-DD) and builds the MSC3030 URL.
     * Gates on the feature flag from C++ side.
     *
     * @param roomId Target room ID
     * @param dateString Date in YYYY-MM-DD format
     * @param serverUrl Base URL of the homeserver
     * @param accessToken User's access token
     * @param isEnabled Whether jumptodate labs setting is enabled
     *
     * @return JSON string with url, accessToken, timestamp — or {"error":"..."} on failure
     */
    @JvmStatic
    external fun nativeValidateAndBuild(
        roomId: String,
        dateString: String,
        serverUrl: String,
        accessToken: String,
        isEnabled: Boolean
    ): String

    /**
     * Parses the UTC timestamp_to_event response from the homeserver.
     *
     * @param responseBody HTTP response body
     * @param httpStatus HTTP status code
     *
     * @return JSON string with eventId — or error info on failure
     */
    @JvmStatic
    external fun nativeParseResponse(
        responseBody: String,
        httpStatus: Int
    ): String

    /**
     * Parses a Matrix event JSON to find the source event it relates to
     * (reply, reaction, edit, thread root).
     *
     * @param eventJson Full JSON of the Matrix event
     * @param allowedTypes Comma-separated list of allowed relation types, or empty for all
     *
     * @return JSON with sourceEventId and relationType, or {"isRelation": false}
     */
    @JvmStatic
    external fun nativeParseRelation(
        eventJson: String,
        allowedTypes: String
    ): String

    // --- Export functions ---

    @JvmStatic
    external fun nativeFormatEventHtml(
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        fileName: String,
        mediaSize: String,
        relationType: String,
        isContinuation: Boolean
    ): String

    @JvmStatic
    external fun nativeFormatEventPlainText(
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        fileName: String,
        relationType: String
    ): String

    @JvmStatic
    external fun nativeBuildHtmlExport(
        roomName: String,
        roomTopic: String,
        exportDate: String,
        eventHtmls: Array<String>
    ): String

    // --- Event Cache ---

    @JvmStatic
    external fun nativeCachePut(
        eventId: String,
        senderId: String,
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        eventType: String,
        relationType: String,
        sourceEventId: String,
        sentByMe: Boolean
    )

    @JvmStatic
    external fun nativeCacheGetContext(eventId: String): String

    @JvmStatic
    external fun nativeCacheClear()

    @JvmStatic
    external fun nativeCacheSize(): Int

    // --- SQLite Event Database ---

    @JvmStatic
    external fun nativeDbOpen(dbPath: String): Boolean

    @JvmStatic
    external fun nativeDbClose()

    @JvmStatic
    external fun nativeDbInsertEvent(
        eventId: String, roomId: String, senderId: String, senderName: String,
        timestamp: String, body: String, msgType: String, eventType: String,
        relationType: String, sourceEventId: String,
        originServerTs: Long, displayIndex: Int, sentByMe: Boolean
    )

    @JvmStatic
    external fun nativeDbGetContext(eventId: String): String

    @JvmStatic
    external fun nativeDbClearRoom(roomId: String)

    @JvmStatic
    external fun nativeDbCount(): Int

    // --- Translation ---

    @JvmStatic
    external fun nativeBuildTranslateRequest(
        text: String,
        sourceLang: String,
        targetLang: String,
        apiEndpoint: String,
        apiToken: String,
        model: String
    ): String

    @JvmStatic
    external fun nativeParseTranslateResponse(
        responseBody: String,
        httpStatus: Int
    ): String

    // --- Proxy / Tor / I2P ---

    @JvmStatic
    external fun nativeComputeProxyConfig(
        connType: Int,
        proxyType: Int,
        host: String,
        port: Int,
        username: String,
        password: String
    ): String

    // --- Yggdrasil ---

    @JvmStatic external fun nativeIsYggdrasilAddress(addr: String): Boolean
    @JvmStatic external fun nativeIsYggdrasilDomain(host: String): Boolean
    @JvmStatic external fun nativeBuildYggHomeserverUrl(addr: String, port: Int, tls: Boolean): String
    @JvmStatic external fun nativeRewriteHomeserverUrl(originalUrl: String, yggAddr: String): String

    // --- Markdown ---

    @JvmStatic external fun nativeMarkdownToHtml(markdown: String, enableTables: Boolean): String
    @JvmStatic external fun nativeParseMarkdownTable(tableBlock: String, withScroll: Boolean): String

    // --- Account Export ---

    @JvmStatic external fun nativeEncryptAccount(
        userId: String, token: String, refreshToken: String,
        homeServer: String, deviceId: String, deviceName: String,
        displayName: String, avatarUrl: String,
        includeCache: Boolean, passphrase: String
    ): String

    @JvmStatic external fun nativeDecryptAccount(encrypted: String, passphrase: String): String

    // --- Audio ---

    @JvmStatic external fun nativeFormatDuration(ms: Long): String
    @JvmStatic external fun nativeFormatPositionInfo(posMs: Long, durMs: Long): String
    @JvmStatic external fun nativeComputeProgress(posMs: Long, durMs: Long): Float
    @JvmStatic external fun nativeIsSupportedAudio(mimeType: String): Boolean

    // --- Media Filter ---

    @JvmStatic external fun nativeGetFileExtension(fileName: String, mimeType: String): String
    @JvmStatic external fun nativeIsValidMxcUri(uri: String): Boolean

    // --- Content Filter ---

    @JvmStatic external fun nativeKeywordFilterLoad(raw: String)
    @JvmStatic external fun nativeKeywordFilterCheck(text: String): String
    @JvmStatic external fun nativeKeywordFilterExport(): String
    @JvmStatic external fun nativeKeywordFilterCount(): Int
    @JvmStatic external fun nativeKeywordFilterClear()

    @JvmStatic external fun nativeShouldBlockImage(
        blockAll: Boolean, allowAvatars: Boolean, allowStickers: Boolean, allowEmoji: Boolean,
        mxcUrl: String, imageType: String
    ): Boolean

    // --- Network Stats ---

    @JvmStatic external fun nativeNetStatsStart(url: String, method: String): Int
    @JvmStatic external fun nativeNetStatsEnd(requestId: Int, statusCode: Int, bytesSent: Long, bytesReceived: Long, error: String)
    @JvmStatic external fun nativeNetStatsToJson(): String
    @JvmStatic external fun nativeNetStatsToText(): String
    @JvmStatic external fun nativeNetStatsClear()

    // --- Masquerade ---

    @JvmStatic external fun nativeIsValidMasqueradeName(name: String): Boolean
    @JvmStatic external fun nativeGetSuggestedMasqueradeNames(): String
    @JvmStatic external fun nativeIsValidIconAlias(alias: String): Boolean
    @JvmStatic external fun nativeBuildMasqueradeAlias(baseAlias: String, iconName: String): String

    // --- User Mask ---

    @JvmStatic external fun nativeUserMaskSet(mxid: String, displayName: String, avatarUrl: String, overrideMxid: String)
    @JvmStatic external fun nativeUserMaskRemove(mxid: String)
    @JvmStatic external fun nativeUserMaskResolveName(mxid: String, originalName: String): String
    @JvmStatic external fun nativeUserMaskResolveAvatar(mxid: String, originalUrl: String): String
    @JvmStatic external fun nativeUserMaskExportJson(): String
    @JvmStatic external fun nativeUserMaskImportJson(json: String)
    @JvmStatic external fun nativeIsValidMxid(mxid: String): Boolean
    @JvmStatic external fun nativeUserMaskClear()
    @JvmStatic external fun nativeUserMaskCount(): Int

    // --- Chunked Upload ---

    @JvmStatic external fun nativeUploaderSetChunkSizeMb(mb: Int)
    @JvmStatic external fun nativeUploaderComputeChunks(fileSize: Long): Int
    @JvmStatic external fun nativeUploaderGetChunkJson(index: Int): String
    @JvmStatic external fun nativeUploaderAdvance()
    @JvmStatic external fun nativeUploaderCancel()
    @JvmStatic external fun nativeUploaderFail(error: String)
    @JvmStatic external fun nativeUploaderReset()
    @JvmStatic external fun nativeUploaderProgressJson(): String
    @JvmStatic external fun nativeUploaderContentRange(chunkIndex: Int): String
    @JvmStatic external fun nativeSuggestChunkSizeMb(fileSize: Long): Int

    // --- Chat Features (Timezone + EXIF) ---

    @JvmStatic external fun nativeGetCommonTimezones(): String
    @JvmStatic external fun nativeFormatTimestampInTimezone(utcMs: Long, tzId: String): String
    @JvmStatic external fun nativeIsValidTimezoneId(tzId: String): Boolean
    @JvmStatic external fun nativeFileHasMetadata(mimeType: String): Boolean
    @JvmStatic external fun nativeGetStrippableMimeTypes(): String

    // --- Invitation Hide ---

    @JvmStatic external fun nativeInviteHide(roomId: String, roomName: String, inviterName: String, inviterMxid: String)
    @JvmStatic external fun nativeInviteUnhide(roomId: String)
    @JvmStatic external fun nativeInviteIsHidden(roomId: String): Boolean
    @JvmStatic external fun nativeInviteExportJson(): String
    @JvmStatic external fun nativeInviteImportJson(json: String)
    @JvmStatic external fun nativeInviteClear()
    @JvmStatic external fun nativeInviteCount(): Int

    // --- Thread Aggregator ---

    @JvmStatic external fun nativeThreadAdd(
        threadId: String, roomId: String, roomName: String,
        accountId: String, accountIndex: String,
        lastMsg: String, lastSender: String,
        lastTs: Long, replyCount: Int, unread: Boolean
    )
    @JvmStatic external fun nativeThreadGetAllJson(): String
    @JvmStatic external fun nativeThreadClear()
    @JvmStatic external fun nativeThreadCount(): Int
    @JvmStatic external fun nativeThreadRemoveRoom(roomId: String)

    // --- User Messages ---

    @JvmStatic external fun nativeFormatUserMessagePreview(roomName: String, body: String, msgType: String, maxLen: Int): String

    // --- Room Version ---

    @JvmStatic external fun nativeGetRoomVersionsJson(): String
    @JvmStatic external fun nativeIsValidRoomVersion(version: String): Boolean

    // --- Chat Preview ---

    @JvmStatic external fun nativeFormatShortTime(epochMs: Long): String
    @JvmStatic external fun nativeTruncateMessage(body: String, maxLen: Int): String

    // --- RAM Monitor ---

    @JvmStatic external fun nativeGetMemoryInfo(): String
    @JvmStatic external fun nativeFormatMemoryLabel(rssKb: Long): String

    // --- Cache Manager ---

    @JvmStatic external fun nativeCacheTrack(eventId: String, roomId: String, roomName: String, timestamp: Long, sizeBytes: Long, msgType: String, body: String)
    @JvmStatic external fun nativeCacheStatsJson(): String
    @JvmStatic external fun nativeCacheGetByRoom(roomId: String): String
    @JvmStatic external fun nativeCacheGetOlderThan(beforeTs: Long): String
    // --- Message Aggregator (All Messages) ---

    @JvmStatic external fun nativeMsgAggAdd(eventId: String, roomId: String, roomName: String, accountId: String, accountIndex: String, senderName: String, body: String, msgType: String, originServerTs: Long)
    @JvmStatic external fun nativeMsgAggGetAllJson(): String
    @JvmStatic external fun nativeMsgAggClear()
    @JvmStatic external fun nativeMsgAggCount(): Int

    // --- Room Info ---

    @JvmStatic external fun nativeFormatCreationDate(epochMs: Long): String
    @JvmStatic external fun nativeIsLikelyFullHistory(cached: Int, estimated: Int): Boolean

    // --- Deleted Archive ---

    @JvmStatic external fun nativeDeletedArchiveAdd(eventId: String, roomId: String, roomName: String, senderName: String, body: String, msgType: String, timestamp: String, originTs: Long, deletedBy: String)
    @JvmStatic external fun nativeDeletedArchiveExportJson(): String
    @JvmStatic external fun nativeDeletedArchiveClear()
    @JvmStatic external fun nativeDeletedArchiveCount(): Int

    // --- Search Index ---

    @JvmStatic external fun nativeSearchIndexMessage(eventId: String, roomId: String, roomName: String, senderName: String, body: String, timestamp: Long, isEncrypted: Boolean)
    @JvmStatic external fun nativeSearchQuery(query: String, limit: Int): String
    @JvmStatic external fun nativeSearchClear()
    @JvmStatic external fun nativeSearchIndexedCount(): Int

    // --- Module Loader ---

    @JvmStatic external fun nativeModuleScanDir(dirPath: String)
    @JvmStatic external fun nativeModuleEnable(name: String)
    @JvmStatic external fun nativeModuleIsEnabled(name: String): Boolean
    @JvmStatic external fun nativeModuleListJson(): String

    // --- Notification Keywords ---

    @JvmStatic external fun nativeNotifKeywordAdd(keyword: String, caseSensitive: Boolean)
    @JvmStatic external fun nativeNotifKeywordCheck(body: String): String
    @JvmStatic external fun nativeNotifKeywordExport(): String
    @JvmStatic external fun nativeNotifKeywordImport(json: String)
    @JvmStatic external fun nativeNotifKeywordClear()

    // --- Reaction Preview ---

    @JvmStatic external fun nativeFormatReactionPreview(reactorName: String, emoji: String, sourceBody: String, sourceSender: String): String

    // --- Room Mirror ---

    @JvmStatic external fun nativeMirrorAdd(srcRoomId: String, srcRoomName: String, mirRoomId: String, mirRoomName: String, enabled: Boolean, useDolls: Boolean)
    @JvmStatic external fun nativeMirrorRemove(srcRoomId: String)
    @JvmStatic external fun nativeMirrorIsActive(srcRoomId: String): Boolean
    @JvmStatic external fun nativeMirrorFormatMessage(senderName: String, senderMxid: String, roomName: String, body: String, msgType: String, ts: Long): String
    @JvmStatic external fun nativeMirrorGenerateDollMxid(originalMxid: String, targetServer: String): String
    @JvmStatic external fun nativeMirrorIsValidDoll(mxid: String): Boolean
    @JvmStatic external fun nativeMirrorExportJson(): String

    // --- Input Tools ---

    @JvmStatic external fun nativeSymbolAdd(symbol: String, label: String)
    @JvmStatic external fun nativeSymbolExport(): String
    @JvmStatic external fun nativeSymbolImport(json: String)

    @JvmStatic external fun nativeReplacementAddRule(pattern: String, replacement: String, exactMatch: Boolean)
    @JvmStatic external fun nativeReplacementApply(text: String): String
    @JvmStatic external fun nativeReplacementExport(): String

    // --- LLM ---

    @JvmStatic external fun nativeBuildLlmRequest(prompt: String, provider: Int, endpoint: String, token: String, model: String, systemPrompt: String, temp: Float, maxTokens: Int): String
    @JvmStatic external fun nativeBuildLlmHeaders(provider: Int, token: String): String
    @JvmStatic external fun nativeParseLlmResponse(body: String, statusCode: Int, provider: Int): String
    @JvmStatic external fun nativeFormatLlmBroadcast(prompt: String, response: String): String

    // --- Duplicate Names ---

    @JvmStatic external fun nativeFormatUserDisplayName(displayName: String, mxid: String, showMxid: Boolean): String

    // --- MXID Visibility ---

    @JvmStatic external fun nativeMxidVisibilityHide(mxid: String)
    @JvmStatic external fun nativeMxidVisibilityShow(mxid: String)
    @JvmStatic external fun nativeMxidVisibilityIsVisible(mxid: String): Boolean
    @JvmStatic external fun nativeMxidVisibilityExport(): String

    // --- Read Receipts ---

    @JvmStatic external fun nativeComputeReceiptDisplay(receiptsJson: String, maxVisible: Int): String
    @JvmStatic external fun nativeFormatOverflowLabel(count: Int): String
    @JvmStatic external fun nativeFormatReceiptAccessibility(visibleJson: String, overflow: Int): String

    // --- Room Analytics ---

    @JvmStatic external fun nativeExtractServerName(mxid: String): String

    // --- User Hide Timer ---

    @JvmStatic external fun nativeUserHideFor(userId: String, displayName: String, minutes: Int)
    @JvmStatic external fun nativeUserHideIsHidden(userId: String): Boolean
    @JvmStatic external fun nativeUserHideGetActive(): String

    // --- Message Queue ---

    @JvmStatic external fun nativeMsgQueueEnqueue(msgId: String, roomId: String, body: String, formattedBody: String, order: Int, maxRetries: Int)
    @JvmStatic external fun nativeMsgQueueSetOrder(msgId: String, order: Int)
    @JvmStatic external fun nativeMsgQueueMarkFailed(msgId: String, error: String)
    @JvmStatic external fun nativeMsgQueueMarkSent(msgId: String)
    @JvmStatic external fun nativeMsgQueuePendingCount(): Int
    @JvmStatic external fun nativeMsgQueueExport(): String

    // --- Image Crop ---

    @JvmStatic external fun nativeIsValidCrop(imgW: Int, imgH: Int, x: Int, y: Int, w: Int, h: Int): Boolean

    // --- Auto-Scroll ---

    @JvmStatic external fun nativeComputeScrollPlan(smooth: Boolean, durationMin: Int, totalLines: Int, lineHeightPx: Int): String

    // --- Language Detection ---

    @JvmStatic external fun nativeDetectLanguage(text: String, method: Int): String
    @JvmStatic external fun nativeGetLanguageLabel(code: String): String

    // --- Language Hide ---

    @JvmStatic external fun nativeLangHideAdd(langCode: String, roomId: String, userId: String, specificUser: Boolean, minutes: Int)
    @JvmStatic external fun nativeLangHideIsHidden(langCode: String, roomId: String, userId: String): Boolean

    // --- Chat Push Down ---

    @JvmStatic external fun nativeChatPushDown(roomId: String, minutes: Int)
    @JvmStatic external fun nativeChatIsPushedDown(roomId: String): Boolean
    @JvmStatic external fun nativeChatPushDownRestore(roomId: String)

    // --- Emoji Blacklist ---

    @JvmStatic external fun nativeEmojiBlacklistAdd(emoji: String)
    @JvmStatic external fun nativeEmojiBlacklistRemove(emoji: String)
    @JvmStatic external fun nativeEmojiBlacklistIsBlocked(emoji: String): Boolean
    @JvmStatic external fun nativeEmojiBlacklistExport(): String
    @JvmStatic external fun nativeEmojiBlacklistImport(json: String)

    // --- Avatar History ---

    @JvmStatic external fun nativeAvatarAddChange(mxcUrl: String, eventId: String, timestamp: Long)
    @JvmStatic external fun nativeAvatarExportJson(): String
    @JvmStatic external fun nativeAvatarClear()

    // --- Jump to Date with Time ---

    @JvmStatic external fun nativeParseJumpToDate(input: String): String

    // --- Room Matching ---

    @JvmStatic external fun nativeMatchRooms(query: String, roomsJson: String): String
    @JvmStatic external fun nativeIsRoomId(input: String): Boolean
    @JvmStatic external fun nativeIsRoomAlias(input: String): Boolean

    // --- Event Links ---

    @JvmStatic external fun nativeExtractEventLinks(body: String): String
    @JvmStatic external fun nativeFormatResolvedEvent(sender: String, body: String): String
    @JvmStatic external fun nativeIsEventId(text: String): Boolean

    // --- Timestamps ---

    @JvmStatic external fun nativeFormatTimestamp(epochMs: Long, includeSeconds: Boolean): String
    @JvmStatic external fun nativeFormatFullTimestamp(epochMs: Long): String

    // --- Lightweight Call ---

    @JvmStatic external fun nativeLightCallEnter(): String
    @JvmStatic external fun nativeLightCallExit(): String
    @JvmStatic external fun nativeLightCallAssessMemory(): String
    @JvmStatic external fun nativeShouldUseLightweightMode(): Boolean

    // --- Scheduled Edits ---

    @JvmStatic external fun nativeSchedEditSchedule(roomId: String, targetEventId: String, newContent: String, contentUrl: String, formattedContent: String, formattedUrl: String, scheduledAtMs: Long, recurring: Boolean): String
    @JvmStatic external fun nativeSchedEditCancel(editId: String)
    @JvmStatic external fun nativeSchedEditGetDue(): String
    @JvmStatic external fun nativeSchedEditMarkApplied(editId: String)
    @JvmStatic external fun nativeSchedEditMarkFailed(editId: String, error: String)
    @JvmStatic external fun nativeSchedEditExport(): String
    @JvmStatic external fun nativeSchedEditStats(): String

    // --- SVG Rendering ---

    @JvmStatic external fun nativeParseSvg(svgData: String): String
    @JvmStatic external fun nativeIsValidSvg(data: String): Boolean

    // --- Drawing Canvas ---

    @JvmStatic external fun nativeDrawMoveTo(x: Double, y: Double)
    @JvmStatic external fun nativeDrawLineTo(x: Double, y: Double)
    @JvmStatic external fun nativeDrawSetColor(argb: Int)
    @JvmStatic external fun nativeDrawSetWidth(w: Double)
    @JvmStatic external fun nativeDrawExportJson(): String
    @JvmStatic external fun nativeDrawToSvgPath(): String
    @JvmStatic external fun nativeDrawClear()

    // --- Profile Swiper ---

    @JvmStatic external fun nativeProfileSwiperSetProfiles(profilesJson: String)
    @JvmStatic external fun nativeProfileSwiperNext(): String
    @JvmStatic external fun nativeProfileSwiperPrev(): String

    // --- Rainbow Generator ---

    @JvmStatic external fun nativeGenerateRainbow(text: String): String

    // --- Text Formatting ---

    @JvmStatic external fun nativeFormatSpoiler(text: String): String
    @JvmStatic external fun nativeFormatEmote(sender: String, text: String): String
    @JvmStatic external fun nativeFormatShrug(text: String): String
    @JvmStatic external fun nativeFormatLenny(text: String): String
    @JvmStatic external fun nativeFormatTableFlip(text: String): String
    @JvmStatic external fun nativeFormatPlain(text: String): String
    @JvmStatic external fun nativeIsEmojiOnly(text: String): Boolean
    @JvmStatic external fun nativeTruncateText(text: String, maxLen: Int): String

    // --- URL Tools ---

    @JvmStatic external fun nativeParseUrl(url: String): String
    @JvmStatic external fun nativeExtractFirstUrl(text: String): String
    @JvmStatic external fun nativeUrlEncode(input: String): String
    @JvmStatic external fun nativeUrlDecode(input: String): String
    @JvmStatic external fun nativeBuildMatrixToUrl(roomId: String): String

    // --- Notification Priority ---

    @JvmStatic external fun nativeComputeNotifPriority(isDM: Boolean, isMention: Boolean, isRoomMention: Boolean, isKeyword: Boolean, isCall: Boolean, isBackground: Boolean, dnd: Boolean, favorite: Boolean): String
    @JvmStatic external fun nativeFormatNotifTitle(roomName: String, senderName: String, isDM: Boolean): String
    @JvmStatic external fun nativeFormatNotifBody(body: String, senderName: String, isDM: Boolean, showSender: Boolean): String
    @JvmStatic external fun nativeIsRoomMention(body: String): Boolean

    // --- Matrix Patterns ---

    @JvmStatic external fun nativeIsUserId(input: String): Boolean
    @JvmStatic external fun nativeParseMatrixToPermalink(url: String): String
    @JvmStatic external fun nativeIsValidEmail(input: String): Boolean

    // --- Desync Detector ---

    @JvmStatic external fun nativeDesyncTrackEvent(eventId: String, serverName: String, timestamp: Long)
    @JvmStatic external fun nativeDesyncCheck(roomId: String, currentServer: String): String

    // --- Latency Tracker ---

    @JvmStatic external fun nativeLatencyRecord(latencyMs: Double, server: String, endpoint: String, success: Boolean)
    @JvmStatic external fun nativeLatencyStats(): String
    @JvmStatic external fun nativeLatencyStatsText(): String

    // --- String Utils ---

    @JvmStatic external fun nativeSanitizeRoomName(input: String): String
    @JvmStatic external fun nativeWordCount(input: String): Int

    // --- Location Sharing ---

    @JvmStatic external fun nativeLocationStartSession(roomId: String, userId: String, intervalSec: Int, autoStop: Boolean, autoStopMin: Int): String
    @JvmStatic external fun nativeLocationStopSession(sessionId: String)
    @JvmStatic external fun nativeLocationIsDue(sessionId: String): Boolean
    @JvmStatic external fun nativeLocationFormatMessage(lat: Double, lon: Double, acc: Double, label: String): String
    @JvmStatic external fun nativeLocationFormatGeoJson(lat: Double, lon: Double, acc: Double): String
    @JvmStatic external fun nativeLocationDistance(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double

    // --- Color Utils ---

    @JvmStatic external fun nativeContrastRatio(fgR: Int, fgG: Int, fgB: Int, bgR: Int, bgG: Int, bgB: Int): Double
    @JvmStatic external fun nativeParseColor(input: String): String

    // --- E2EE Utils ---

    @JvmStatic external fun nativeGetTrustLabel(level: Int): String
    @JvmStatic external fun nativeGetTrustBadge(level: Int): String

    // --- Thumbnail ---

    @JvmStatic external fun nativeComputeThumbnail(srcW: Int, srcH: Int, maxW: Int, maxH: Int, upscale: Boolean, quality: Int): String
    @JvmStatic external fun nativeBuildThumbnailUrl(mxcUri: String, w: Int, h: Int, method: String, animated: Boolean): String

    // --- Waveform ---

    @JvmStatic external fun nativeSuggestBarCount(durationMs: Long): Int
    @JvmStatic external fun nativeComputeRmsVolume(samples: IntArray): Double

    // --- Session Timeout ---

    @JvmStatic external fun nativeShouldLock(lockMethod: Int, idleTimeoutMin: Int, maxSessionMin: Int, maxFailedPin: Int, lockOnBg: Boolean, lastActivityMs: Long, sessionStartMs: Long, failedAttempts: Int, isLocked: Boolean, isBackground: Boolean): Boolean
    @JvmStatic external fun nativeIsValidPin(pin: String, minLen: Int, maxLen: Int): Boolean

    // --- Password Validator ---

    @JvmStatic external fun nativeValidatePassword(password: String): String

    // --- Spellcheck ---

    @JvmStatic external fun nativeEditDistance(a: String, b: String): Int

    // --- Typing Indicator ---

    @JvmStatic external fun nativeFormatTypingText(namesJson: String): String

    // --- Link Preview ---

    @JvmStatic external fun nativeIsImageUrl(url: String): Boolean

    // --- Hash Utils ---

    @JvmStatic external fun nativeSha256Hex(input: String): String
    @JvmStatic external fun nativeGenerateToken(bytes: Int): String

    // --- Room Stats ---

    @JvmStatic external fun nativeComputeMessagesPerDay(count: Int, firstTs: Long, lastTs: Long): Double

    // --- Mention Parser ---

    @JvmStatic external fun nativeBuildUserPill(userId: String, displayName: String): String
    @JvmStatic external fun nativeStripPills(html: String): String

    // --- Poll Utils ---

    @JvmStatic external fun nativeIsValidPollQuestion(question: String): Boolean

    // --- Reaction Utils ---

    @JvmStatic external fun nativeGetQuickReactions(): String

    // --- File Validator ---

    @JvmStatic external fun nativeFormatFileSize(bytes: Long): String
    @JvmStatic external fun nativeGetExtensionFromName(fileName: String): String

    // --- Date Utils ---

    @JvmStatic external fun nativeFormatChatTimestamp(epochMs: Long, includeSeconds: Boolean): String
    @JvmStatic external fun nativeFormatRelativeTime(epochMs: Long): String

    // --- Message Queue ---

    @JvmStatic external fun nativeTextSimilarity(a: String, b: String): Double

    // --- Pinned Events (Element Web parity) ---

    @JvmStatic external fun nativeParsePinnedEventIds(stateJson: String): String
    @JvmStatic external fun nativeBuildPinnedEventsContent(idsJson: String): String

    // --- Server Capabilities ---

    @JvmStatic external fun nativeParseServerCapabilities(wellKnownJson: String): String

    // --- Username Validator ---

    @JvmStatic external fun nativeValidateUsername(username: String): String

    // --- Emoji Analyzer ---

    @JvmStatic external fun nativeCountEmojis(text: String): Int
    @JvmStatic external fun nativeIsEmojiOnlyMessage(text: String): Boolean

    // --- Identity Utils ---

    @JvmStatic external fun nativeResolveMatrixId(input: String): String
    @JvmStatic external fun nativeGetInitials(name: String): String

    // --- Notification Analyzer ---

    @JvmStatic external fun nativeSuggestQuietHours(byHourJson: String): String

    // --- Sync Analyzer ---

    @JvmStatic external fun nativeFormatProgressBar(percent: Double, width: Int): String

    // --- User Rating ---

    @JvmStatic external fun nativeComputeStreak(timestampsJson: String): String

    // --- Event Timeline ---

    @JvmStatic external fun nativeFormatGroupLabel(timestampMs: Long): String

    // --- Room Directory ---

    @JvmStatic external fun nativeExtractServers(roomsJson: String): String

    // --- Widget Utils ---

    @JvmStatic external fun nativeGetWidgetTypeName(type: String): String

    // --- SSO Utils ---

    @JvmStatic external fun nativeValidateHomeserverUrl(input: String): String

    // --- Backup Utils ---

    @JvmStatic external fun nativeIsValidRecoveryKey(key: String): Boolean

    // --- Device Manager ---

    @JvmStatic external fun nativeFormatDeviceLastSeen(lastSeenMs: Long): String
    @JvmStatic external fun nativeIsDeviceInactive(lastSeenMs: Long): Boolean

    // --- Presence ---

    @JvmStatic external fun nativeFormatPresence(presence: Int): String
    @JvmStatic external fun nativeFormatPresenceLine(presence: Int, lastActiveAgoMs: Long, statusMsg: String): String

    // --- Room Permissions ---

    @JvmStatic external fun nativeGetSuggestedRole(plJson: String, userId: String): String

    // --- Room Summary ---

    @JvmStatic external fun nativeFormatLastMessagePreview(sender: String, body: String, encrypted: Boolean): String

    // --- Membership ---

    @JvmStatic external fun nativeFormatMembership(membershipStr: String): String

    // --- Event Validator ---

    @JvmStatic external fun nativeValidateEvent(eventId: String, eventType: String, senderId: String, contentJson: String, originTs: String, blockedUsersJson: String): String

    // --- Room Encryption ---

    @JvmStatic external fun nativeIsRoomEncrypted(stateJson: String): Boolean

    // --- Login Utils ---

    @JvmStatic external fun nativeBuildLoginBody(userId: String, password: String, deviceName: String, deviceId: String): String

    // --- Account Utils ---

    @JvmStatic external fun nativeValidatePasswordWithUsername(password: String, username: String): String

    // --- Connection Monitor ---

    @JvmStatic external fun nativeConnMonitorOnConnected()
    @JvmStatic external fun nativeConnMonitorOnDisconnected()
    @JvmStatic external fun nativeConnMonitorGetStatus(): String

    // --- Push Rules ---

    @JvmStatic external fun nativeParsePushCondition(conditionJson: String): String

    // --- Space Utils ---

    @JvmStatic external fun nativeBuildSpaceChildContent(suggested: Boolean, order: String, autoJoin: Boolean, canonical: Boolean): String

    // --- Event Relations ---

    @JvmStatic external fun nativeParseEventRelation(contentJson: String): String

    // --- E2EE Decoration ---

    @JvmStatic external fun nativeComputeE2eeDecoration(encrypted: Boolean, verified: Boolean, crossSigned: Boolean, decryptError: Boolean, blacklisted: Boolean, beforeJoined: Boolean, errorReason: String): String

    // --- Room List ---

    @JvmStatic external fun nativeGetBadgeText(count: Int, highlights: Int): String

    // --- Media Utils ---

    @JvmStatic external fun nativeMimeToMsgType(mimeType: String): String
    @JvmStatic external fun nativeIsValidBlurhash(hash: String): Boolean

    // --- Notification Settings ---

    @JvmStatic external fun nativeFormatNotifMode(mode: Int): String

    // --- Invite Utils ---

    @JvmStatic external fun nativeBuildInviteBody(userId: String, reason: String): String

    // --- Verification ---

    @JvmStatic external fun nativeGetVerificationEmojis(): String

    // --- Session Manager ---

    @JvmStatic external fun nativeFormatSessionBadge(unread: Int, highlights: Int): String

    // --- Auth Utils ---

    @JvmStatic external fun nativeFormatRateLimitMessage(responseJson: String, httpStatus: Int): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeIsServerNotice(contentJson: String): Boolean

    // --- Event Encryption ---

    @JvmStatic external fun nativeParseEncryptedHeader(contentJson: String): String

    // --- Report Utils ---

    @JvmStatic external fun nativeGetReasonDescription(code: String): String

    // --- WebRTC Utils ---

    @JvmStatic external fun nativeFormatCallDuration(seconds: Int): String
    @JvmStatic external fun nativeIsCallEvent(eventType: String): Boolean

    // --- Message Retry ---

    @JvmStatic external fun nativeComputeRetryDelay(retryCount: Int, maxDelayMs: Int): Int
    @JvmStatic external fun nativeDecideRetry(retryCount: Int, errorCode: Int, retryAfterHeader: String): String
    @JvmStatic external fun nativeFormatMessageStatus(state: Int): String

    // --- Sync Utils ---

    @JvmStatic external fun nativeBuildSyncFilter(includeThreads: Boolean, includePresence: Boolean, timelineLimit: Int): String

    // --- Event Display ---

    @JvmStatic external fun nativeClassifyEvent(eventType: String, msgType: String): Int

    // --- Permalink ---

    @JvmStatic external fun nativeBuildEventPermalink(roomId: String, eventId: String): String

    // --- Network Monitor ---

    @JvmStatic external fun nativeGetRecommendedMediaQuality(type: Int, connected: Boolean, metered: Boolean, signal: Int, latency: Double, loss: Double): String

    // --- Client Info ---

    @JvmStatic external fun nativeCompareSemver(a: String, b: String): Int

    // --- Keyshare ---

    @JvmStatic external fun nativeBuildKeyRequestBody(roomId: String, sessionId: String, senderKey: String, algorithm: String, requestId: String, deviceId: String): String

    // --- Display Name ---

    @JvmStatic external fun nativeUserIdToDisplayName(userId: String): String
    @JvmStatic external fun nativeUserIdToColor(userId: String): String

    // --- Message Location ---

    @JvmStatic external fun nativeEstimatePaginationRequests(missingEvents: Int, pageSize: Int): Int

    // --- Timeline Utils ---

    @JvmStatic external fun nativeShouldAutoScroll(isOwnMessage: Boolean): Boolean

    // --- Cross Signing ---

    @JvmStatic external fun nativeParseCrossSigningStatus(accountDataJson: String, userId: String): String

    // --- Edit History ---

    @JvmStatic external fun nativeGetEditBadgeText(editCount: Int): String

    // --- Read Marker / Unread Count ---
    // Ported from: TimelineViewModel.kt (read marker index math)
    //              ReadMarkers.kt (server-side read marker management)
    //              RoomSummary.kt (unread count display)

    /**
     * Compute the read marker position and unread statistics.
     * @return JSON with lastReadEventId, firstUnreadEventId, unreadCount,
     *         unreadMentions, hasUnread, readMarkerIndex
     */
    @JvmStatic external fun nativeComputeReadMarker(
        lastReadEventId: String,
        loadedEventIds: Array<String>,
        loadedSenders: Array<String>,
        isMention: BooleanArray,
        isHighlight: BooleanArray,
        myUserId: String
    ): String

    @JvmStatic external fun nativeShouldShowJumpToUnread(readMarkerJson: String): Boolean

    @JvmStatic external fun nativeFormatUnreadJumpLabel(readMarkerJson: String): String

    @JvmStatic external fun nativeAdvanceReadMarker(roomId: String, latestEventId: String): String

    // --- Kotlin fallbacks for Read Marker ---

    fun computeReadMarkerFallback(
        lastReadEventId: String,
        loadedEventIds: Array<String>,
        loadedSenders: Array<String>,
        isMention: BooleanArray,
        isHighlight: BooleanArray,
        myUserId: String
    ): JSONObject {
        val result = JSONObject()
        result.put("lastReadEventId", lastReadEventId)
        result.put("firstUnreadEventId", "")
        result.put("unreadCount", 0)
        result.put("unreadMentions", 0)
        result.put("hasUnread", false)
        result.put("readMarkerIndex", -1)

        if (lastReadEventId.isEmpty() || loadedEventIds.isEmpty()) return result

        val markerIndex = loadedEventIds.indexOf(lastReadEventId)
        if (markerIndex < 0) return result

        result.put("readMarkerIndex", markerIndex)

        var unreadCount = loadedEventIds.size - markerIndex - 1
        var unreadMentions = 0
        for (i in markerIndex + 1 until loadedEventIds.size) {
            if (i < loadedSenders.size && loadedSenders[i] == myUserId) {
                unreadCount--
                continue
            }
            if (i < isMention.size && isMention[i]) unreadMentions++
        }

        result.put("unreadCount", maxOf(0, unreadCount))
        result.put("unreadMentions", unreadMentions)
        result.put("hasUnread", unreadCount > 0)
        if (markerIndex + 1 < loadedEventIds.size) {
            result.put("firstUnreadEventId", loadedEventIds[markerIndex + 1])
        }
        return result
    }

    fun shouldShowJumpToUnreadFallback(readMarkerJson: String): Boolean {
        return readMarkerJson.contains("\"hasUnread\": true") ||
               readMarkerJson.contains("\"hasUnread\":true")
    }

    fun formatUnreadJumpLabelFallback(unreadCount: Int, unreadMentions: Int): String {
        val sb = StringBuilder()
        sb.append(unreadCount)
        if (unreadCount == 1) sb.append(" new message")
        else sb.append(" new messages")
        if (unreadMentions > 0) sb.append(" ($unreadMentions mentions)")
        return sb.toString()
    }

    fun advanceReadMarkerFallback(roomId: String, latestEventId: String): String {
        return latestEventId
    }

    // --- Slash Commands ---
    // Ported from: SlashCommandParser.kt, SlashCommand.kt, Command.kt

    @JvmStatic external fun nativeParseSlashCommand(text: String): String
    @JvmStatic external fun nativeFormatSlashCommand(command: String, arguments: String, type: Int, senderDisplayName: String): String
    @JvmStatic external fun nativeIsKnownSlashCommand(text: String): Boolean
    @JvmStatic external fun nativeGetAvailableCommands(): String

    // --- Typing Monitor ---
    // Ported from: TypingUsersTracker.kt, TypingHelper.kt, TypingView.kt

    @JvmStatic external fun nativeUpdateTypingState(roomId: String, typingUserIds: Array<String>, displayNames: Array<String>, nowMs: Long): String
    @JvmStatic external fun nativeFormatTypingText(typingStateJson: String): String
    @JvmStatic external fun nativeIsUserTyping(typingStateJson: String, userId: String, nowMs: Long): Boolean

    // --- URL Preview / OpenGraph ---
    // Ported from: UrlPreviewer.kt, EventHtmlRenderer.kt

    @JvmStatic external fun nativeParseUrlPreview(html: String, baseUrl: String): String
    @JvmStatic external fun nativeExtractHtmlTitle(html: String): String
    @JvmStatic external fun nativeResolveUrl(baseUrl: String, relative: String): String
    @JvmStatic external fun nativeIsImageUrl(url: String): Boolean
    @JvmStatic external fun nativeStripHtmlTags(html: String): String

    // --- Power Levels / Room Permissions ---
    // Ported from: PowerLevelsContent.kt, RoomPermissions.kt, PowerLevelsHelper.kt

    @JvmStatic external fun nativeParsePowerLevels(json: String): String
    @JvmStatic external fun nativeComputeUserPermissions(powerLevelsJson: String, userId: String): String
    @JvmStatic external fun nativeGetUserRole(powerLevel: Int): String

    // --- Well-Known / Server Discovery ---
    // Ported from: WellKnown.kt, LoginServerUrlFormatter.kt

    @JvmStatic external fun nativeParseWellKnown(json: String): String
    @JvmStatic external fun nativeFormatServerUrl(input: String): String
    @JvmStatic external fun nativeExtractServerName(mxid: String): String
    @JvmStatic external fun nativeIsValidHomeserverUrl(url: String): Boolean
    @JvmStatic external fun nativeNeedsWellKnownLookup(input: String): Boolean

    // --- Room Sort / Ordering ---
    // Ported from: RoomComparator.kt, RoomListViewModel.kt

    @JvmStatic external fun nativeSortRooms(roomsJson: String): String
    @JvmStatic external fun nativeGetRoomSortKey(lastEventTs: Long, notifCount: Int, highlightCount: Int, isDirect: Boolean, hasUnread: Boolean, tagStr: String, priority: Int): Int
    @JvmStatic external fun nativeGetRoomSectionName(tagStr: String, isDirect: Boolean): String

    // --- Key Backup / Recovery ---
    // Ported from: KeysBackup.kt, KeysBackupSetupSharedViewModel.kt

    @JvmStatic external fun nativeFormatRecoveryKey(raw: String): String
    @JvmStatic external fun nativeValidateRecoveryKey(key: String): String
    @JvmStatic external fun nativeParseKeyBackupVersion(json: String): String
    @JvmStatic external fun nativeIsValidPassphrase(passphrase: String): Boolean
    @JvmStatic external fun nativeComputeRecoveryKey(curve25519Key: String): String
    @JvmStatic external fun nativeParseMatrixError(json: String): String
    @JvmStatic external fun nativeGetErrorDescription(errorCode: String): String

    // --- AI Agent Executor ---
    // Enables /agent <task> — LLM-driven chat automation like Claude Code

    @JvmStatic external fun nativeBuildAgentSystemPrompt(systemPrompt: String): String
    @JvmStatic external fun nativeProcessAgentIteration(stateJson: String, llmResponse: String): String
    @JvmStatic external fun nativeParseToolCalls(llmResponse: String): String
    @JvmStatic external fun nativeExtractTextAnswer(llmResponse: String): String

    // --- Push Condition Evaluator ---
    // Ported from: EventMatchCondition.kt, Glob.kt

    @JvmStatic external fun nativeEvaluatePushCondition(eventJson: String, key: String, pattern: String): Boolean
    @JvmStatic external fun nativeExtractJsonField(json: String, fieldPath: String): String

    // --- Content Utils (MXC URLs, Message Types) ---
    // Ported from: ContentUrlResolver.kt, MessageContent.kt

    @JvmStatic external fun nativeResolveMxcDownloadUrl(mxcUrl: String, homeServerUrl: String): String
    @JvmStatic external fun nativeParseMessageContent(contentJson: String): String
    @JvmStatic external fun nativeIsMxcUri(url: String): Boolean
    @JvmStatic external fun nativeFormatFileSize(bytes: Long): String

    // --- Room State Parsers ---
    // Ported from: RoomJoinRules.kt, RoomHistoryVisibility.kt, RoomGuestAccess.kt, RoomCreate.kt

    @JvmStatic external fun nativeParseJoinRules(contentJson: String): String
    @JvmStatic external fun nativeParseHistoryVisibility(contentJson: String): String
    @JvmStatic external fun nativeParseGuestAccess(contentJson: String): String
    @JvmStatic external fun nativeParseRoomCreate(contentJson: String): String
    @JvmStatic external fun nativeParseTombstone(contentJson: String): String

    // --- Login Flow Parser ---
    // Ported from: LoginWizard.kt, LoginFlow.kt, LoginFlowTypes.kt

    @JvmStatic external fun nativeParseLoginFlows(json: String): String
    @JvmStatic external fun nativeGetLoginFlowDescription(type: String): String
    @JvmStatic external fun nativeGetSsoProviderIcon(providerId: String): String

    // --- Device Naming / User Agent ---
    // Ported from: ComputeUserAgentUseCase.kt

    @JvmStatic external fun nativeBuildUserAgent(appName: String, appVersion: String, manufacturer: String, model: String, androidVersion: String, buildId: String, flavor: String, sdkVersion: String): String
    @JvmStatic external fun nativeBuildDeviceDisplayName(appName: String, deviceModel: String): String
    @JvmStatic external fun nativeShortDeviceName(manufacturer: String, model: String): String

    // --- Sync Filter Builder ---
    // Ported from: SyncFilterParams.kt, SyncFilterBuilder.kt

    @JvmStatic external fun nativeBuildSyncFilter(lazyLoadState: Boolean, lazyLoadMessages: Boolean, useThreadNotif: Boolean, canUseThreadNotif: Boolean, timelineTypes: Array<String>, stateTypes: Array<String>): String
    @JvmStatic external fun nativeGetDefaultSyncFilter(): String

    // --- Server Capabilities (updated from HomeServerCapabilities.kt) ---

    @JvmStatic external fun nativeParseServerCapabilities(json: String): String
    @JvmStatic external fun nativeIsDelegatedOidcEnabled(authenticationIssuer: String): Boolean

    // --- Room Display Name ---
    // Ported from: RoomDisplayNameResolver.kt

    @JvmStatic external fun nativeParseRoomNameContent(contentJson: String): String
    @JvmStatic external fun nativeNormalizeRoomName(name: String): String

    // --- Notification Count Formatter ---
    // Ported from: RoomSummaryFormatter.kt

    @JvmStatic external fun nativeFormatUnreadCounter(count: Int): String
    @JvmStatic external fun nativeFormatBadgeText(totalCount: Int): String

    // --- Permalink Parser (updated) ---

    @JvmStatic external fun nativeParsePermalinkFull(url: String): String

    // --- Kotlin fallbacks for Slash Commands ---

    fun parseSlashCommandFallback(text: String): JSONObject {
        val result = JSONObject()
        result.put("isSlashCommand", false)
        result.put("command", "")
        result.put("arguments", "")
        result.put("type", -1)
        result.put("needsMatrixId", false)
        result.put("isMessage", false)
        result.put("isAdmin", false)
        if (text.isEmpty() || !text.startsWith("/")) return result

        val spaceIdx = text.indexOf(' ')
        val cmdPart = if (spaceIdx < 0) text else text.substring(0, spaceIdx)
        val args = if (spaceIdx >= 0) text.substring(spaceIdx + 1).trim() else ""
        val lowered = cmdPart.lowercase()

        val commands = mapOf(
            "/me" to 1, "/join" to 2, "/part" to 3, "/invite" to 4,
            "/kick" to 5, "/ban" to 6, "/unban" to 7, "/op" to 8,
            "/deop" to 9, "/nick" to 10, "/topic" to 11, "/roomname" to 12,
            "/notice" to 13, "/shrug" to 14, "/tableflip" to 15, "/unflip" to 16,
            "/lenny" to 17, "/rainbow" to 18, "/rainbowme" to 19, "/plain" to 20,
            "/spoiler" to 21, "/avatar" to 22, "/discardsession" to 23,
            "/clearscalartoken" to 24, "/markdown" to 25
        )
        val type = commands[lowered] ?: return result

        result.put("isSlashCommand", true)
        result.put("command", cmdPart)
        result.put("arguments", args)
        result.put("type", type)
        result.put("isMessage", type in listOf(1, 13, 14, 15, 16, 17, 18, 19, 20, 21, 25))
        result.put("isAdmin", type in listOf(5, 6, 7, 8, 9))
        return result
    }

    fun formatSlashCommandFallback(command: String, arguments: String, type: Int, sender: String): String {
        return when (type) {
            1, 19 -> "* $sender $arguments"
            13 -> arguments
            14 -> if (arguments.isNotEmpty()) "$arguments ¯\\_(ツ)_/¯" else "¯\\_(ツ)_/¯"
            15 -> "(╯°□°）╯︵ ┻━┻"
            16 -> "┬──┬ ノ( ゜-゜ノ)"
            17 -> "( ͡° ͜ʖ ͡°)"
            21 -> "||$arguments||"
            else -> arguments
        }
    }

    // --- Pure Kotlin fallback implementations ---

    fun validateAndBuildFallback(
        roomId: String,
        dateString: String,
        serverUrl: String,
        accessToken: String,
        isEnabled: Boolean
    ): JSONObject {
        val result = JSONObject()

        if (!isEnabled) {
            return result.put("error", "/jumptodate is disabled. Enable it in Settings → Labs.")
        }

        // Validate date format
        val dateRegex = Regex("^(\\d{4})-(\\d{2})-(\\d{2})$")
        val match = dateRegex.matchEntire(dateString)
            ?: return result.put("error", "Invalid date format. Use YYYY-MM-DD.")

        val year = match.groupValues[1].toInt()
        val month = match.groupValues[2].toInt()
        val day = match.groupValues[3].toInt()

        if (month < 1 || month > 12) return result.put("error", "Invalid month. Must be 01-12.")
        if (day < 1 || day > 31) return result.put("error", "Invalid day. Must be 01-31.")

        // Days per month validation
        val daysInMonth = intArrayOf(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
        val isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)
        val maxDay = if (month == 2 && isLeapYear) 29 else daysInMonth[month - 1]

        if (day > maxDay) return result.put("error", "Invalid day for given month.")

        // Compute Unix timestamp (UTC, milliseconds) using Java's Calendar
        val cal = java.util.GregorianCalendar(java.util.TimeZone.getTimeZone("UTC"))
        cal.set(year, month - 1, day, 0, 0, 0)
        cal.set(java.util.Calendar.MILLISECOND, 0)
        val timestamp = cal.timeInMillis

        // Build MSC3030 URL
        val url = "$serverUrl/_matrix/client/unstable/org.matrix.msc3030/rooms/$roomId/timestamp_to_event?ts=$timestamp&dir=f"

        result.put("url", url)
        result.put("accessToken", accessToken)
        result.put("timestamp", timestamp)

        return result
    }

    fun parseResponseFallback(responseBody: String?, httpStatus: Int): JSONObject {
        val result = JSONObject()

        if (responseBody == null) {
            return result.put("error", "Empty response from server.")
                    .put("statusCode", httpStatus)
        }

        if (httpStatus != 200) {
            try {
                val json = JSONObject(responseBody)
                val errcode = json.optString("errcode", "")
                val error = json.optString("error", "")
                val msg = buildString {
                    append("Server returned $httpStatus")
                    if (errcode.isNotEmpty()) append(" ($errcode)")
                    if (error.isNotEmpty()) append(": $error")
                }
                result.put("error", msg)
                result.put("statusCode", httpStatus)
                return result
            } catch (e: Exception) {
                return result.put("error", "Server returned $httpStatus")
                        .put("statusCode", httpStatus)
            }
        }

        try {
            val json = JSONObject(responseBody)
            val eventId = json.optString("event_id", "")

            if (eventId.isEmpty()) {
                return result.put("error", "Response missing event_id field.")
                        .put("statusCode", httpStatus)
            }

            if (eventId == "~") {
                return result.put("error", "No events found after the given date.")
                        .put("statusCode", httpStatus)
            }

            result.put("eventId", eventId)
            result.put("statusCode", httpStatus)
            return result
        } catch (e: Exception) {
            return result.put("error", "Failed to parse server response.")
                    .put("statusCode", httpStatus)
        }
    }

    fun parseRelationFallback(eventJson: String, allowedTypes: String): JSONObject {
        val result = JSONObject()
        try {
            val json = JSONObject(eventJson)
            val content = json.optJSONObject("content") ?: json
            val relatesTo = content.optJSONObject("m.relates_to")
                ?: content.optJSONObject("relates_to") ?: return result.put("isRelation", false)

            val sourceEventId = relatesTo.optString("event_id", "")
            val relationType = relatesTo.optString("rel_type", "")

            if (sourceEventId.isEmpty()) return result.put("isRelation", false)

            val jumpableTypes = setOf("m.annotation", "m.reference", "m.replace", "m.thread")
            if (allowedTypes.isNotEmpty() && !allowedTypes.split(",").any { it == relationType }) {
                return result.put("isRelation", false)
            }
            if (!jumpableTypes.contains(relationType)) {
                return result.put("isRelation", false)
            }

            result.put("isRelation", true)
            result.put("sourceEventId", sourceEventId)
            result.put("relationType", relationType)
            return result
        } catch (e: Exception) {
            return result.put("isRelation", false)
        }
    }

    // --- Export fallback implementations ---

    fun formatEventHtmlFallback(
        senderName: String, timestamp: String, body: String,
        msgType: String, fileName: String, mediaSize: String,
        relationType: String, isContinuation: Boolean
    ): String = buildString {
        if (!isContinuation) {
            appendLine("<div class=\"mx_EventTile\">")
            appendLine("  <div class=\"mx_EventTile_info\">")
            append("    <span class=\"mx_EventTile_sender\">").append(escapeHtmlFn(senderName)).appendLine("</span>")
            if (timestamp.isNotEmpty())
                append("    <span class=\"mx_MessageTimestamp\">").append(timestamp).appendLine("</span>")
            appendLine("  </div>")
        } else {
            appendLine("<div class=\"mx_EventTile mx_EventTile_continuation\">")
        }
        appendLine("  <div class=\"mx_EventTile_body\">")
        if (msgType in listOf("m.image", "m.video", "m.file", "m.audio")) {
            appendLine("    <div class=\"mx_EventTile_attachment\">")
            append("      <span class=\"mx_Attachment_name\">").append(fileName.ifEmpty { msgType.substring(2) }).appendLine("</span>")
            if (mediaSize.isNotEmpty())
                append("      <span class=\"mx_Attachment_size\">").append(mediaSize).appendLine(" bytes</span>")
            appendLine("    </div>")
        }
        if (body.isNotEmpty()) {
            appendLine("    <div class=\"mx_EventTile_content\">")
            append("      ").appendLine(escapeHtmlFn(body))
            appendLine("    </div>")
        }
        appendLine("  </div>")
        appendLine("</div>")
    }

    fun formatEventPlainTextFallback(
        senderName: String, timestamp: String, body: String,
        msgType: String, fileName: String, relationType: String
    ): String = buildString {
        if (timestamp.isNotEmpty()) append(timestamp).append(" - ")
        append(senderName).append(": ")
        if (msgType in listOf("m.image", "m.video", "m.file", "m.audio")) {
            append("[").append(msgType.substring(2)).append(" attached")
            if (fileName.isNotEmpty()) append(": ").append(fileName)
            append("]")
        }
        if (body.isNotEmpty()) append(" ").append(body)
        if (relationType == "m.reference") append(" (in reply)")
        appendLine()
    }

    fun buildHtmlExportFallback(
        roomName: String, roomTopic: String, exportDate: String, eventHtmls: Array<String>
    ): String = buildString {
        appendLine("<!DOCTYPE html>")
        appendLine("<html lang=\"en\">")
        appendLine("<head>")
        appendLine("<meta charset=\"UTF-8\">")
        append("<title>").append(escapeHtmlFn(roomName)).appendLine(" — Chat Export</title>")
        appendLine("<style>body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;margin:0;padding:16px;background:#f5f5f5;}.mx_EventTile{background:#fff;border-radius:8px;padding:12px 16px;margin-bottom:8px;box-shadow:0 1px 2px rgba(0,0,0,0.05);}.mx_EventTile_continuation{margin-top:-4px;border-radius:0 0 8px 8px;}.mx_EventTile_sender{font-weight:600;color:#333;margin-right:8px;}.mx_MessageTimestamp{color:#999;font-size:0.85em;}.mx_EventTile_body{color:#222;line-height:1.5;}.mx_EventTile_attachment{background:#f0f0f0;border-radius:4px;padding:8px;margin-bottom:8px;}.mx_ExportHeader{background:#fff;border-radius:8px;padding:16px;margin-bottom:16px;box-shadow:0 1px 3px rgba(0,0,0,0.1);}.mx_ExportHeader h1{margin:0 0 8px;font-size:1.5em;}.mx_ExportHeader p{margin:4px 0;color:#666;font-size:0.9em;}.mx_EventTile_content{white-space:pre-wrap;word-wrap:break-word;}.mx_EventTile_reaction{font-style:italic;color:#666;}</style>")
        appendLine("</head>")
        appendLine("<body>")
        appendLine("<div class=\"mx_ExportHeader\">")
        append("  <h1>").append(escapeHtmlFn(roomName)).appendLine("</h1>")
        if (roomTopic.isNotEmpty()) append("  <p>").append(escapeHtmlFn(roomTopic)).appendLine("</p>")
        append("  <p>Exported: ").append(exportDate).appendLine("</p>")
        append("  <p>Total messages: ").append(eventHtmls.size).appendLine("</p>")
        appendLine("</div>")
        for (html in eventHtmls) append(html)
        appendLine("<hr><p style=\"color:#999;text-align:center;\">Exported with Progressive Chat</p>")
        appendLine("</body></html>")
    }

    private fun escapeHtmlFn(s: String): String = s
        .replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace("\"", "&quot;")

    // --- Kotlin fallback cache (in-memory HashMap) ---

    data class CachedEvent(
        val eventId: String = "",
        val senderId: String = "",
        val senderName: String = "",
        val timestamp: String = "",
        val body: String = "",
        val msgType: String = "",
        val eventType: String = "",
        val relationType: String = "",
        val sourceEventId: String = "",
        val sentByMe: Boolean = false
    )

    private val fallbackCache = mutableMapOf<String, CachedEvent>()
    private val fallbackRelationIndex = mutableMapOf<String, MutableList<String>>()

    fun cachePutFallback(
        eventId: String, senderId: String, senderName: String, timestamp: String,
        body: String, msgType: String, eventType: String,
        relationType: String, sourceEventId: String, sentByMe: Boolean
    ) {
        val event = CachedEvent(eventId, senderId, senderName, timestamp, body, msgType, eventType, relationType, sourceEventId, sentByMe)
        synchronized(fallbackCache) {
            val old = fallbackCache[eventId]
            if (old != null && old.sourceEventId.isNotEmpty()) {
                fallbackRelationIndex[old.sourceEventId]?.remove(eventId)
            }
            fallbackCache[eventId] = event
            if (sourceEventId.isNotEmpty()) {
                fallbackRelationIndex.getOrPut(sourceEventId) { mutableListOf() }.add(eventId)
            }
        }
    }

    fun cacheGetContextFallback(eventId: String): JSONObject {
        val result = JSONObject()
        synchronized(fallbackCache) {
            val e = fallbackCache[eventId] ?: return result.put("cached", false)
            result.put("cached", true)
            result.put("eventId", e.eventId)
            result.put("senderId", e.senderId)
            result.put("senderName", e.senderName)
            result.put("timestamp", e.timestamp)
            result.put("body", e.body)
            result.put("msgType", e.msgType)
            result.put("eventType", e.eventType)
            result.put("sentByMe", e.sentByMe)
            result.put("hasFailed", false)
        }
        return result
    }

    fun cacheClearFallback() {
        synchronized(fallbackCache) {
            fallbackCache.clear()
            fallbackRelationIndex.clear()
        }
    }

    fun cacheSizeFallback(): Int = synchronized(fallbackCache) { fallbackCache.size }

    // --- Translation fallback ---

    fun buildTranslateRequestFallback(
        text: String, sourceLang: String, targetLang: String,
        apiEndpoint: String, apiToken: String, model: String
    ): JSONObject {
        val body = JSONObject()
        body.put("model", model)
        val messages = org.json.JSONArray()
        val systemMsg = JSONObject().apply {
            put("role", "system")
            var prompt = "You are a translator. Translate the following text"
            if (sourceLang.isNotEmpty()) prompt += " from $sourceLang"
            prompt += " to $targetLang. Output ONLY the translation, nothing else."
            put("content", prompt)
        }
        messages.put(systemMsg)
        messages.put(JSONObject().apply {
            put("role", "user")
            put("content", text)
        })
        body.put("messages", messages)
        body.put("temperature", 0.1)
        return body
    }

    fun parseTranslateResponseFallback(responseBody: String?, httpStatus: Int): JSONObject {
        val result = JSONObject()
        if (responseBody == null) return result.put("success", false).put("error", "Empty response")
        if (httpStatus != 200) {
            try {
                val json = JSONObject(responseBody)
                val error = json.optJSONObject("error")
                val msg = error?.optString("message", "Server returned $httpStatus") ?: "Server returned $httpStatus"
                return result.put("success", false).put("error", msg).put("statusCode", httpStatus)
            } catch (e: Exception) {
                return result.put("success", false).put("error", "Server returned $httpStatus").put("statusCode", httpStatus)
            }
        }
        try {
            val json = JSONObject(responseBody)
            val choices = json.optJSONArray("choices")
            if (choices == null || choices.length() == 0)
                return result.put("success", false).put("error", "No choices in response")
            val message = choices.getJSONObject(0).optJSONObject("message")
            if (message == null)
                return result.put("success", false).put("error", "No message in response")
            val content = message.optString("content", "")
            if (content.isEmpty())
                return result.put("success", false).put("error", "No content in response")
            result.put("success", true)
            result.put("translatedText", content)
            return result
        } catch (e: Exception) {
            return result.put("success", false).put("error", "Failed to parse response")
        }
    }

}
