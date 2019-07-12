/*
  ==============================================================================

    JuceFFTWrapper.cpp
    Created: 13 Jul 2019 12:35:42am
    Author:  HAROL

  ==============================================================================
*/

#include "JuceFFTWrapper.h"

FFTFunctionWrapper::FFTFunctionWrapper()
	: forwardFFT(FFTORDER)
{
}

// get FFT from input audiofile
void FFTFunctionWrapper::processAF(AudioFile& af) {
	const int numIterations = ceil(af.numSamples / FFTSIZE);
	const float invNumChannels = 1.0f / af.numChannels;
	clearMemory();

	// Add whole audiofile;
	for (int n = 0; n < numIterations; n++) {
		// add audio to fftbuffer
		clearFFTData();
		for (int i = 0; i < FFTSIZE; i++) {
			const int index = n * FFTSIZE + i;
			DBG(std::to_string(index) + " with numsamples: " + std::to_string(af.numSamples));
			for (int c = 0; c < af.numChannels; c++) {
				if (index < af.numSamples) {
					jassert(i < FFTSIZE);
					//fftData[i] += invNumChannels * af.audio[c][index];
				}
			}
		}

		//// calculate FFT
		forwardFFT.performRealOnlyForwardTransform(fftData);
		for (int i = 0; i < 0.5f * FFTSIZE; i++) {
			magnitudes[i] += sqrt(pow(fftData[2 * i], 2.0f) + pow(fftData[2 * i + 1], 2.0f));
		}
	}

	// normalize values
	const float invNumIters = 1.0f / numIterations;
	for (int i = 0; i < 0.5f * FFTSIZE; i++) {
		magnitudes[i] *= invNumIters;
		jassert(!isnan(magnitudes[i]));
	}
	//const float sum = getMagnitudeForPartOfSpectrum(0.0f, 1.0f);
}

// get magnitude amount for a part of the fourier transform.
float FFTFunctionWrapper::getMagnitudeForPartOfSpectrum(float rmin, float rmax) {
	float sum = 0.0f;
	const int rminInt = round(rmin * (FFTSIZE / 4));
	const int rmaxInt = round(rmax * (FFTSIZE / 4));
	for (int i = rminInt; i < rmaxInt; i++) {
		sum += magnitudes[i];
	}
	sum /= rmaxInt - rminInt;
	jassert(!isnan(sum));
	return sum;
}

void FFTFunctionWrapper::clearMemory() {
	clearFFTData();
	//std::fill(magnitudes, magnitudes + (FFTSIZE / 4), 0.0f);
}
void FFTFunctionWrapper::clearFFTData() {
	//std::fill(fftData, fftData + 2 * FFTSIZE, 0.0f);
}
