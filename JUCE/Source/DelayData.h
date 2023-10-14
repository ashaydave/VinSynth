/*
  ==============================================================================

    DelayData.h
    Created: 1 Oct 2023 12:00:06pm
    Author:  ashay

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <math.h>

class DelayData
{
public:
    DelayData();
    ~DelayData();
    void prepareToPlay(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer, float delayLengthParameter, float dryWetParameter, float feedbackParameter);
    void reset();

private:
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferLength, delayReadIndex, delayWriteIndex, counter;
    float delayLength, dryMix, wetMix, feedback;
    float delayLengthForBuffer;
    double delaySampleRate;
    int delaySamplesPerBlock;
};