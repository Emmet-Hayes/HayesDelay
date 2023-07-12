#pragma once
#include <JuceHeader.h>

class HayesDelayAudioProcessor : public juce::AudioProcessor
                               , public juce::AudioProcessorValueTreeState::Listener
{
public:
    HayesDelayAudioProcessor();

    void releaseResources() override {};
    bool isBusesLayoutSupported(const BusesLayout& /*layouts*/) const override { return true; }
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int /*index*/) override {}
    const juce::String getProgramName(int /*index*/) override { return juce::String(); }
    void changeProgramName(int /*index*/, const juce::String& /*newName*/) override {};

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer&, MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getValueTreeState() { return apvts; }

    static juce::String paramGain;
    static juce::String paramTime;
    static juce::String paramFeedback;

private:
    void writeToDelayBuffer(AudioSampleBuffer& buffer,
        const int channelIn, const int channelOut,
        const int writePos,
        float startGain, float endGain,
        bool replacing);

    void readFromDelayBuffer(AudioSampleBuffer& buffer,
        const int channelIn, const int channelOut,
        const int readPos,
        float startGain, float endGain,
        bool replacing);

    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::Atomic<float> mGain     {   0.0f };
    juce::Atomic<float> mTime     { 200.0f };
    juce::Atomic<float> mFeedback {  -6.0f };

    float mLastInputGain    { 0.0f };
    float mLastFeedbackGain { 0.0f };

    int    mWritePos        {  0 };
    int    mExpectedReadPos { -1 };
    double mSampleRate      {  0 };

    const double TAIL_LENGTH   { 2.0 };
    
    juce::UndoManager                  mUndoManager;
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioSampleBuffer            mDelayBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesDelayAudioProcessor)
};
