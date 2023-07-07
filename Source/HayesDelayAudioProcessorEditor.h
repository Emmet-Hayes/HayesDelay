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

    AudioProcessorValueTreeState::SliderAttachment mGainAttachment      { processor.getValueTreeState(), HayesDelayAudioProcessor::paramGain,     mGainSlider };
    AudioProcessorValueTreeState::SliderAttachment mTimeAttachment      { processor.getValueTreeState(), HayesDelayAudioProcessor::paramTime,     mTimeSlider };
    AudioProcessorValueTreeState::SliderAttachment mFeedbackAttachment  { processor.getValueTreeState(), HayesDelayAudioProcessor::paramFeedback, mFeedbackSlider };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesDelayAudioProcessorEditor)
};
