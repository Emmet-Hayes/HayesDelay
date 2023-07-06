#include "HayesDelayAudioProcessor.h"
#include "HayesDelayAudioProcessorEditor.h"


String HayesDelayAudioProcessor::paramGain     ("gain");
String HayesDelayAudioProcessor::paramTime     ("time");
String HayesDelayAudioProcessor::paramFeedback ("feedback");


HayesDelayAudioProcessor::HayesDelayAudioProcessor()
  : mState (*this, &mUndoManager, "HayesDelay",
          {
              std::make_unique<AudioParameterFloat>(paramGain,
                                                    TRANS ("Input Gain"),
                                                    NormalisableRange<float>(-100.0f, 6.0f, 0.1f, std::log (0.5f) / std::log (100.0f / 106.0f)),
                                                    mGain.get(), "dB",
                                                    AudioProcessorParameter::genericParameter,
                                                    [](float v, int) { return String (v, 1) + " dB"; },
                                                    [](const String& t) { return t.dropLastCharacters (3).getFloatValue(); }),
              std::make_unique<AudioParameterFloat>(paramTime,
                                                    TRANS ("Delay TIme"),    NormalisableRange<float>(0.0, 2000.0, 1.0),
                                                    mTime.get(), "ms",
                                                    AudioProcessorParameter::genericParameter,
                                                    [](float v, int) { return String (roundToInt (v)) + " ms"; },
                                                    [](const String& t) { return t.dropLastCharacters (3).getFloatValue(); }),
              std::make_unique<AudioParameterFloat>(paramFeedback,
                                                    TRANS ("Feedback Gain"), NormalisableRange<float>(-100.0f, 6.0f, 0.1f, std::log (0.5f) / std::log (100.0f / 106.0f)),
                                                    mFeedback.get(), "dB", AudioProcessorParameter::genericParameter,
                                                    [](float v, int) { return String (v, 1) + " dB"; },
                                                    [](const String& t) { return t.dropLastCharacters (3).getFloatValue(); })
          })
{
    mState.addParameterListener (paramGain, this);
    mState.addParameterListener (paramTime, this);
    mState.addParameterListener (paramFeedback, this);
}

void HayesDelayAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == paramGain)
        mGain = newValue;
    else if (parameterID == paramTime)
        mTime = newValue;
    else if (parameterID == paramFeedback)
        mFeedback = newValue;
}

void HayesDelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    mSampleRate = sampleRate;

    // sample buffer for 2 seconds + 2 buffers safety
    mDelayBuffer.setSize (getTotalNumOutputChannels(), TAIL_LENGTH * (samplesPerBlock + sampleRate), false, false);
    mDelayBuffer.clear();

    mExpectedReadPos = -1;
}

void HayesDelayAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    if (Bus* inputBus = getBus (true, 0))
    {
        const float gain = Decibels::decibelsToGain (mGain.get());
        const float time = mTime.get();
        const float feedback = Decibels::decibelsToGain (mFeedback.get());

        for (int i=0; i < mDelayBuffer.getNumChannels(); ++i) // write original to delay
        {
            const int inputChannelNum = inputBus->getChannelIndexInProcessBlockBuffer (std::min (i, inputBus->getNumberOfChannels()));
            writeToDelayBuffer (buffer, inputChannelNum, i, mWritePos, 1.0f, 1.0f, true);
        }

        
        buffer.applyGainRamp (0, buffer.getNumSamples(), mLastInputGain, gain); // adapt dry gain
        mLastInputGain = gain;

        auto readPos = roundToInt (mWritePos - (mSampleRate * time / 1000.0)); // read delayed signal
        if (readPos < 0)
            readPos += mDelayBuffer.getNumSamples();

        if (Bus* outputBus = getBus (false, 0))
        {
            if (mExpectedReadPos >= 0) // if has run before
            {
                auto endGain = (readPos == mExpectedReadPos) ? 1.0f : 0.0f; // fade out if readPos is off
                for (int i=0; i<outputBus->getNumberOfChannels(); ++i)
                {
                    const int outputChannelNum = outputBus->getChannelIndexInProcessBlockBuffer (i);
                    readFromDelayBuffer (buffer, i, outputChannelNum, mExpectedReadPos, 1.0, endGain, false);
                }
            }

            if (readPos != mExpectedReadPos) // fade in at new position
            {
                for (int i=0; i<outputBus->getNumberOfChannels(); ++i)
                {
                    const int outputChannelNum = outputBus->getChannelIndexInProcessBlockBuffer (i);
                    readFromDelayBuffer (buffer, i, outputChannelNum, readPos, 0.0, 1.0, false);
                }
            }
        }

        for (int i=0; i<inputBus->getNumberOfChannels(); ++i) // add feedback to delay
        {
            const int outputChannelNum = inputBus->getChannelIndexInProcessBlockBuffer (i);
            writeToDelayBuffer (buffer, outputChannelNum, i, mWritePos, mLastFeedbackGain, feedback, false);
        }
        mLastFeedbackGain = feedback;

        mWritePos += buffer.getNumSamples(); // advance positions
        if (mWritePos >= mDelayBuffer.getNumSamples())
            mWritePos -= mDelayBuffer.getNumSamples();

        mExpectedReadPos = readPos + buffer.getNumSamples();
        if (mExpectedReadPos >= mDelayBuffer.getNumSamples())
            mExpectedReadPos -= mDelayBuffer.getNumSamples();
    }
}

void HayesDelayAudioProcessor::writeToDelayBuffer (AudioSampleBuffer& buffer,
                                                  const int channelIn, const int channelOut,
                                                  const int writePos, float startGain, float endGain, bool replacing)
{
    if (writePos + buffer.getNumSamples() <= mDelayBuffer.getNumSamples())
    {
        if (replacing)
            mDelayBuffer.copyFromWithRamp (channelOut, writePos, buffer.getReadPointer (channelIn), buffer.getNumSamples(), startGain, endGain);
        else
            mDelayBuffer.addFromWithRamp (channelOut, writePos, buffer.getReadPointer (channelIn), buffer.getNumSamples(), startGain, endGain);
    }
    else
    {
        const auto midPos  = mDelayBuffer.getNumSamples() - writePos;
        const auto midGain = jmap (float (midPos) / buffer.getNumSamples(), startGain, endGain);
        if (replacing)
        {
            mDelayBuffer.copyFromWithRamp (channelOut, writePos, buffer.getReadPointer (channelIn),         midPos, startGain, midGain);
            mDelayBuffer.copyFromWithRamp (channelOut, 0,        buffer.getReadPointer (channelIn, midPos), buffer.getNumSamples() - midPos, midGain, endGain);
        }
        else
        {
            mDelayBuffer.addFromWithRamp (channelOut, writePos, buffer.getReadPointer (channelIn),         midPos, mLastInputGain, midGain);
            mDelayBuffer.addFromWithRamp (channelOut, 0,        buffer.getReadPointer (channelIn, midPos), buffer.getNumSamples() - midPos, midGain, endGain);
        }
    }
}

void HayesDelayAudioProcessor::readFromDelayBuffer (AudioSampleBuffer& buffer,
                                                   const int channelIn, const int channelOut,
                                                   const int readPos,
                                                   float startGain, float endGain,
                                                   bool replacing)
{
    if (readPos + buffer.getNumSamples() <= mDelayBuffer.getNumSamples())
    {
        if (replacing)
            buffer.copyFromWithRamp (channelOut, 0, mDelayBuffer.getReadPointer (channelIn, readPos), buffer.getNumSamples(), startGain, endGain);
        else
            buffer.addFromWithRamp (channelOut, 0, mDelayBuffer.getReadPointer (channelIn, readPos), buffer.getNumSamples(), startGain, endGain);
    }
    else
    {
        const auto midPos  = mDelayBuffer.getNumSamples() - readPos;
        const auto midGain = jmap (float (midPos) / buffer.getNumSamples(), startGain, endGain);
        if (replacing)
        {
            buffer.copyFromWithRamp (channelOut, 0,      mDelayBuffer.getReadPointer (channelIn, readPos), midPos, startGain, midGain);
            buffer.copyFromWithRamp (channelOut, midPos, mDelayBuffer.getReadPointer (channelIn), buffer.getNumSamples() - midPos, midGain, endGain);
        }
        else
        {
            buffer.addFromWithRamp (channelOut, 0,      mDelayBuffer.getReadPointer (channelIn, readPos), midPos, startGain, midGain);
            buffer.addFromWithRamp (channelOut, midPos, mDelayBuffer.getReadPointer (channelIn), buffer.getNumSamples() - midPos, midGain, endGain);
        }
    }
}

AudioProcessorEditor* HayesDelayAudioProcessor::createEditor()
{
    return new HayesDelayAudioProcessorEditor (*this);
}

void HayesDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    MemoryOutputStream stream(destData, false);
    mState.state.writeToStream (stream);
}

void HayesDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ValueTree tree = ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
        mState.state = tree;
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HayesDelayAudioProcessor();
}
