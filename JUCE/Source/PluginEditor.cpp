/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <BinaryData.h>
#include <BinaryData.cpp>

//==============================================================================
VinSynthAudioProcessorEditor::VinSynthAudioProcessorEditor (VinSynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(700, 500);

    // Oscillators
    for (int osc = 0; osc < 1; osc++)
    {
        addAndMakeVisible(&oscVolumeSlider[osc]);
        oscVolumeSlider[osc].setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        oscVolumeSlider[osc].setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
        oscVolumeSlider[osc].addListener(this);

        oscVolumeValue[osc] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, ("oscillatorVolume" + std::to_string(osc)), oscVolumeSlider[osc]);
        oscVolumeLabel[osc].attachToComponent(&oscVolumeSlider[osc], false);
        oscVolumeLabel[osc].setText("Volume", juce::dontSendNotification);
        oscVolumeLabel[osc].setJustificationType(juce::Justification::centred);

        addAndMakeVisible(oscTypeDropdown[osc]);
        oscTypeDropdown[osc].addItem("Sine", 1);
        oscTypeDropdown[osc].addItem("Square", 2);
        oscTypeDropdown[osc].addItem("Triangle", 3);
        oscTypeDropdown[osc].addItem("Saw", 4);

        oscTypeValue[osc] = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, ("oscillatorType" + std::to_string(osc)), oscTypeDropdown[osc]);
    }

    // Distortion slider
    addAndMakeVisible(&distortionSlider);
    distortionSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    distortionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    distortionSlider.addListener(this);
    distortionValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "distortion", distortionSlider);

    distortionLabel.attachToComponent(&distortionSlider, false);
    distortionLabel.setText("Distortion", juce::dontSendNotification);
    distortionLabel.setJustificationType(juce::Justification::centred);

    // Delay 
    addAndMakeVisible(&delayLengthSlider);
    delayLengthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    delayLengthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    delayLengthSlider.addListener(this);
    delayLengthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "delayLength", delayLengthSlider);

    delayLengthLabel.attachToComponent(&delayLengthSlider, false);
    delayLengthLabel.setText("Delay", juce::dontSendNotification);
    delayLengthLabel.setJustificationType(juce::Justification::centred);

    // Dry West
    addAndMakeVisible(&dryWetSlider);
    dryWetSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    dryWetSlider.addListener(this);
    dryWetValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "dryWet", dryWetSlider);

    dryWetLabel.attachToComponent(&dryWetSlider, false);
    dryWetLabel.setText("Mix", juce::dontSendNotification);
    dryWetLabel.setJustificationType(juce::Justification::centred);

    // Feedback
    addAndMakeVisible(&feedbackSlider);
    feedbackSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    feedbackSlider.addListener(this);
    feedbackValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "feedback", feedbackSlider);

    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.setJustificationType(juce::Justification::centred);

    // Bitcrusher
    addAndMakeVisible(&bitDepthSlider);
    bitDepthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bitDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    bitDepthSlider.addListener(this);
    bitDepthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "bitDepth", bitDepthSlider);

    bitDepthLabel.attachToComponent(&bitDepthSlider, false);
    bitDepthLabel.setText("Bit Depth", juce::dontSendNotification);
    bitDepthLabel.setJustificationType(juce::Justification::centred);

    // Speed
    addAndMakeVisible(&speedSlider);
    speedSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    speedSlider.addListener(this);
    speedValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "speed", speedSlider);

    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);

    // Filter
    addAndMakeVisible(filterDropdown);
    filterDropdown.addItem("Lowpass", 1);
    filterDropdown.addItem("Highpass", 2);

    filterTypeValue = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.treeState, "filterType", filterDropdown);

    // Cutoff Freq
    addAndMakeVisible(&cutoffFreqSlider);
    cutoffFreqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    cutoffFreqSlider.addListener(this);
    cutoffFreqValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "cutoffFreq", cutoffFreqSlider);

    cutoffFreqLabel.attachToComponent(&cutoffFreqSlider, false);
    cutoffFreqLabel.setText("Cutoff Freq", juce::dontSendNotification);
    cutoffFreqLabel.setJustificationType(juce::Justification::centred);

    // Resonance
    addAndMakeVisible(&resonanceSlider);
    resonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    resonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    resonanceSlider.addListener(this);
    resonanceValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "resonance", resonanceSlider);

    resonanceLabel.attachToComponent(&resonanceSlider, false);
    resonanceLabel.setText("Resonance", juce::dontSendNotification);
    resonanceLabel.setJustificationType(juce::Justification::centred);

    // Gain
    //addAndMakeVisible(&gainSlider);
    //gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    //gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
    //gainSlider.addListener(this);
    //gainValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, "gain", gainSlider);

    //gainLabel.attachToComponent(&gainSlider, false);
    //gainLabel.setText("Gain", juce::dontSendNotification);
    //gainLabel.setJustificationType(juce::Justification::centred);

    // Colour Schemes
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::red);
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::whitesmoke);
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::yellow);
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::antiquewhite);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::red);

}

VinSynthAudioProcessorEditor::~VinSynthAudioProcessorEditor()
{
}

//==============================================================================
void VinSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    //auto indianredHue = juce::Colours::indianred.getHue();
    auto whiteHue = juce::Colours::floralwhite.getHue();
    auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::backgroundNew_png, BinaryData::backgroundNew_pngSize);
    g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, 1234, 597);


    auto x = getWidth() / 15;
    auto y = getHeight() / 20;
    juce::Rectangle<int> oscillatorRectangle(x, y, 5 * x, 7 * y);

    g.setColour(juce::Colour::fromHSV(whiteHue, 0.5f, 1.0f, 1.0f));
    g.drawRoundedRectangle(oscillatorRectangle.toFloat(), 5, 10);
    auto mia = juce::ImageCache::getFromMemory(BinaryData::miaWallace_png, BinaryData::miaWallace_pngSize);
    g.drawImage(mia, x, y, 5 * x, 7 * y, 0, 0, 1234, 597);


    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawFittedText("Wallacilator", x - 55, oscillatorRectangle.getY() * 2, oscillatorRectangle.getWidth(), 1, juce::Justification::centred, 1);


    // Delay Rectangle
    juce::Rectangle<int> delayRectangle(x, 10 * y, 5 * x, 8 * y);

    g.setColour(juce::Colour::fromHSV(whiteHue, 0.5f, 1.0f, 1.0f));
    g.drawRoundedRectangle(delayRectangle.toFloat(), 5, 10);
    auto tarantino = juce::ImageCache::getFromMemory(BinaryData::tarantino_png, BinaryData::tarantino_pngSize);
    g.drawImage(tarantino, x, 10 * y, 5 * x, 8 * y, 0, 0, 1234, 597);

    g.setColour(juce::Colours::white);
    g.setFont(22.0f);
    g.drawFittedText("TaranDelay", x - 50, delayRectangle.getY() + 18, delayRectangle.getWidth(), 1, juce::Justification::centred, 1);

    // Bitcrusher Rectangle
    juce::Rectangle<int> bitcrusherRectangle(7 * x, y, 5 * x, 7 * y);

    g.setColour(juce::Colour::fromHSV(whiteHue, 0.5f, 1.0f, 1.0f));
    g.drawRoundedRectangle(bitcrusherRectangle.toFloat(), 5, 10);
    auto butch = juce::ImageCache::getFromMemory(BinaryData::butch_png, BinaryData::butch_pngSize);
    g.drawImage(butch, 7 * x, y, 5 * x, 7 * y, 0, 0, 1234, 597);

    g.setColour(juce::Colours::white);
    g.setFont(22.0f);
    g.drawFittedText("ButchCrusher", x + 235, bitcrusherRectangle.getY() + 18, bitcrusherRectangle.getWidth(), 1, juce::Justification::centred, 1);

    // Filter Rectangle
    juce::Rectangle<int> filterRectangle(7 * x, 10 * y, 5 * x, 7 * y);

    g.setColour(juce::Colour::fromHSV(whiteHue, 0.5f, 1.0f, 1.0f));
    g.drawRoundedRectangle(filterRectangle.toFloat(), 5, 10);
    auto pulpFilter = juce::ImageCache::getFromMemory(BinaryData::pulpFilter_png, BinaryData::pulpFilter_pngSize);
    g.drawImage(pulpFilter, 7 * x, 10 * y, 5 * x, 7 * y, 0, 0, 1234, 597);

    g.setColour(juce::Colours::white);
    g.setFont(22.0f);
    g.drawFittedText("Pulp Filter", x + 220, filterRectangle.getY() + 20, filterRectangle.getWidth(), 1, juce::Justification::centred, 1);

}

void VinSynthAudioProcessorEditor::resized()
{
    // Dividing the GUI window into a grid of 10x10
    auto x = getWidth() / 15;
    auto y = getHeight() / 20;

    oscTypeDropdown[0].setBounds(3.75 * x, 1.5 * y, 2 * x, y);
    oscVolumeSlider[0].setBounds(x, 4 * y, 2 * x, 4 * y);
    distortionSlider.setBounds(4 * x, 4 * y, 2 * x, 4 * y);

    // Delay
    delayLengthSlider.setBounds(x, 13 * y, 2 * x, 3 * y);
    dryWetSlider.setBounds(2.5 * x, 15 * y, 2 * x, 3 * y); 
    feedbackSlider.setBounds(4 * x, 13 * y, 2 * x, 3 * y);

    // Bitcrusher
    bitDepthSlider.setBounds(7 * x, 4 * y, 2 * x, 4 * y);
    speedSlider.setBounds(10 * x, 4 * y, 2 * x, 4 * y);

    // Filter
    filterDropdown.setBounds(9.75 * x, 10.5 * y, 2 * x, y);
    cutoffFreqSlider.setBounds(7 * x, 13 * y, 2 * x, 3 * y);
    resonanceSlider.setBounds(10 * x, 13 * y, 2 * x, 3 * y);
    gainSlider.setBounds(8.5 * x, 13 * y, 2 * x, 3 * y);
}

void VinSynthAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
}


