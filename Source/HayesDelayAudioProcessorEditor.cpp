#include "HayesDelayAudioProcessor.h"
#include "HayesDelayAudioProcessorEditor.h"


HayesDelayAudioProcessorEditor::HayesDelayAudioProcessorEditor (HayesDelayAudioProcessor& p)
:   BaseAudioProcessorEditor { p }
,   processor { p }
,   presetBar { p }
{
    setLookAndFeel(&customLookAndFeel);

    auto setup_labeled_slider = [&](juce::Label* label, juce::Slider* slider, const char* labelText)
    {
        slider->setSliderStyle(juce::Slider::Rotary);
        addAndMakeVisible(slider);
        label->setText(labelText, juce::dontSendNotification);
        label->attachToComponent(slider, false);
        label->setJustificationType(juce::Justification::centred);
        addAndMakeVisible(label);
    };

    setup_labeled_slider(&timeLabel, &timeSlider, "Time");
    setup_labeled_slider(&feedbackLabel, &feedbackSlider, "Feedback");
    setup_labeled_slider(&gainLabel, &gainSlider, "Mix");
    setup_labeled_slider(&panningLabel, &panningSlider, "Pan");
    addAndMakeVisible(presetBar);

    image = juce::ImageCache::getFromMemory(BinaryData::bg_file_jpg, BinaryData::bg_file_jpgSize);

    using Attachment = AudioProcessorValueTreeState::SliderAttachment;
    mTimeAttachment     = std::make_unique<Attachment>(processor.getValueTreeState(), HayesDelayAudioProcessor::paramTime, timeSlider);
    mFeedbackAttachment = std::make_unique<Attachment>(processor.getValueTreeState(), HayesDelayAudioProcessor::paramFeedback, feedbackSlider);
    mGainAttachment     = std::make_unique<Attachment>(processor.getValueTreeState(), HayesDelayAudioProcessor::paramGain, gainSlider);
    mPanningAttachment  = std::make_unique<Attachment>(processor.getValueTreeState(), HayesDelayAudioProcessor::paramPanning, panningSlider);

    const auto ratio = static_cast<double> (defaultWidth) / defaultHeight;
    setResizable(false, true);
    getConstrainer()->setFixedAspectRatio(ratio);
    getConstrainer()->setSizeLimits(defaultWidth, defaultHeight, defaultWidth * 2, defaultHeight * 2);
    setSize(defaultWidth, defaultHeight);
}

HayesDelayAudioProcessorEditor::~HayesDelayAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

void HayesDelayAudioProcessorEditor::paint (Graphics& g)
{
    g.drawImage (image, 0, 0, getWidth(), getHeight(), 0, 0, 800, 500);
}

void HayesDelayAudioProcessorEditor::resized()
{
    const auto scale = static_cast<float> (getWidth()) / defaultWidth;

    auto setBoundsAndApplyScaling = [&](juce::Component* component, int x, int y, int w, int h, bool isSlider = false)
    {
        component->setBounds(static_cast<int>(x * scale), static_cast<int>(y * scale),
            static_cast<int>(w * scale), static_cast<int>(h * scale));
        if (isSlider)
            dynamic_cast<juce::Slider*>(component)->setTextBoxStyle(juce::Slider::TextBoxBelow, false, static_cast<int>(70 * scale), static_cast<int>(20 * scale));
    };

    customLookAndFeel.setWindowScale(scale);
    setBoundsAndApplyScaling(&presetBar, 0, 0, 400, 20);
    setBoundsAndApplyScaling(&timeSlider, 20, 90, 80, 80, true);
    setBoundsAndApplyScaling(&feedbackSlider, 110, 90, 80, 80, true);
    setBoundsAndApplyScaling(&panningSlider, 200, 90, 80, 80, true);
    setBoundsAndApplyScaling(&gainSlider, 290, 90, 80, 80, true);
}
