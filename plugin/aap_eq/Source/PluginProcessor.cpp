/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Aap_eqAudioProcessor::Aap_eqAudioProcessor()
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}


Aap_eqAudioProcessor::~Aap_eqAudioProcessor() {}

//==============================================================================
// PARAMETER LAYOUT
juce::AudioProcessorValueTreeState::ParameterLayout
Aap_eqAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "BASS", 1 },
        "Bass",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.01f),
        0.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "MID", 1 },
        "Mid",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.01f),
        0.0f
    ));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "TREBLE", 1 },
        "Treble",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.01f),
        0.0f
    ));

    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String Aap_eqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Aap_eqAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Aap_eqAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Aap_eqAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double Aap_eqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
int Aap_eqAudioProcessor::getNumPrograms() { return 1; }
int Aap_eqAudioProcessor::getCurrentProgram() { return 0; }
void Aap_eqAudioProcessor::setCurrentProgram(int) {}
const juce::String Aap_eqAudioProcessor::getProgramName(int) { return {}; }
void Aap_eqAudioProcessor::changeProgramName(int, const juce::String&) {}

//==============================================================================
void Aap_eqAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void Aap_eqAudioProcessor::releaseResources() {}

//==============================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
bool Aap_eqAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

//==============================================================================
void Aap_eqAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // ====== PARAMETER READ (FOR NEXT STEP) ======
    auto bass = apvts.getRawParameterValue("BASS")->load();
    auto mid = apvts.getRawParameterValue("MID")->load();
    auto treble = apvts.getRawParameterValue("TREBLE")->load();

    juce::ignoreUnused(bass, mid, treble);
}

//==============================================================================
bool Aap_eqAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* Aap_eqAudioProcessor::createEditor()
{
    return new Aap_eqAudioProcessorEditor(*this);
}

//==============================================================================
void Aap_eqAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Aap_eqAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Aap_eqAudioProcessor();
}
