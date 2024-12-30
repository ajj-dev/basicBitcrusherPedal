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
class BitCrusher01AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BitCrusher01AudioProcessorEditor (BitCrusher01AudioProcessor&, juce::AudioProcessorValueTreeState& vts);
    ~BitCrusher01AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BitCrusher01AudioProcessor& audioProcessor;

    juce::Slider bitDepthSlider;
    juce::Label bitDepthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitDepthAttachment;

    juce::Slider sampleReductionSlider;
    juce::Label sampleReductionLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sampleReductionAttachment;

    juce::Slider mixSlider;
    juce::Label mixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

    juce::Slider cutoffFreqSlider;
    juce::Label cutoffFreqLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffFreqAttachment;

    juce::Label highpassButtonLabel;
    juce::ToggleButton highpassButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>highpassAttachment;

    juce::Label shouldFilterLabel;
    juce::ToggleButton shouldFilterButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>shouldFilterAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitCrusher01AudioProcessorEditor)
};
