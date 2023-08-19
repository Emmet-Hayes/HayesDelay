#include "HayesDelayAudioProcessor.h"
#include "HayesDelayAudioProcessorEditor.h"


HayesDelayAudioProcessorEditor::HayesDelayAudioProcessorEditor (HayesDelayAudioProcessor& p)
:   BaseAudioProcessorEditor { p }
,   processor { p }
,   presetBar { p }
{
    mTimeSlider.setLookAndFeel    (&customLookAndFeel);
    mFeedbackSlider.setLookAndFeel(&customLookAndFeel);
    mGainSlider.setLookAndFeel    (&customLookAndFeel);
    mPanningSlider.setLookAndFeel (&customLookAndFeel);
    presetBar.setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(mTimeSlider);
    addAndMakeVisible(mFeedbackSlider);
    addAndMakeVisible(mGainSlider);
    addAndMakeVisible(mPanningSlider);
    addAndMakeVisible(presetBar);

    image = juce::ImageCache::getFromMemory(BinaryData::bg_file_jpg, BinaryData::bg_file_jpgSize);
    setSize(400, 250);

    using Attachment = AudioProcessorValueTreeState::SliderAttachment;
    mTimeAttachment     = std::make_unique<Attachment>(processor.getValueTreeState(), HayesDelayAudioProcessor::paramTime, mTimeSlider);
    mFeedbackAttachment = std::make_unique<Attachment>(processor.getValueTreeState(), HayesDelayAudioProcessor::paramFeedback, mFeedbackSlider);
    mGainAttachment     = std::make_unique<Attachment>(processor.getValueTreeState(), HayesDelayAudioProcessor::paramGain, mGainSlider);
    mPanningAttachment  = std::make_unique<Attachment>(processor.getValueTreeState(), HayesDelayAudioProcessor::paramPanning, mPanningSlider);
}

HayesDelayAudioProcessorEditor::~HayesDelayAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void HayesDelayAudioProcessorEditor::paint (Graphics& g)
{
    g.drawImage (image, 0, 0, getWidth(), getHeight(), 0, 0, 800, 500);

    auto box = getLocalBounds().reduced (20);
    box = box.withTop(box.getBottom() - 40);
    const auto width = box.getWidth() / 4;

    g.setColour(Colours::silver);
    g.setFont(juce::Font("Lucida Console", 16.f, juce::Font::bold));
    g.drawFittedText (TRANS("Time"),     box.removeFromLeft(width), Justification::centred, 1);
    g.drawFittedText (TRANS("Feedback"), box.removeFromLeft(width), Justification::centred, 1);
    g.drawFittedText (TRANS("Pan"),      box.removeFromLeft(width), Justification::centred, 1);
    g.drawFittedText (TRANS("Dry/Wet"),  box.removeFromLeft(width), Justification::centred, 1);    
}

void HayesDelayAudioProcessorEditor::resized()
{
    presetBar.setBounds(0, 0, 400, 20);
    auto box = getLocalBounds().reduced(20);
    box.removeFromTop(20);
    box.removeFromBottom(40);

    const auto width = box.getWidth() / 4;
    mTimeSlider.setBounds     (box.removeFromLeft(width).reduced (10));
    mFeedbackSlider.setBounds (box.removeFromLeft(width).reduced (10));
    mPanningSlider.setBounds  (box.removeFromLeft(width).reduced(10));
    mGainSlider.setBounds     (box.removeFromLeft(width).reduced (10));
}
