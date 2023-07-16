#pragma once
#include <JuceHeader.h>
#include "HayesDelayAudioProcessor.h"
#include "CustomLookAndFeel.h"

class HayesDelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    HayesDelayAudioProcessorEditor (HayesDelayAudioProcessor&);

    void paint (Graphics&) override;
    void resized() override;

private:
    HayesDelayAudioProcessor& processor;
    
    CustomLookAndFeel customLookAndFeel;
    
    juce::Image image;
    
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
