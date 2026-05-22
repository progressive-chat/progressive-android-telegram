package chat.progressive.app.features.auth

import android.os.Bundle
import android.view.View
import androidx.core.view.isVisible
import chat.progressive.app.features.home.NativeSessionManager
import chat.progressive.app.features.home.TelegramChatRepository
import chat.progressive.app.native.ProgressiveNative
import im.vector.app.R
import im.vector.app.core.platform.VectorBaseActivity
import im.vector.app.databinding.ActivityTelegramAuthBinding

class TelegramAuthActivity : VectorBaseActivity<ActivityTelegramAuthBinding>(),
    TelegramAuthFragment.AuthCallback {

    override val rootView: View
        get() = views.root

    private var nativeHandle: Long = 0
    private var authFragment: TelegramAuthFragment? = null

    override fun getBinding() = ActivityTelegramAuthBinding.inflate(layoutInflater)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        authFragment = TelegramAuthFragment()
        supportFragmentManager.beginTransaction()
            .replace(R.id.telegramAuthContainer, authFragment!!)
            .commit()

        initNativeClient()
    }

    private fun initNativeClient() {
        try {
            ProgressiveNative.ensureLoaded()
            ProgressiveNative.tgInit()
        } catch (e: Exception) {
            views.telegramAuthLoading.isVisible = false
            authFragment?.showError("Failed to initialize native library: ${e.message}")
            return
        }

        val filesDir = getExternalFilesDir(null)?.absolutePath ?: filesDir.absolutePath
        val dbDir = "$filesDir/tdlib"

        val apiId = try {
            packageManager.getApplicationInfo(packageName, android.content.pm.PackageManager.GET_META_DATA)
                .metaData?.getInt("tg_api_id", 0) ?: 0
        } catch (_: Exception) { 0 }

        val apiHash = try {
            packageManager.getApplicationInfo(packageName, android.content.pm.PackageManager.GET_META_DATA)
                .metaData?.getString("tg_api_hash") ?: ""
        } catch (_: Exception) { "" }

        nativeHandle = ProgressiveNative.tgCreateClient(apiId, apiHash, dbDir, filesDir)

        if (nativeHandle == 0L) {
            views.telegramAuthLoading.isVisible = false
            authFragment?.showError("TDLib not available. Rebuild with PROGRESSIVE_HAS_TDLIB=1 and include the TDLib native library (.so).")
            return
        }

        authFragment?.bind(nativeHandle)
        views.telegramAuthLoading.isVisible = false
    }

    override fun onAuthReady(userId: String) {
        NativeSessionManager.setActiveSession(nativeHandle, userId)
        TelegramChatRepository.attach(nativeHandle)
        startActivity(im.vector.app.features.home.HomeActivity.newIntent(this, firstStartMainActivity = true))
        finish()
    }

    override fun onDestroy() {
        super.onDestroy()
        if (nativeHandle != 0L && !NativeSessionManager.isActive()) {
            ProgressiveNative.tgDestroyClient(nativeHandle)
        }
    }
}
