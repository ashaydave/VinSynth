/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class VinSynthAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener
{
public:
    VinSynthAudioProcessorEditor (VinSynthAudioProcessor&);
    ~VinSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;

    // Oscillator
    juce::Slider oscVolumeSlider[1];
    juce::Label oscVolumeLabel[1];

    juce::ComboBox oscTypeDropdown[3];

    juce::Slider distortionSlider;
    juce::Label distortionLabel;

    // Delay
    juce::Slider delayLengthSlider;
    juce::Label delayLengthLabel;

    juce::Slider dryWetSlider;
    juce::Label dryWetLabel;

    juce::Slider feedbackSlider;
    juce::Label feedbackLabel;

    // Bitcrusher
    juce::Slider bitDepthSlider;
    juce::Label bitDepthLabel;

    juce::Slider speedSlider;
    juce::Label speedLabel;

    // Filter
    juce::ComboBox filterDropdown;

    juce::Slider cutoffFreqSlider;
    juce::Label cutoffFreqLabel;

    juce::Slider resonanceSlider;
    juce::Label resonanceLabel;

    juce::Slider gainSlider;
    juce::Label gainLabel;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> oscVolumeValue[1];
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscTypeValue[4];
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> distortionValue;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> delayLengthValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> dryWetValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> feedbackValue;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> bitDepthValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> speedValue;

    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterTypeValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> cutoffFreqValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> resonanceValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> gainValue;

    void oscTypeChanged();

    juce::ImageComponent imageComponent;
    juce::ImageComponent johnTravolta;
    juce::ImageComponent bruceWillis;
    juce::ImageComponent umaThurman;
    juce::ImageComponent pulpFiction;
    juce::ImageComponent tarantino;
    juce::ImageComponent logo;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VinSynthAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VinSynthAudioProcessorEditor)
};
