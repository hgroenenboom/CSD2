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
#include <functional>
#include <future>

class AudioAnalyzer {
public:
	// AudioAnalyzer settings
	static const int NUMPARAMS = 5;
	const std::string PARAMS[NUMPARAMS] = { "length", "mean", "low", "mid", "high" };
	const std::string IDENTIFIER = "test v1.0.1";

	AudioAnalyzer();

	std::vector<AFFeatureSet>& getFeatureSets();

	void analyseAudioFilesAsynchrous(std::vector<std::string> files);

	// stores whether analysis succeeds in the succes parameter
	AFFeatureSet analyseAudio(std::string inputAudioFile, bool* succes);

	std::function<void()> filesAnalysed = nullptr;
	std::function<void()> fileAnalysed = nullptr;
	std::future<void> analysisProgress;
	int filesToAnalyse = 0;
	int numFilesAnalysed = 0;
private:
	std::vector<AFFeatureSet> featureSets;
	bool featureSetNotYetLoaded(std::string& file);

	void analyseFiles(std::vector<std::string> files);

	// audiorelated variables
	const static int maxChannels = 2;
	const static int maxSamples = 44100 * 60 * 4;
	float audio[2][maxSamples];

	LoadableAudioFile af;
	std::vector<float> featureSet;
	std::vector<std::string> p;

	/********************************************/
	/*******************AUDIO********************/
	/********************************************/

	// get audio from selected audiofile
	void getAudio();

	// transform
	void multiply(float am);
	void absolute();
	void power(float am);
	void add(float am);

	// analyse
	float getMean();
	float sum();
	float absoluteSum();

	int getParamIndex(std::string s);

	FFTFunctionWrapper fft;
};

