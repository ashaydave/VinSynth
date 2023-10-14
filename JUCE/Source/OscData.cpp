/*
  ==============================================================================

    OscData.h
    Created: 1 Oct 2023 11:59:57am
    Author:  ashay

  ==============================================================================
*/

#pragma once
#include "OscData.h"


void OscData::setSampleRate(const double& sampleRate)
{
    this->sampleRate = sampleRate;
    stop();
}

void OscData::setWaveType(int waveType)
{
    this->waveType = waveType;
}

void OscData::setFrequency(const float& frequency)
{
    this->frequency = frequency;
}


float OscData::getSample()
{
    // Sine
    if (waveType == 0)
    {
        if (phase >= 1.0)
            phase = phase - 1.0;
        sample = sin(2 * juce::MathConstants<float>::pi * phase);
        phase += (frequency) / sampleRate;
        return sample;
    }

    // Square
    else if (waveType == 1)
    {
        if (phase >= 1.0)
            phase = phase - 1.0;
        sample = signbit(sin(2 * juce::MathConstants<float>::pi * phase));
        phase += (frequency) / sampleRate;
        return sample;
    }

    // Triangle
    else if (waveType == 2)
    {
        if (phase >= 1.0)
            phase = phase - 1.0;
        sample = (2/juce::MathConstants<float>::pi) * asin(sin(2 * juce::MathConstants<float>::pi * phase));
        phase += (frequency) / sampleRate;
        return sample;
    }

    // Sawtooth
    else if (waveType == 3)
    {
        if (phase >= 1.0)
            phase = phase - 1.0;
        sample = 2.0 * phase - 1.0;
        phase += (frequency) / sampleRate;
        return sample;
    }

}


void OscData::stop()
{
    frequency = 0;
    phase = 0;
}

bool OscData::isPlaying()
{
    return frequency != 0;
}


