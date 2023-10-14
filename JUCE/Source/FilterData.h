/*
  ==============================================================================

    FilterData.h
    Created: 1 Oct 2023 12:00:12pm
    Author:  ashay

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <math.h>


class FilterData
{
public:
    FilterData();
    ~FilterData();
    void reset();
    void prepareToPlay(double sampleRate);
    void calculateCoefficients(double sampleRate, float cutoffFreq, float resonance, float gain, int filterType);
    void process(juce::AudioBuffer<float>& buffer);

private:
    double sampleRate;
    float cutoffFreq, resonance, gain;
    int filterType;
    float bufferX1;
    float bufferX2;
    float bufferY1;
    float bufferY2;
    float a0, a1, a2, b0, b1, b2;
};
