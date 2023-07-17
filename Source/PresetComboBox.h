#pragma once
#include <JuceHeader.h>
#include "HayesDelayAudioProcessor.h"

class PresetBar;

class PresetComboBox : public juce::ComboBox
{
public:
    PresetComboBox(HayesDelayAudioProcessor& p, PresetBar* bar);
    void mouseDown(const juce::MouseEvent& event) override;
    void savePreset();
    void loadPreset();
    void deletePreset();

    HayesDelayAudioProcessor& processor;
    PresetBar* presetBar;
    juce::PopupMenu saveLoadMenu;
};
