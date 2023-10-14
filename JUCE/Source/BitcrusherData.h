/*
  ==============================================================================

    BitcrusherData.h
    Created: 1 Oct 2023 12:00:31pm
    Author:  ashay

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <math.h>

class BitcrusherData
{
   public:
       BitcrusherData();
       ~BitcrusherData();
       //void prepareToPlay(double sampleRate);
       void process(juce::AudioBuffer<float>& buffer, float bitDepthParameter, float speedParameter);
       void reset();

   private:
       float bitDepth, speed;
       float step, phasor, quantizedValue;
       //double sampleRate;
};