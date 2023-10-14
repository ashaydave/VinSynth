/*
  ==============================================================================

    Synth.h
    Created: 1 Oct 2023 11:59:47am
    Author:  ashay

  ==============================================================================
*/

#pragma once

#include "OscData.h"
#include "DelayData.h"
#include "BitcrusherData.h"
#include "FilterData.h"


#define NUMBER_OF_NOTES 127
#define NUMBER_OF_OSCILLATORS 1

class Synth
{
public:
    Synth() = default;
    ~Synth() = default;
    Synth(const Synth& synth) = default;

    void prepareToPlay(const double& sampleRate, int samplesPerBlock)
    {
        this->sampleRate = sampleRate;
        this->samplesPerBlock = samplesPerBlock;
        for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; osc++)
        {
            for (int note = 0; note < NUMBER_OF_NOTES; note++)
            {
                oscillators[osc][note].setSampleRate(sampleRate);
            }
        }

    }

    void setOscillatorTypes(int oscTypes[]) {
        for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; osc++)
            for (int note = 0; note < NUMBER_OF_NOTES; note++)
                oscillators[osc][note].setWaveType(oscTypes[osc]);
    }

    void setEffectParameters(float oscVolumes[NUMBER_OF_OSCILLATORS],  float distortion) //float delayLengthParameter, float dryWetParameter, float feedbackParameter, float bitDepthParameter, float speedParameter,
    {
        this->distortion = distortion;
        //filterData.setParameters();
        for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; osc++)
        {
            this->oscVolumes[osc] = oscVolumes[osc];
        }
    }

    void render(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& messages)
    {
        auto currentSample = 0;
        for (const auto midi : messages)
        {
            auto message = midi.getMessage();
            auto messagePosition = static_cast<int> (message.getTimeStamp());
            process(buffer, currentSample, messagePosition);
            currentSample = messagePosition;
            handleMidiEvent(message);
        }
        process(buffer, currentSample, buffer.getNumSamples());

    }

private:

    float midiToFreq(const int midiNote)
    {
        const float A4_FREQ = 440;
        const float A4_MIDINOTE = 69;
        const float NOTES_IN_OCTAVE = 12.0f;
        return A4_FREQ * std::powf(2, (static_cast<float>(midiNote) - A4_MIDINOTE) / NOTES_IN_OCTAVE);
    }

    void handleMidiEvent(const juce::MidiMessage& message)
    {
        if (message.isNoteOn())
        {
            frequency = midiToFreq(message.getNoteNumber());

            for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; osc++)
            {
                for (int i = 0; i < NUMBER_OF_NOTES; i++)
                {
                    oscillators[osc][i].stop();
                }    
            }               

            for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; osc++)
            {
                oscillators[osc][message.getNoteNumber()].setFrequency(frequency);
            }
                
        }

        if (message.isNoteOff())
        {
            for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; osc++)
                oscillators[osc][message.getNoteNumber()].stop();
        }

        if (message.isAllNotesOff())
        {
            for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; osc++)
                for (int i = 0; i < NUMBER_OF_NOTES; i++)
                    oscillators[osc][message.getNoteNumber()].stop();
        }
    }

    void process(juce::AudioBuffer<float>& buffer, const int startSample, const int endSample)
    {
        auto* firstChannel = buffer.getWritePointer(0);
        for (auto sample = startSample; sample < endSample; sample++)
        {
            for (int osc = 0; osc < NUMBER_OF_OSCILLATORS; osc++)
                for (int note = 0; note < NUMBER_OF_NOTES; note++)
                {
                    if (oscillators[0][note].isPlaying())
                    {
                        for (int channel = 0; channel < buffer.getNumChannels(); channel++)
                        {
                            auto* channelData = buffer.getWritePointer(channel);
                            channelData[sample] += tanh(distortion * oscillators[osc][note].getSample() * oscVolumes[osc]);
                        }
                    }
                }
            firstChannel[sample] = firstChannel[sample];
        }
        

        for (int channel = 1; channel < buffer.getNumChannels(); channel++)
        {
            auto channelData = buffer.getWritePointer(channel);
            std::copy(firstChannel + startSample, firstChannel + endSample, channelData + startSample);
        }

    }


    OscData oscillators[NUMBER_OF_OSCILLATORS][NUMBER_OF_NOTES];
    DelayData delayData;
    BitcrusherData bitcrusherData;
   // FilterData filterData;
    double sampleRate;
    float frequency;
    float distortion;
    float oscVolumes[NUMBER_OF_OSCILLATORS];
    int samplesPerBlock;
};
            