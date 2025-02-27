#include "SettingsWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "GameFramework/GameUserSettings.h"

void USettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind Buttons
    if (ApplyButton)
    {
        ApplyButton->OnClicked.AddDynamic(this, &USettingsWidget::OnApplySettings);
    }
    if (CancelButton)
    {
        CancelButton->OnClicked.AddDynamic(this, &USettingsWidget::OnCancelSettings);
    }

    // Bind FPS Slider
    if (FPSLimitSlider)
    {
        FPSLimitSlider->OnValueChanged.AddDynamic(this, &USettingsWidget::OnFPSLimitChanged);
    }

    // Populate Fullscreen Dropdown
    if (FullscreenModeDropdown)
    {
        FullscreenModeDropdown->ClearOptions();
        FullscreenModeDropdown->AddOption("Fullscreen");
        FullscreenModeDropdown->AddOption("Windowed");
        FullscreenModeDropdown->AddOption("Windowed Fullscreen");
    }

    // Populate Resolution Dropdown
    if (ResolutionDropdown)
    {
        ResolutionDropdown->ClearOptions();
        ResolutionDropdown->AddOption("1280x720");
        ResolutionDropdown->AddOption("1920x1080");
        ResolutionDropdown->AddOption("2560x1440");
    }

    // Populate Quality Settings
    if (TextureQualityDropdown && ShadowQualityDropdown && ShaderQualityDropdown && AntiAliasingDropdown)
    {
        TextureQualityDropdown->ClearOptions();
        ShadowQualityDropdown->ClearOptions();
        ShaderQualityDropdown->ClearOptions();
        AntiAliasingDropdown->ClearOptions();

        for (FString Option : { "Low", "Medium", "High", "Epic" })
        {
            TextureQualityDropdown->AddOption(Option);
            ShadowQualityDropdown->AddOption(Option);
            ShaderQualityDropdown->AddOption(Option);
            AntiAliasingDropdown->AddOption(Option);
        }
    }

    LoadCurrentSettings();
}

void USettingsWidget::LoadCurrentSettings()
{
    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    if (!UserSettings) return;

    // Load Fullscreen Mode
    if (FullscreenModeDropdown)
    {
        EWindowMode::Type FullscreenMode = UserSettings->GetFullscreenMode();
        switch (FullscreenMode)
        {
        case EWindowMode::Fullscreen:
            FullscreenModeDropdown->SetSelectedOption("Fullscreen");
            break;
        case EWindowMode::Windowed:
            FullscreenModeDropdown->SetSelectedOption("Windowed");
            break;
        case EWindowMode::WindowedFullscreen:
            FullscreenModeDropdown->SetSelectedOption("Windowed Fullscreen");
            break;
        }
    }

    // Load Resolution
    if (ResolutionDropdown)
    {
        FIntPoint Resolution = UserSettings->GetScreenResolution();
        FString ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
        ResolutionDropdown->SetSelectedOption(ResolutionString);
    }

    // Load Quality Settings
    if (TextureQualityDropdown)
    {
        TextureQualityDropdown->SetSelectedIndex(UserSettings->GetTextureQuality());
    }
    if (ShadowQualityDropdown)
    {
        ShadowQualityDropdown->SetSelectedIndex(UserSettings->GetShadowQuality());
    }
    if (ShaderQualityDropdown)
    {
        ShaderQualityDropdown->SetSelectedIndex(UserSettings->GetPostProcessingQuality());
    }
    if (AntiAliasingDropdown)
    {
        AntiAliasingDropdown->SetSelectedIndex(UserSettings->GetAntiAliasingQuality());
    }

    // Load VSync
    if (VSyncCheckBox)
    {
        VSyncCheckBox->SetIsChecked(UserSettings->IsVSyncEnabled());
    }

    // Load FPS Limit
    if (FPSLimitSlider)
    {
        FPSLimitSlider->SetValue(UserSettings->GetFrameRateLimit());
    }
}

void USettingsWidget::OnApplySettings()
{
    UGameUserSettings* UserSettings = UGameUserSettings::GetGameUserSettings();
    if (!UserSettings) return;

    // Apply Fullscreen Mode
    if (FullscreenModeDropdown)
    {
        FString SelectedOption = FullscreenModeDropdown->GetSelectedOption();
        if (SelectedOption == "Fullscreen")
        {
            UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
        }
        else if (SelectedOption == "Windowed")
        {
            UserSettings->SetFullscreenMode(EWindowMode::Windowed);
        }
        else if (SelectedOption == "Windowed Fullscreen")
        {
            UserSettings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
        }
    }

    // Apply Resolution
    if (ResolutionDropdown)
    {
        FString SelectedResolution = ResolutionDropdown->GetSelectedOption();
        FIntPoint NewResolution;

        if (SelectedResolution == "1280x720")
        {
            NewResolution = FIntPoint(1280, 720);
        }
        else if (SelectedResolution == "1920x1080")
        {
            NewResolution = FIntPoint(1920, 1080);
        }
        else if (SelectedResolution == "2560x1440")
        {
            NewResolution = FIntPoint(2560, 1440);
        }

        UserSettings->SetScreenResolution(NewResolution);
    }

    // Apply Quality Settings
    if (TextureQualityDropdown)
    {
        UserSettings->SetTextureQuality(TextureQualityDropdown->GetSelectedIndex());
    }
    if (ShadowQualityDropdown)
    {
        UserSettings->SetShadowQuality(ShadowQualityDropdown->GetSelectedIndex());
    }
    if (ShaderQualityDropdown)
    {
        UserSettings->SetPostProcessingQuality(ShaderQualityDropdown->GetSelectedIndex());
    }
    if (AntiAliasingDropdown)
    {
        UserSettings->SetAntiAliasingQuality(AntiAliasingDropdown->GetSelectedIndex());
    }

    // Apply V-Sync
    if (VSyncCheckBox)
    {
        UserSettings->SetVSyncEnabled(VSyncCheckBox->IsChecked());
    }

    // Apply FPS Limit
    if (FPSLimitSlider)
    {
        UserSettings->SetFrameRateLimit(FPSLimitSlider->GetValue());
    }

    // Apply and Save Settings
    UserSettings->ApplySettings(true);
    UserSettings->SaveSettings();
}

void USettingsWidget::OnCancelSettings()
{
    LoadCurrentSettings();
}

void USettingsWidget::OnFPSLimitChanged(float Value)
{
    // Optional: Implement logic to update a text label to display FPS value.
}
