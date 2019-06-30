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
#include <string>

class AudioAnalyzer {
public:
	static const int NUMPARAMS = 2;
	const std::string PARAMS[NUMPARAMS] = { "length", "mean" };
	const std::string IDENTIFIER = "test v1.0";

	AudioAnalyzer();

	// stores whether analysis succeeds in the succes parameter
	AFFeatureSet analyseAudio(AudioFile* inputAudioFile, bool* succes) {
		*succes = false; 
		af.open(inputAudioFile->file.getFullPathName());
		af.read();

		// check whether input file is elegible
		if (af.numChannels > maxChannels || af.numSamples > maxSamples || af.fileLoaded == false) {
			return nullptr;
		}
		else {
			getAudio();

			f[getParamIndex("mean")] = getMean();
			f[getParamIndex("length")] = af.numSamples;

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
	float f[NUMPARAMS];
	std::string p[NUMPARAMS];

	void getAudio() {
		for (int c = 0; c < af.numChannels; c++) {
			for (int i = 0; i < af.numSamples; i++) {
				audio[c][i] = af.audio->getPointers()[c][i];
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

