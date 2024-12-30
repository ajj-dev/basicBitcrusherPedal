/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "bitCrusher.h"


//==============================================================================
/**
*/
class BitCrusher01AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BitCrusher01AudioProcessor();
    ~BitCrusher01AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;
    bitCrusher bitcrusher;

    std::atomic<float>* bitDepthParam = nullptr;
    std::atomic<float>* sampleRateReductionParam = nullptr;

    std::atomic<float>* mixParam = nullptr;

    std::atomic<float>* cutoffFrequencyParam = nullptr;
    std::atomic<float>* highpassParam = nullptr;
    std::atomic<float>* shouldFilterParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitCrusher01AudioProcessor)
};
