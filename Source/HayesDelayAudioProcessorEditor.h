#pragma once
#include <JuceHeader.h>
#include "HayesDelayAudioProcessor.h"
#include "../../Common/CustomLookAndFeel.h"
#include "../../Common/PresetBar.h"
#include "../../Common/BaseAudioProcessorEditor.h"

class HayesDelayAudioProcessorEditor : public BaseAudioProcessorEditor
{
public:
    HayesDelayAudioProcessorEditor(HayesDelayAudioProcessor&);
    ~HayesDelayAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    CustomLookAndFeel customLookAndFeel;
    
    HayesDelayAudioProcessor& processor;
    
    juce::Image image;

    PresetBar presetBar;
    
    Slider mGainSlider      { Slider::RotaryHorizontalVerticalDrag,  Slider::TextBoxBelow };
    Slider mTimeSlider      { Slider::RotaryHorizontalVerticalDrag,  Slider::TextBoxBelow };
    Slider mFeedbackSlider  { Slider::RotaryHorizontalVerticalDrag,  Slider::TextBoxBelow };
    Slider mPanningSlider   { Slider::RotaryHorizontalVerticalDrag,  Slider::TextBoxBelow };

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mTimeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mFeedbackAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mPanningAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesDelayAudioProcessorEditor)
};
