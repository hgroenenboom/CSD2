/*
  ==============================================================================

    JuceFFTWrapper.h
    Created: 1 Jul 2019 1:15:23pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioFile.h"

class FFTFunctionWrapper {
public:
	FFTFunctionWrapper();

	// get FFT from input audiofile
	// calcultate magnitude spectrum
	void processAF(AudioFile& af);

	// get magnitude amount for a part of the fourier transform.
	float getMagnitudeForPartOfSpectrum(float rmin, float rmax);
private:
	// FFT settings
	const static int FFTORDER = 12;
	const static int FFTSIZE = 2 << FFTORDER;

	// fft object and it's data
	dsp::FFT forwardFFT;
	float fftData[2 * FFTSIZE];
	float magnitudes[FFTSIZE >> 2];

	// helper functions
	void clearMemory();
	void clearFFTData();
};
