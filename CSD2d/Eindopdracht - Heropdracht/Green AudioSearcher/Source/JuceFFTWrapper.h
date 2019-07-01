/*
  ==============================================================================

    JuceFFTWrapper.h
    Created: 1 Jul 2019 1:15:23pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FFTWrapper {
	// FFT settings
	const static int FFTORDER = 8;
	const static int FFTSIZE = 2 << FFTORDER;

	// fft object
	dsp::FFT forwardFFT;
	float fftData[2 * FFTSIZE];
	float magnitudes[FFTSIZE >> 1];
public:
	FFTWrapper()
		: forwardFFT(FFTORDER)
	{
	}

	// get FFT from input audiofile
	void processAF(AudioFile* af) {
		const int numIterations = ceil(af->numSamples / FFTSIZE);
		const float invNumChannels = 1.0f / af->numChannels;

		// Add whole audiofile;
		for (int n = 0; n < numIterations; n++) {
			for (int i = 0; i < FFTSIZE; i++) {
				fftData[i] = 0;

				const int index = n * FFTSIZE + i;
				for (int c = 0; c < af->numChannels; c++) {
					if (index < af->numSamples) {
						fftData[i] += invNumChannels * af->audio[c][n*FFTSIZE + i];
					}
				}
			}
		}

		// normalize values
		const float invNumIters = 1.0f / numIterations;
		for (int i = 0; i < FFTSIZE; i++) {
			fftData[i] *= invNumIters;
		}

		// calculate FFT
		forwardFFT.performRealOnlyForwardTransform(fftData);
		float maxMag = 0.0f;
		for (int i = 0; i < FFTSIZE >> 1; i++) {
			magnitudes[i] = sqrt(pow(fftData[2 * i], 2.0f) + pow(fftData[2 * i + 1], 2.0f));
			maxMag = std::max(magnitudes[i], maxMag);
		}
		for (int i = 0; i < FFTSIZE >> 1; i++) {
			magnitudes[i] /= maxMag;
		}
	}

	// get magnitude amount for a part of the fourier transform.
	float getMagnitudeForPartOfSpectrum(float rmin, float rmax) {
		float sum = 0.0f;
		const int rminInt = round(rmin * (FFTSIZE / 4));
		const int rmaxInt = round(rmax * (FFTSIZE / 4));
		for (int i = rminInt; i < rmaxInt; i++) {
			sum += magnitudes[i];
		}
		sum /= rmaxInt - rminInt;
		return sum;
	}
};
