#pragma once
#include "Array.h"
#include "fftw++.h"

namespace dsp
{
	class SignalProcessor
	{
	public:
		SignalProcessor(unsigned int sampleRate);
		void process(int16_t* samples);
		unsigned int getSize();
		double getResolution();
		double getMagnitude(int i);
		const Complex& operator[](int i);
	private:
		unsigned int size;
		unsigned int sampleRate;
		Array::array1<Complex> data;
		fftwpp::fft1d forward;
		double hammingWindow(int16_t i);
		unsigned int niquistFreq(unsigned int sampleRate);
		unsigned int optimalSize(unsigned int sampleRate, double targetResolution = 50);
	};
}