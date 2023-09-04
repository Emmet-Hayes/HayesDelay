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
    void addAllGUIComponents();

    CustomLookAndFeel customLookAndFeel;
    
    HayesDelayAudioProcessor& processor;
    
    juce::Image image;

    PresetBar presetBar;
    
    juce::Slider gainSlider, timeSlider, feedbackSlider, panningSlider;
    juce::Label gainLabel, timeLabel, feedbackLabel, panningLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment, mTimeAttachment, mFeedbackAttachment, mPanningAttachment;

    int defaultWidth = 400;
    int defaultHeight = 250;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesDelayAudioProcessorEditor)
};
