#include "SignalProcessor.h"

using namespace dsp;

SignalProcessor::SignalProcessor(unsigned int sampleRate)
	: size(optimalSize(sampleRate))
	, sampleRate(sampleRate)
	, data(size, sizeof(Complex))
	, forward(-1, data)
{
	fftwpp::fftw::maxthreads = get_max_threads();
}

void SignalProcessor::process(int16_t* samples)
{
	for (unsigned int i = 0; i < size; i++)
	{
		data[i] = samples[i] * hammingWindow(i);
	}
	forward.fft(data);
}

unsigned int SignalProcessor::getSize()
{
	return size;
}

double SignalProcessor::getResolution()
{
	return niquistFreq(sampleRate) / (size / double(2));
}

double dsp::SignalProcessor::getMagnitude(int i)
{
	return abs(data[i]) / size;
}

const Complex& SignalProcessor::operator[](int i)
{
	return data[i];
}

unsigned int SignalProcessor::niquistFreq(unsigned int sampleRate)
{
	unsigned int result = 1;
	while (result < sampleRate / 2)
	{
		result = result << 1;
	}
	return result;
}

unsigned int SignalProcessor::optimalSize(unsigned int sampleRate, double targetResolution)
{
	unsigned int result = 1;
	while (result < sampleRate / targetResolution)
	{
		result = result << 1;
	}
	return result;
}

double SignalProcessor::hammingWindow(int16_t i)
{
	return 0.53836 * (1 - cos((2.0 * 3.14159265359 * i) / (size - 1)));
}
