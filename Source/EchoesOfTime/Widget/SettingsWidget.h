#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

class UComboBoxString;
class UCheckBox;
class USlider;
class UButton;
class UEditableTextBox;

UCLASS()
class ECHOESOFTIME_API USettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

private:
    // UI Elements
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* FullscreenModeDropdown;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionDropdown;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* TextureQualityDropdown;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ShadowQualityDropdown;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ShaderQualityDropdown;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* AntiAliasingDropdown;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* VSyncCheckBox;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* FPSLimitTextBox;

    UPROPERTY(meta = (BindWidget))
    UButton* ApplyButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CancelButton;

    // Functions
    UFUNCTION()
    void OnApplySettings();

    UFUNCTION()
    void OnCancelSettings();

    UFUNCTION()
    void OnFPSLimitTextChanged(const FText& Text);

    void LoadCurrentSettings();
};
