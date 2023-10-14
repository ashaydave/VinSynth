/*
  ==============================================================================

    FilterData.cpp
    Created: 1 Oct 2023 12:16:13pm
    Author:  ashay

  ==============================================================================
*/

#include "FilterData.h"
#include <juce_core/juce_core.h>



FilterData::FilterData()
{
    cutoffFreq = 15000;
    resonance = 1;
    gain = 0;
    filterType = 0;
    bufferX1 = 0;
    bufferX2 = 0;
    bufferY1 = 0;
    bufferY2 = 0;
    a0 = 0;
    a1 = 0;
    a2 = 0;
    b0 = 0;
    b1 = 0;
    b2 = 0;
}

FilterData::~FilterData()
{
}

void FilterData::reset()
{
    bufferX1 = 0;
    bufferX2 = 0;
    bufferY1 = 0;
    bufferY2 = 0;
}

void FilterData::prepareToPlay(double sampleRate)
{
}


void FilterData::calculateCoefficients(double sampleRate, float cutoffFreq, float resonance, float gain, int filterType)
{
    float w0 = 2.0 * juce::MathConstants<double>::pi * cutoffFreq / sampleRate;
    float alpha = std::sin(w0) / (2.0 * resonance);
    float c = std::cos(w0);

    if (filterType == 0) // Lowpass
    {
        a0 = (1.0 - c) / 2.0;
        a1 = 1.0 - c;
        a2 = (1.0 - c) / 2.0;
        b0 = 1.0 + alpha;
        b1 = -2.0 * c;
        b2 = 1.0 - alpha;
    }

    else if (filterType == 1) // Highpass
    {
        a0 = (1.0 + c) / 2.0;
        a1 = -(1.0 + c);
        a2 = (1.0 + c) / 2.0;
        b0 = 1.0 + alpha;
        b1 = -2.0 * c;
        b2 = 1.0 - alpha;
    }
}

void FilterData::process(juce::AudioBuffer<float>& buffer)
{
    /*this->gain = gain;*/
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        int numSamples = buffer.getNumSamples();

        for (int n = 0; n < numSamples; ++n)
        {
            float x0 = channelData[n];
            float y0 = a0 * x0 + a1 * bufferX1 + a2 * bufferX2 - b1 * bufferY1 - b2 * bufferY2;
           /* juce::Logger::writeToLog("a0 " + juce::String(a0));
            juce::Logger::writeToLog("a1 " + juce::String(a1));
            juce::Logger::writeToLog("a2 " + juce::String(a2));
            juce::Logger::writeToLog("b1 " + juce::String(b1));
            juce::Logger::writeToLog("b2 " + juce::String(b2));*/

            bufferX2 = bufferX1;
            bufferX1 = x0;
            bufferY2 = bufferY1;
            bufferY1 = y0;

            //juce::Logger::writeToLog("bufferx2 " + juce::String(bufferX2));
            //juce::Logger::writeToLog("bufferx1 " + juce::String(bufferX1));
            //juce::Logger::writeToLog("bufferY2 " + juce::String(bufferY2));
            //juce::Logger::writeToLog("bufferY1 " + juce::String(bufferY1));

            y0 = std::pow(10.0, (gain / 20.0)) * y0;
            channelData[n] = y0;
        }
    }
}
