/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BitCrusher01AudioProcessorEditor::BitCrusher01AudioProcessorEditor (BitCrusher01AudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    //BIT DEPTH SLIDER
    addAndMakeVisible(bitDepthSlider);
    bitDepthSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    bitDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    bitDepthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "bit_depth", bitDepthSlider));
    //BIT DEPTH LABEL
    addAndMakeVisible(bitDepthLabel);
    bitDepthLabel.setText("Bit Depth", juce::dontSendNotification);
    //SAMPLE REDUCTION SLIDER
    addAndMakeVisible(sampleReductionSlider);
    sampleReductionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sampleReductionSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    sampleReductionAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "sample_rate_reduction", sampleReductionSlider));
    //SAMPLE REDUCTION LABEL
    addAndMakeVisible(sampleReductionLabel);
    sampleReductionLabel.setText("Sample Rate Reduction", juce::dontSendNotification);
    //MIX SLIDER
    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    mixAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "mix", mixSlider));
    //MIX LABEL
    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", juce::dontSendNotification);
    //CUT OFF FREQ SLIDER
    addAndMakeVisible(cutoffFreqSlider);
    cutoffFreqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    cutoffFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    cutoffFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(vts, "cutoff_frequency", cutoffFreqSlider));
    //CUT OFF FREQ LABEL
    addAndMakeVisible(cutoffFreqLabel);
    cutoffFreqLabel.setText("Cutoff Frequency", juce::dontSendNotification);
    //HIGH PASS BUTTON
    addAndMakeVisible(highpassButton);
    highpassAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "highpass", highpassButton));
    //HIGHPASS LABEL
    addAndMakeVisible(highpassButtonLabel);
    highpassButtonLabel.setText("Highpass", juce::dontSendNotification);
    //SHOULD FILTER BUTTON
    addAndMakeVisible(shouldFilterButton);
    shouldFilterAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(vts, "should_filter", shouldFilterButton));
    //SHOULD FILTER LABEL
    addAndMakeVisible(shouldFilterLabel);
    shouldFilterLabel.setText("Should Filter", juce::dontSendNotification);

    setSize (500, 400);
}

BitCrusher01AudioProcessorEditor::~BitCrusher01AudioProcessorEditor()
{
}

//==============================================================================
void BitCrusher01AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
}

void BitCrusher01AudioProcessorEditor::resized()
{
    const int sliderWidth = 100;
    const int sliderHeight = 250;
    const int labelHeight = 25;
    const int sliderSpacing = 20; // Space between sliders
    const int buttonWidth = 100;
    const int buttonHeight = 30;
    const int buttonSpacing = 10; // Space between buttons and labels
    const int buttonsYOffset = sliderHeight + 2 * labelHeight + 10; // Y offset for buttons below sliders

    // Calculate the starting X position for the sliders (left side)
    int xPos = sliderSpacing;

    // Position bitDepthSlider and its label
    bitDepthSlider.setBounds(xPos, 20, sliderWidth, sliderHeight);
    bitDepthLabel.setBounds(xPos, sliderHeight + 20, sliderWidth, labelHeight);
    xPos += sliderWidth + sliderSpacing;

    // Position sampleReductionSlider and its label
    sampleReductionSlider.setBounds(xPos, 20, sliderWidth, sliderHeight);
    sampleReductionLabel.setBounds(xPos, sliderHeight + 20, sliderWidth, labelHeight);
    xPos += sliderWidth + sliderSpacing;

    // Position mixSlider and its label
    mixSlider.setBounds(xPos, 20, sliderWidth, sliderHeight);
    mixLabel.setBounds(xPos, sliderHeight + 20, sliderWidth, labelHeight);

    // Position the buttons directly below the first 3 sliders (centered under each slider)
    int buttonXPos = sliderSpacing; // Start button positioning under first slider

    highpassButton.setBounds(buttonXPos, buttonsYOffset, buttonWidth, buttonHeight);
    highpassButtonLabel.setBounds(buttonXPos, buttonsYOffset + buttonHeight + buttonSpacing, buttonWidth, labelHeight);
    buttonXPos += sliderWidth + sliderSpacing;

    shouldFilterButton.setBounds(buttonXPos, buttonsYOffset, buttonWidth, buttonHeight);
    shouldFilterLabel.setBounds(buttonXPos, buttonsYOffset + buttonHeight + buttonSpacing, buttonWidth, labelHeight);
    buttonXPos += sliderWidth + sliderSpacing;


    // Continue with other sliders to the right of the buttons
    xPos = sliderSpacing; // Reset xPos to align other sliders correctly

    // Position cutoffFreqSlider and its label below the previously positioned sliders
    xPos += (sliderWidth + sliderSpacing) * 3; // Align after the three buttons
    cutoffFreqSlider.setBounds(xPos, 20, sliderWidth, sliderHeight);
    cutoffFreqLabel.setBounds(xPos, sliderHeight + 20, sliderWidth, labelHeight);

    // Position noiseAmountSlider and its label
    xPos += sliderWidth + sliderSpacing;
}

