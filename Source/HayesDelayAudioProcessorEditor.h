#pragma once
#include <JuceHeader.h>
#include "HayesDelayAudioProcessor.h"

class HayesDelayAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    HayesDelayAudioProcessorEditor (HayesDelayAudioProcessor&);

    void paint (Graphics&) override;
    void resized() override;

private:
    HayesDelayAudioProcessor& processor;
    
    juce::Image image;
    
    Slider mGainSlider      { Slider::RotaryHorizontalVerticalDrag,  Slider::TextBoxBelow };
    Slider mTimeSlider      { Slider::RotaryHorizontalVerticalDrag,  Slider::TextBoxBelow };
    Slider mFeedbackSlider  { Slider::RotaryHorizontalVerticalDrag,  Slider::TextBoxBelow };

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mTimeAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mFeedbackAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesDelayAudioProcessorEditor)
};
