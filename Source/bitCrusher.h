#pragma once

#include <JuceHeader.h>
#include <vector>
class bitCrusher
{
public:
	bitCrusher() : bitDepth(16), sampleRateReduction(1){}

	void setBitDepth(float newBitDepth);
	void setSampleRateReduction(float newSampleRateReduction);
	void setMix(float newMix);

	void setHighpass(bool highpass);
	void setCutoffFrequency(float cutoffFrequency);
	void setSamplingRate(float sampleRate);
	void isFiltered(bool shouldFilter);

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

private:
	float bitDepth = 16;
	float sampleRateReduction = 1;
	float mix = 0.5;
	float previousSample = 0.0f;

	bool highpass = false;
	bool shouldFilter = true;
	float cutoffFreq = 500;
	float sampleRate = 44100;
	std::vector<float> dnBuffer;
};

