#include "bitCrusher.h"
#include <cmath>
#include<random>

void bitCrusher::setBitDepth(float newBitDepth)
{
	this->bitDepth = newBitDepth;
}
void bitCrusher::setSampleRateReduction(float newSampleRateReduction)
{
	this->sampleRateReduction = newSampleRateReduction;
}
void bitCrusher::setMix(float newMix)
{
	this->mix = newMix;
}
void bitCrusher::isFiltered(bool shouldFilter)
{
	this->shouldFilter = shouldFilter;
}
void bitCrusher::setHighpass(bool highpass)
{
	this->highpass = highpass;
}
void bitCrusher::setCutoffFrequency(float cutoffFrequency)
{
	this->cutoffFreq = cutoffFrequency;
}
void bitCrusher::setSamplingRate(float samplingRate)
{
	this->sampleRate = samplingRate;
}
void bitCrusher::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
	constexpr auto PI = 3.14159265359f;
	//Get number of channels and resize if there are more channles than expected. Initalize to 0.
	dnBuffer.resize(buffer.getNumChannels(), 0.0f);

	//if highpass is true then sign is negative, if highpass is false, then sign is positive.
	const auto sign = highpass ? -1.0f : 1.0f;
	//helper function for all pass
	const auto tan = std::tan(PI * cutoffFreq / sampleRate);
	//coefficient used for allpass processing and is in the transfer function for the allpass
	const auto a1 = (tan - 1.0f) / (tan + 1.0f);
	//start processing
	float drySample = 0.0f;
	float wetSample = 0.0f;


	for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
	{
		float* channelSamples = buffer.getWritePointer(channel);
		for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
		{
			drySample = channelSamples[sample];
			wetSample = drySample;

			if (sample % static_cast<int>(sampleRateReduction) == 0)
			{
				float scaled = wetSample * (std::pow(2, bitDepth - 1) - 1);
				float crushed = std::round(scaled) / (std::pow(2, bitDepth - 1) - 1);
				wetSample = crushed;
				previousSample = wetSample;
			}
			else
			{
				wetSample = previousSample;
			}
			if (shouldFilter)
			{
				const auto inputSample = wetSample;
				const auto allPassFilteredSample = a1 * inputSample + dnBuffer[channel];
				dnBuffer[channel] = inputSample - a1 * allPassFilteredSample;
				wetSample = 0.5f * (inputSample + sign * allPassFilteredSample);
			}
			float outputSample = drySample * (1.0f - mix) + wetSample * mix;
			channelSamples[sample] = outputSample;
		}
	}
}
