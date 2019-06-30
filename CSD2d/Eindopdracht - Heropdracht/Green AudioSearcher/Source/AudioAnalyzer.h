/*
  ==============================================================================

    AudioAnalyzer.h
    Created: 30 Jun 2019 4:34:12pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include "AudioFile.h"
#include "AFFeatureSet.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

class FFTWrapper {
	const static int FFTORDER = 8;
	const static int FFTSIZE = 2 << FFTORDER;

	dsp::FFT forwardFFT;            
	float fftData[2 * FFTSIZE];
	float magnitudes[FFTSIZE >> 1];
public:
	FFTWrapper() 
		: forwardFFT(FFTORDER)
	{
	}

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
						fftData[i] += invNumChannels * af->audio->getPointers()[c][n*FFTSIZE + i];
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
			magnitudes[i] = sqrt( pow( fftData[ 2 * i ], 2.0f ) + pow( fftData[ 2 * i + 1 ], 2.0f ) );
			maxMag = std::max(magnitudes[i], maxMag);
		}
		for (int i = 0; i < FFTSIZE >> 1; i++) {
			magnitudes[i] /= maxMag;
		}
	}

	float getMagnitudeForPartOfSpectrum(float rmin, float rmax) {
		float sum = 0.0f;
		const int rminInt = round(rmin * (FFTSIZE/4) );
		const int rmaxInt = round(rmax * (FFTSIZE/4) );
		for (int i = rminInt; i < rmaxInt; i++) {
			sum += magnitudes[i];
		}
		sum /= rmaxInt - rminInt;
		return sum;
	}
};

class AudioAnalyzer {
public:
	static const int NUMPARAMS = 5;
	const std::string PARAMS[NUMPARAMS] = { "length", "mean", "low", "mid", "high" };
	const std::string IDENTIFIER = "test v1.0.1";

	AudioAnalyzer();

	// stores whether analysis succeeds in the succes parameter
	AFFeatureSet analyseAudio(String inputAudioFile, bool* succes) {
		*succes = false; 
		af.open(inputAudioFile);

		// check whether input file is elegible
		if (af.numChannels > maxChannels || af.numSamples > maxSamples || af.fileLoaded == false) {
			return AFFeatureSet();
		}
		else {
			getAudio();
			fft.processAF(&af);

			f[getParamIndex("mean")] = getMean();
			f[getParamIndex("length")] = 1.0f / (1.0f+ 0.2f*(af.numSamples / af.sampleRate));
			const float lowLimit = 500.0f / af.sampleRate;
			const float midLimit = 2500.0f / af.sampleRate;
			f[getParamIndex("low")] = std::tanh(fft.getMagnitudeForPartOfSpectrum(0, lowLimit));
			f[getParamIndex("mid")] = std::tanh(fft.getMagnitudeForPartOfSpectrum(lowLimit, midLimit));
			f[getParamIndex("high")] = std::tanh(fft.getMagnitudeForPartOfSpectrum(midLimit, 1.0f));

			AFFeatureSet newSet( af.file.getFullPathName().toStdString(), IDENTIFIER, p, f, NUMPARAMS);
			*succes = true;
			return newSet;
		}
	}

	const static int maxChannels = 2;
	const static int maxSamples = 44100 * 60 * 4;
	float audio[2][maxSamples];

private:
	AudioFile af;
	FFTWrapper fft;
	float f[NUMPARAMS];
	std::string p[NUMPARAMS];

	void getAudio() {
		const auto loadedAudioPointer = af.audio->getPointers();
		for (int c = 0; c < af.numChannels; c++) {
			for (int i = 0; i < af.numSamples; i++) {
				audio[c][i] = loadedAudioPointer[c][i];
			}
		}
	}

	float getMean() {
		return absoluteSum() / (af.numChannels * af.numSamples);
	}

	void multiply(float am) {
		for (int c = 0; c < af.numChannels; c++) {
			for (int i = 0; i < af.numSamples; i++) {
				audio[c][i] *= am;
			}
		}
	}

	void absolute() {
		for (int c = 0; c < af.numChannels; c++) {
			for (int i = 0; i < af.numSamples; i++) {
				audio[c][i] = abs(audio[c][i]);
			}
		}
	}

	void power(float am) {
		for (int c = 0; c < af.numChannels; c++) {
			for (int i = 0; i < af.numSamples; i++) {
				audio[c][i] = pow(audio[c][i], am);
			}
		}
	}

	void add(float am) {
		for (int c = 0; c < af.numChannels; c++) {
			for (int i = 0; i < af.numSamples; i++) {
				audio[c][i] += am;
			}
		}
	}

	float sum() {
		float sum = 0.0f;
		for (int c = 0; c < af.numChannels; c++) {
			for (int i = 0; i < af.numSamples; i++) {
				sum += audio[c][i];
			}
		}
		return sum;
	}

	float absoluteSum() {
		float sum = 0.0f;
		for (int c = 0; c < af.numChannels; c++) {
			for (int i = 0; i < af.numSamples; i++) {
				sum += abs(audio[c][i]);
			}
		}
		return sum;
	}

	int getParamIndex(std::string s) {
		for (int i = 0; i < NUMPARAMS; i++) {
			if (PARAMS[i] == s) {
				return i;
			}
		}
	}
};

