/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BitCrusher01AudioProcessor::BitCrusher01AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#else
#endif

parameters(*this, nullptr, juce::Identifier("BitcrusherPlugin"),
    {
        std::make_unique<juce::AudioParameterFloat>("bit_depth", "Bit Depth", 1.0f, 16.0f, 16.0f),
        std::make_unique<juce::AudioParameterFloat>("sample_rate_reduction", "Sample Rate Reduction", 1.0f, 100.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("cutoff_frequency", "Cutoff Frequency", juce::NormalisableRange{20.0f, 20000.0f, 0.1f, 0.2f, false}, 500.0f),
        std::make_unique<juce::AudioParameterBool>("highpass", "Highpass", false),
        std::make_unique<juce::AudioParameterBool>("should_filter", "Should Filter", false)
    })

{
    bitDepthParam = parameters.getRawParameterValue("bit_depth");
    sampleRateReductionParam = parameters.getRawParameterValue("sample_rate_reduction");

    mixParam = parameters.getRawParameterValue("mix");

    cutoffFrequencyParam = parameters.getRawParameterValue("cutoff_frequency");
    highpassParam = parameters.getRawParameterValue("highpass");
    shouldFilterParam = parameters.getRawParameterValue("should_filter");
}

BitCrusher01AudioProcessor::~BitCrusher01AudioProcessor()
{
}

//==============================================================================
const juce::String BitCrusher01AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BitCrusher01AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BitCrusher01AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BitCrusher01AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BitCrusher01AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BitCrusher01AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BitCrusher01AudioProcessor::getCurrentProgram()
{
    return 0;
}

void BitCrusher01AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BitCrusher01AudioProcessor::getProgramName (int index)
{
    return {};
}

void BitCrusher01AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BitCrusher01AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    bitcrusher.setSamplingRate(static_cast<float>(sampleRate));
}

void BitCrusher01AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BitCrusher01AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BitCrusher01AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    const auto bitDepth = bitDepthParam->load();
    const auto sampleReduction = sampleRateReductionParam->load();

    const auto mix = mixParam->load();

    const auto cutoffFrequency = cutoffFrequencyParam->load();
    const auto highpass = *highpassParam < 0.5f ? false : true;
    const auto shouldFilter = *shouldFilterParam < 0.5f ? false : true;

    bitcrusher.setBitDepth(bitDepth);
    bitcrusher.setSampleRateReduction(sampleReduction);

    bitcrusher.setMix(mix);

    bitcrusher.isFiltered(shouldFilter);
    bitcrusher.setHighpass(highpass);
    bitcrusher.setCutoffFrequency(cutoffFrequency);

    bitcrusher.processBlock(buffer, midiMessages);
}

//==============================================================================
bool BitCrusher01AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BitCrusher01AudioProcessor::createEditor()
{
    return new BitCrusher01AudioProcessorEditor (*this, parameters);
}

//==============================================================================
void BitCrusher01AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream mos(destData, true);
    parameters.state.writeToStream(mos);
}

void BitCrusher01AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        parameters.replaceState(tree);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BitCrusher01AudioProcessor();
}
