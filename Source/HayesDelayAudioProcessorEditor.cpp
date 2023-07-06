#include "HayesDelayAudioProcessor.h"
#include "HayesDelayAudioProcessorEditor.h"


HayesDelayAudioProcessorEditor::HayesDelayAudioProcessorEditor (HayesDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    addAndMakeVisible (mGainSlider);
    addAndMakeVisible (mTimeSlider);
    addAndMakeVisible (mFeedbackSlider);

    setSize (400, 250);
}

void HayesDelayAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::darkmagenta);

    g.setColour (Colours::silver);
    g.setFont   (24.0f);

    auto box = getLocalBounds().reduced (20);
    box = box.withTop (box.getBottom() - 40);

    const auto width = box.getWidth() / 3;
    g.drawFittedText (TRANS ("Dry Gain"), box.removeFromLeft (width), Justification::centred, 1);
    g.drawFittedText (TRANS ("Time"), box.removeFromLeft (width), Justification::centred, 1);
    g.drawFittedText (TRANS ("Feedback"), box.removeFromLeft (width), Justification::centred, 1);
}

void HayesDelayAudioProcessorEditor::resized()
{
    auto box = getLocalBounds().reduced (20);
    box.removeFromBottom (40);

    const auto width = box.getWidth() / 3;
    mGainSlider.setBounds (box.removeFromLeft (width).reduced (10));
    mTimeSlider.setBounds (box.removeFromLeft (width).reduced (10));
    mFeedbackSlider.setBounds (box.removeFromLeft (width).reduced (10));
}
