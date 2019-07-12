/*
  ==============================================================================

    AudioAnalyzer.cpp
    Created: 13 Jul 2019 12:40:17am
    Author:  HAROL

  ==============================================================================
*/

#include "AudioAnalyzer.h"
#include "../JuceLibraryCode/JuceHeader.h"

AudioAnalyzer::AudioAnalyzer() {
	featureSet.resize(NUMPARAMS);
	p.resize(NUMPARAMS);

	for (int i = 0; i < NUMPARAMS; i++) {
		p[i] = PARAMS[i];
	}
}

std::vector<AFFeatureSet>& AudioAnalyzer::getFeatureSets() {
	return featureSets;
}

void AudioAnalyzer::analyseAudioFilesAsynchrous(std::vector<std::string> files) {
	analysisProgress = std::async(std::launch::async, &AudioAnalyzer::analyseFiles, this, files);
}

// stores whether analysis succeeds in the succes parameter
AFFeatureSet AudioAnalyzer::analyseAudio(std::string inputAudioFile, bool* succes) {
	// init
	*succes = false;
	if (File(inputAudioFile).getSize() < 2000000)
		if (af.open(inputAudioFile)) {
			// check whether input file is elegible
			if (af.numChannels > maxChannels || af.numSamples > maxSamples) {
				return AFFeatureSet();
			}
			else {
				// get audio data and perform fft analysis
				af.read();
				getAudio();
				fft.processAF(af);

				const int s = featureSet.size();
				// store analysis values inside feature vector
				featureSet[getParamIndex("mean")] = getMean();
				featureSet[getParamIndex("length")] = 1.0f / (1.0f + 0.2f*(af.numSamples / af.sampleRate));
				const float lowLimit = 500.0f / af.sampleRate;
				const float midLimit = 2500.0f / af.sampleRate;
				featureSet[getParamIndex("low")] = std::tanh(fft.getMagnitudeForPartOfSpectrum(0, lowLimit));
				featureSet[getParamIndex("mid")] = std::tanh(fft.getMagnitudeForPartOfSpectrum(lowLimit, midLimit));
				featureSet[getParamIndex("high")] = std::tanh(fft.getMagnitudeForPartOfSpectrum(midLimit, 1.0f));

				for (int i = 0; i < NUMPARAMS; i++) {
					jassert(!isnan(featureSet[i]));
				}

				// create new feature set
				AFFeatureSet newSet(af.fullPath, IDENTIFIER, p, featureSet);
				*succes = true;
				return newSet;
			}
		}

	return AFFeatureSet();
}

bool AudioAnalyzer::featureSetNotYetLoaded(std::string& file) {
	for (auto set : featureSets) {
		if (set.filePath == file && set.versionID == IDENTIFIER) {
			return false;
		}
	}
	return true;
}

void AudioAnalyzer::analyseFiles(std::vector<std::string> files) {
	filesToAnalyse = files.size();
	numFilesAnalysed = 0;
	for (std::string f : files) {
		if (featureSetNotYetLoaded(f)) {
			DBG("current_file:\t" + f);
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

/********************************************/
/*******************AUDIO********************/
/********************************************/

// get audio from selected audiofile
void AudioAnalyzer::getAudio() {
	const auto loadedAudioPointer = af.audio;
	for (int c = 0; c < af.numChannels; c++) {
		for (int i = 0; i < af.numSamples; i++) {
			audio[c][i] = loadedAudioPointer[c][i];
		}
	}
}

float AudioAnalyzer::getMean() {
	return absoluteSum() / (af.numChannels * af.numSamples);
}

void AudioAnalyzer::multiply(float am) {
	for (int c = 0; c < af.numChannels; c++) {
		for (int i = 0; i < af.numSamples; i++) {
			audio[c][i] *= am;
		}
	}
}

void AudioAnalyzer::absolute() {
	for (int c = 0; c < af.numChannels; c++) {
		for (int i = 0; i < af.numSamples; i++) {
			audio[c][i] = abs(audio[c][i]);
		}
	}
}

void AudioAnalyzer::power(float am) {
	for (int c = 0; c < af.numChannels; c++) {
		for (int i = 0; i < af.numSamples; i++) {
			audio[c][i] = pow(audio[c][i], am);
		}
	}
}

void AudioAnalyzer::add(float am) {
	for (int c = 0; c < af.numChannels; c++) {
		for (int i = 0; i < af.numSamples; i++) {
			audio[c][i] += am;
		}
	}
}

float AudioAnalyzer::sum() {
	float sum = 0.0f;
	for (int c = 0; c < af.numChannels; c++) {
		for (int i = 0; i < af.numSamples; i++) {
			sum += audio[c][i];
		}
	}
	return sum;
}

float AudioAnalyzer::absoluteSum() {
	float sum = 0.0f;
	for (int c = 0; c < af.numChannels; c++) {
		for (int i = 0; i < af.numSamples; i++) {
			sum += abs(audio[c][i]);
		}
	}
	return sum;
}

int AudioAnalyzer::getParamIndex(std::string s) {
	for (int i = 0; i < NUMPARAMS; i++) {
		if (PARAMS[i] == s) {
			return i;
		}
	}
}