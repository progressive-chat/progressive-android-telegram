package im.vector.app.features.onboarding.ftueauth

import android.annotation.SuppressLint
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import com.google.android.material.card.MaterialCardView
import dagger.hilt.android.AndroidEntryPoint
import im.vector.app.R
import im.vector.app.core.resources.BuildMeta
import im.vector.app.databinding.FragmentFtueAuthSplashBinding
import im.vector.app.features.VectorFeatures
import im.vector.app.features.onboarding.OnboardingAction
import im.vector.app.features.onboarding.OnboardingFlow
import im.vector.app.features.settings.VectorPreferences
import im.vector.lib.strings.CommonStrings
import javax.inject.Inject

/**
 * Shows protocol selection (Matrix / Telegram) as two tappable cards,
 * each with its description visible at a glance.
 */
@AndroidEntryPoint
class FtueAuthSplashFragment :
        AbstractFtueAuthFragment<FragmentFtueAuthSplashBinding>() {

    @Inject lateinit var vectorPreferences: VectorPreferences
    @Inject lateinit var vectorFeatures: VectorFeatures
    @Inject lateinit var buildMeta: BuildMeta

    private var selectedProtocol = "matrix"

    override fun getBinding(inflater: LayoutInflater, container: ViewGroup?): FragmentFtueAuthSplashBinding {
        return FragmentFtueAuthSplashBinding.inflate(inflater, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        setupViews()
    }

    private fun setupViews() {
        setupProtocolCards()
        setupButtons()
        setupDebugInfo()
    }

    private fun setupProtocolCards() {
        views.cardMatrix.debouncedClicks { onProtocolSelected("matrix") }
        views.cardTelegram.debouncedClicks { onProtocolSelected("telegram") }
    }

    private fun onProtocolSelected(protocol: String) {
        selectedProtocol = protocol
        val isMatrix = protocol == "matrix"

        views.cardMatrix.apply {
            isChecked = isMatrix
            strokeWidth = if (isMatrix) 2 else 1
            setStrokeColor(
                if (isMatrix) getColor(im.vector.lib.ui.styles.R.color.colorPrimary)
                else getColor(im.vector.lib.ui.styles.R.color.vctr_content_quaternary)
            )
        }
        views.cardTelegram.apply {
            isChecked = !isMatrix
            strokeWidth = if (isMatrix) 1 else 2
            setStrokeColor(
                if (isMatrix) getColor(im.vector.lib.ui.styles.R.color.vctr_content_quaternary)
                else getColor(im.vector.lib.ui.styles.R.color.colorPrimary)
            )
        }

        views.radioMatrix.isChecked = isMatrix
        views.radioTelegram.isChecked = !isMatrix

        val isAlreadyHaveAccountEnabled = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
        views.loginSplashSubmit.apply {
            setText(
                if (isMatrix && isAlreadyHaveAccountEnabled) CommonStrings.login_splash_create_account
                else CommonStrings.login_splash_submit
            )
        }
        views.loginSplashAlreadyHaveAccount.isVisible = isMatrix && isAlreadyHaveAccountEnabled
    }

    private fun setupButtons() {
        val isAlreadyHaveAccountEnabled = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
        views.loginSplashSubmit.apply {
            setText(if (isAlreadyHaveAccountEnabled) CommonStrings.login_splash_create_account else CommonStrings.login_splash_submit)
            debouncedClicks { splashSubmit(isAlreadyHaveAccountEnabled) }
        }
        views.loginSplashAlreadyHaveAccount.apply {
            isVisible = vectorFeatures.isOnboardingAlreadyHaveAccountSplashEnabled()
            debouncedClicks { alreadyHaveAnAccount() }
        }
    }

    private fun setupDebugInfo() {
        if (buildMeta.isDebug || vectorPreferences.developerMode()) {
            views.loginSplashVersion.isVisible = true
            @SuppressLint("SetTextI18n")
            views.loginSplashVersion.text = "Version : ${buildMeta.versionName}\n" +
                    "Branch: ${buildMeta.gitBranchName} ${buildMeta.gitRevision}\n" +
                    "Protocol: ${if (selectedProtocol == "matrix") "Matrix" else "Telegram"}"
            views.loginSplashVersion.debouncedClicks { navigator.openDebug(requireContext()) }
        }
    }

    private fun splashSubmit(isAlreadyHaveAccountEnabled: Boolean) {
        val getStartedFlow = if (isAlreadyHaveAccountEnabled) OnboardingFlow.SignUp else OnboardingFlow.SignInSignUp
        viewModel.handle(OnboardingAction.SplashAction.OnProtocolSelected(protocol = selectedProtocol, onboardingFlow = getStartedFlow))
    }

    private fun alreadyHaveAnAccount() {
        viewModel.handle(OnboardingAction.SplashAction.OnProtocolSelected(protocol = selectedProtocol, onboardingFlow = OnboardingFlow.SignIn))
    }

    override fun resetViewModel() {
        // Nothing to do
    }

    private fun getColor(id: Int): Int {
        return try {
            requireContext().resources.getColor(id, requireContext().theme)
        } catch (e: Exception) {
            android.graphics.Color.GRAY
        }
    }
}
