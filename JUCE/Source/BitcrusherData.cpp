/*
  ==============================================================================

    BitcrusherData.cpp
    Created: 1 Oct 2023 12:16:08pm
    Author:  ashay

  ==============================================================================
*/

#include "BitcrusherData.h"
BitcrusherData::BitcrusherData()
{
    bitDepth = 12;
    speed = 0;
    step = 0;
    phasor = 0;
    quantizedValue = 0;
}

BitcrusherData::~BitcrusherData()
{
}



void BitcrusherData::process(juce::AudioBuffer<float>& buffer, float bitDepthParameter, float speedParameter)
{
    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();
    step = 1 / std::pow(2, bitDepthParameter);
    phasor = 0;

    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; sample++)
        {
            phasor = phasor + speedParameter;
            if (phasor >= 1.0f)
            {
                phasor -= 1.0f;
                quantizedValue = step * std::floor(*channelData / step + 0.5f);
            }
            *channelData = quantizedValue;
            channelData++;
        }
    }
}

void BitcrusherData::reset()
{ 
    
}