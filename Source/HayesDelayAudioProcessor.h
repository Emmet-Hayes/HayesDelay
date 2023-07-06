#pragma once
#include <JuceHeader.h>

class HayesDelayAudioProcessor  :  public AudioProcessor,
                                    public AudioProcessorValueTreeState::Listener
{
public:
    HayesDelayAudioProcessor();

    void releaseResources() override {};
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override { return true; }
    bool hasEditor() const override { return true; }
    const String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const String getProgramName(int index) override { return String(); }
    void changeProgramName(int index, const String& newName) override {};

    void parameterChanged(const String& parameterID, float newValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(AudioSampleBuffer&, MidiBuffer&) override;
    AudioProcessorEditor* createEditor() override;
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    AudioProcessorValueTreeState& getValueTreeState() { return mState; }

    static String paramGain;
    static String paramTime;
    static String paramFeedback;

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

    Atomic<float>   mGain     {   0.0f };
    Atomic<float>   mTime     { 200.0f };
    Atomic<float>   mFeedback {  -6.0f };

    UndoManager                  mUndoManager;
    AudioProcessorValueTreeState mState;

    AudioSampleBuffer            mDelayBuffer;

    float mLastInputGain    = 0.0f;
    float mLastFeedbackGain = 0.0f;

    int    mWritePos        = 0;
    int    mExpectedReadPos = -1;
    double mSampleRate      = 0;

    const int TAIL_LENGTH = 2.0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HayesDelayAudioProcessor)
};
