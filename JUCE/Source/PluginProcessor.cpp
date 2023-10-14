/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VinSynthAudioProcessor::VinSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    treeState(*this, nullptr, "PARAMETER", createParameterLayout())

#endif
{
}

VinSynthAudioProcessor::~VinSynthAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout VinSynthAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;

    auto normRangeModFreq = juce::NormalisableRange<float>(0, 10);
    normRangeModFreq.setSkewForCentre(3);

    auto normRangeModDepth = juce::NormalisableRange<float>(0, 1);
    normRangeModDepth.setSkewForCentre(.1);

    auto normRangeVolume = juce::NormalisableRange<float>(-120, 0);
    normRangeVolume.setSkewForCentre(-18);

    auto normRangedistortion = juce::NormalisableRange<float>(1, 10);

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "distortion",P_ID_NUM }, "distortion", normRangedistortion, 1));

    juce::StringArray oscTypeList = { "Sine","Square","Triangle","Saw", };
    juce::StringArray filterTypeList = { "Lowpass","Highpass" };
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "oscillatorVolume0",P_ID_NUM }, "oscillatorVolume0", normRangeVolume, -20));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "oscillatorType0",P_ID_NUM }, "oscillatorType0", oscTypeList, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "oscillatorVolume1",P_ID_NUM }, "oscillatorVolume1", normRangeVolume, -10));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "oscillatorType1",P_ID_NUM }, "oscillatorType1", oscTypeList, 1));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "oscillatorVolume2",P_ID_NUM }, "oscillatorVolume2", normRangeVolume, -8));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "oscillatorType2",P_ID_NUM }, "oscillatorType2", oscTypeList, 2));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "oscillatorVolume3",P_ID_NUM }, "oscillatorVolume3", normRangeVolume, -5));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "oscillatorType3",P_ID_NUM }, "oscillatorType3", oscTypeList, 3));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "delayLength",P_ID_NUM }, "delayLength", 0, 2, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "dryWet",P_ID_NUM }, "dryWet", 0, 1, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "feedback",P_ID_NUM }, "feedback", 0, 1, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "bitDepth",P_ID_NUM }, "bitDepth", 2, 12, 12));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "speed",P_ID_NUM }, "speed", 0.1, 1, 1));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{ "filterType",P_ID_NUM }, "filterType", filterTypeList, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "cutoffFreq",P_ID_NUM }, "cutoffFreq", 1000, 18000, 18000));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "resonance",P_ID_NUM }, "resonance", 1, 5, 1));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ "gain",P_ID_NUM }, "gain", 0, 10, 0));

    return { params.begin(), params.end() };
}
//==============================================================================
const juce::String VinSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VinSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VinSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VinSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VinSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VinSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VinSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VinSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VinSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void VinSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VinSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.prepareToPlay(sampleRate, samplesPerBlock);
    delayData.prepareToPlay(sampleRate, samplesPerBlock);
    filterData.prepareToPlay(sampleRate);
}

void VinSynthAudioProcessor::releaseResources()
{
    delayData.reset();
    filterData.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VinSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VinSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();



    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int osc = 0; osc < 1; osc++)
    {
        oscillatorVolumes[osc] = dbToLinear(treeState.getRawParameterValue("oscillatorVolume" + std::to_string(osc))->load());
        oscillatorTypes[osc] = treeState.getRawParameterValue("oscillatorType" + std::to_string(osc))->load();        
    }

    synth.setOscillatorTypes(oscillatorTypes);

    synth.setEffectParameters(
                              oscillatorVolumes,
                              treeState.getRawParameterValue("distortion")->load()
                              );


    synth.render(buffer, midiMessages);

    // Process the delay gawdammit
    delayData.process(buffer, treeState.getRawParameterValue("delayLength")->load(),
        treeState.getRawParameterValue("dryWet")->load(),
        treeState.getRawParameterValue("feedback")->load());

    // Process the bitcrusher gawdammit
    bitcrusherData.process(buffer, treeState.getRawParameterValue("bitDepth")->load(),
        treeState.getRawParameterValue("speed")->load());
    
    // Process the filter pls I love you
    filterData.calculateCoefficients(getSampleRate(), treeState.getRawParameterValue("cutoffFreq")->load(),
        treeState.getRawParameterValue("resonance")->load(),
        treeState.getRawParameterValue("gain")->load(), treeState.getRawParameterValue("filterType")->load());

    filterData.process(buffer);


}

//==============================================================================
bool VinSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VinSynthAudioProcessor::createEditor()
{
    return new VinSynthAudioProcessorEditor (*this);
}

//==============================================================================
void VinSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void VinSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VinSynthAudioProcessor();
}
