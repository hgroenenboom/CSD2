/*
  ==============================================================================

    AudioAnalyzer.h
    Created: 30 Jun 2019 4:34:12pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include "LoadableAudioFile.h"
#include "AFFeatureSet.h"
#include <string>
#include "JuceFFTWrapper.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include <thread>
#include <functional>
#include <future>

class AudioAnalyzer {
public:
	// AudioAnalyzer settings
	static const int NUMPARAMS = 5;
	const std::string PARAMS[NUMPARAMS] = { "length", "mean", "low", "mid", "high" };
	const std::string IDENTIFIER = "test v1.0.1";

	AudioAnalyzer() {
		f.resize(NUMPARAMS);
		p.resize(NUMPARAMS);

		for (int i = 0; i < NUMPARAMS; i++) {
			p[i] = PARAMS[i];
		}
	}

	std::vector<AFFeatureSet>& getFeatureSets() {
		return featureSets;
	}

	void analyseAudioFiles(std::vector<std::string> files) {
		//std::thread t(&AudioAnalyzer::analyseFiles, this, files);
		analysisProgress = std::async(std::launch::async, &AudioAnalyzer::analyseFiles, this, files);
	}

	// stores whether analysis succeeds in the succes parameter
	AFFeatureSet analyseAudio(std::string inputAudioFile, bool* succes) {
		// init
		*succes = false;
		if ( af.open(inputAudioFile) ) {
			// check whether input file is elegible
			if (af.numChannels > maxChannels || af.numSamples > maxSamples) {
				return AFFeatureSet();
			}
			else {
				// get audio data and perform fft analysis
				af.read();
				getAudio();
				fft.processAF(&af);

				// store analysis values inside feature vector
				f[getParamIndex("mean")] = getMean();
				f[getParamIndex("length")] = 1.0f / (1.0f + 0.2f*(af.numSamples / af.sampleRate));
				const float lowLimit = 500.0f / af.sampleRate;
				const float midLimit = 2500.0f / af.sampleRate;
				f[getParamIndex("low")] = std::tanh(fft.getMagnitudeForPartOfSpectrum(0, lowLimit));
				f[getParamIndex("mid")] = std::tanh(fft.getMagnitudeForPartOfSpectrum(lowLimit, midLimit));
				f[getParamIndex("high")] = std::tanh(fft.getMagnitudeForPartOfSpectrum(midLimit, 1.0f));

				// create new feature set
				AFFeatureSet newSet(af.fullPath, IDENTIFIER, p, f);
				*succes = true;
				return newSet;
			}
		}
	}

	std::function<void()> filesAnalysed = nullptr;
	std::function<void()> fileAnalysed = nullptr;
	std::future<void> analysisProgress;
	int filesToAnalyse = 0;
	int numFilesAnalysed = 0;
private:
	std::vector<AFFeatureSet> featureSets;
	bool featureSetNotYetLoaded(std::string& file) {
		for (auto set : featureSets) {
			if (set.filePath == file && set.versionID == IDENTIFIER) {
				return false;
			}
		}
		return true;
	}

	void analyseFiles(std::vector<std::string> files) {
		filesToAnalyse = files.size();
		numFilesAnalysed = 0;
		for (std::string f : files) {
			if (featureSetNotYetLoaded(f)) {
				bool succes = false;
				AFFeatureSet set = analyseAudio(f, &succes);
				if (succes) {
					featureSets.push_back(set);
					if (fileAnalysed != nullptr) {
						fileAnalysed();
					}
				}
			}
			numFilesAnalysed++;
		}
		if (filesAnalysed != nullptr) {
			filesAnalysed();
		}
	}

	// audiorelated variables
	const static int maxChannels = 2;
	const static int maxSamples = 44100 * 60 * 4;
	float audio[2][maxSamples];

	LoadableAudioFile af;
	FFTWrapper fft;
	std::vector<float> f;
	std::vector<std::string> p;

	/********************************************/
	/*******************AUDIO********************/
	/********************************************/

	// get audio from selected audiofile
	void getAudio() {
		const auto loadedAudioPointer = af.audio;
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

