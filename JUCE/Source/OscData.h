/*
  ==============================================================================

    OscData.h
    Created: 10 Oct 2023 11:59:57am
    Author:  ashay

  ==============================================================================
*/

#pragma once


#include <math.h>
#include <JuceHeader.h>
#include <cmath>

class OscData
{
public:
    OscData() = default;
    ~OscData() = default;
    OscData(const OscData& oscillator) = default;
    void setSampleRate(const double& sampleRate);
    void setWaveType(int waveType);
    void setFrequency(const float& frequency);
    float getSample();
    void stop();
    bool isPlaying();

private:
    float phase = 0;
    float sample = 0;
    float frequency;
    double sampleRate;
    int waveType;
    float mod = 0;
};

