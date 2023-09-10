#pragma once

#include "../../Common/BaseAudioProcessor.h"


class HayesDelayAudioProcessor : public BaseAudioProcessor
                               , public juce::AudioProcessorValueTreeState::Listener
{
public:
    HayesDelayAudioProcessor();

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioSampleBuffer&, MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;

    juce::AudioProcessorValueTreeState& getValueTreeState() { return apvts; }

    static juce::String paramGain;
    static juce::String paramTime;
    static juce::String paramFeedback;
    static juce::String paramPanning;

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

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() override;

    juce::Atomic<float> mGain     {   0.0f };
    juce::Atomic<float> mTime     { 200.0f };
    juce::Atomic<float> mFeedback {  -6.0f };
    juce::Atomic<float> mPanning  { 0.5f };
    juce::Atomic<float> mWobble   { 0.0f };

    float mLastInputGain    { 0.0f };
    float mLastFeedbackGain { 0.0f };

    int    mWritePos        {  0 };
    int    mExpectedReadPos { -1 };
    double mSampleRate      {  0 };

    const double TAIL_LENGTH   { 2.0 };
    
    juce::UndoManager                  mUndoManager;
    juce::AudioSampleBuffer            mDelayBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesDelayAudioProcessor)
};
