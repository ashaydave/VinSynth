/*
  ==============================================================================

    DelayData.cpp
    Created: 1 Oct 2023 12:15:56pm
    Author:  ashay

  ==============================================================================
*/

#include "DelayData.h"
DelayData::DelayData(): delayBuffer(2,1)
{
    delayBuffer.setSize(2, 1);
    delayBuffer.clear();
    delayLength = 0.5f;
    dryMix = 1.0f;
    wetMix = 0.5f;
    feedback = 0.75f;
    delayBufferLength = 1;

    delayReadIndex = 0;
    delayWriteIndex = 0;
    delaySampleRate = 0;
    delaySamplesPerBlock = 0;

    delayLengthForBuffer = delayLength;
    counter = 0;

}

DelayData::~DelayData()
{
}

void DelayData::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    delaySampleRate = sampleRate;
    delaySamplesPerBlock = samplesPerBlock;

    delayBufferLength = (int)(2.0 * sampleRate);
    if (delayBufferLength < 1)
        delayBufferLength = 1;
    delayBuffer.setSize(2, delayBufferLength);
    delayBuffer.clear();

    // Calculate Read Index
    delayReadIndex = (int)(delayWriteIndex - (delayLength * sampleRate) + delayBufferLength) % delayBufferLength;
}


void DelayData::process(juce::AudioBuffer<float>& buffer, float delayLengthParameter, float dryWetParameter, float feedbackParameter)
{
    dryMix = 1.0f - dryWetParameter;
    wetMix = dryWetParameter;
    delayLength = delayLengthParameter;
    feedback = feedbackParameter;

    float initialDelayLength = delayLength;

    int numChannels = buffer.getNumChannels();
    int numSamples = buffer.getNumSamples();
    int channel = 0, newReadIndex, newWriteIndex;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        float* delayData = delayBuffer.getWritePointer(juce::jmin(channel, delayBuffer.getNumChannels() - 1));

        newReadIndex = delayReadIndex;
        newWriteIndex = delayWriteIndex;

        for (int i = 0; i < numSamples; ++i)
        {
            // Calculate Read Index inside buffer
            newReadIndex = (int)(newWriteIndex - (int)(delayLengthForBuffer * delaySampleRate) + delayBufferLength) % delayBufferLength;
            
            if (newReadIndex < 0)
            {
                newReadIndex += delayBufferLength;
            }
            else if (newReadIndex >= delayBufferLength)
            {
                newReadIndex -= delayBufferLength;
            }

            float readIndexFloat = newWriteIndex - (delayLengthForBuffer * delaySampleRate);
            if (readIndexFloat < 0)
            {
                readIndexFloat += (float)delayBufferLength;
            }

            float delayedSample = delayData[newReadIndex]; //interpolated sample

            const float in = channelData[i];
            float out = 0.0;

            // Make a gain factor for Dry Wet Mix
            float gainForDryWet = 1.0f;
            out = (dryMix + (dryMix * gainForDryWet * juce::jmin<float>(dryMix, wetMix))) * in
                + (wetMix + (wetMix * gainForDryWet * juce::jmin<float>(dryMix, wetMix))) * delayedSample;

            if (delayLengthForBuffer != delayLength)
                delayedSample = 0.0f; //

            delayData[newWriteIndex] = in + (delayedSample * feedback);

            // Wrap to zero if buffer length is crossed
            if (++newReadIndex >= delayBufferLength)
                newReadIndex = 0;
            if (++newWriteIndex >= delayBufferLength)
                newWriteIndex = 0;

            channelData[i] = out;
        }
    }

    if (initialDelayLength == delayLength) counter++;
    if (counter == 60)
    {
        counter = 0;
        delayLengthForBuffer = delayLength;
    }

    delayReadIndex = newReadIndex;
    delayWriteIndex = newWriteIndex;
}


void DelayData::reset()
{
    delayBuffer.clear();
}
