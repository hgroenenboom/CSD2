/*
  ==============================================================================

    SimilarityChecker.h
    Created: 30 Jun 2019 6:34:35pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include "AFFeatureSet.h"
#include <limits>
#include "LabelSlider.h"
#include "AudioAnalyzer.h"

class SimilarityChecker : public Component {
public:
	SimilarityChecker(AudioAnalyzer* aAnalyzer) {
		// initialize weights and weightslider
		weights.resize(aAnalyzer->NUMPARAMS);
		sliders.resize(aAnalyzer->NUMPARAMS);
		for (int i = 0; i < sliders.size(); i++) {
			sliders[i] = new LabelSlider();
			auto s = aAnalyzer->PARAMS[i];
			sliders[i]->setSlider(0.0f, 1.0f, 0.001f, 0.5f, 1.0f, String(s) );
			sliders[i]->callBack = [=](float v) {
				weights[i] = v;
			};
			addAndMakeVisible(sliders[i]);
		}
	}

	~SimilarityChecker() {
		for (int i = 0; i < sliders.size(); i++) {
			delete sliders[i];
		}
	}

	void paint(Graphics& g) override {

	}
	void resized() override {
		// place elements inside the components drawing region
		auto l = getLocalBounds();
		const float sliderHeight = getHeight() / sliders.size();
		for (int i = 0; i < sliders.size(); i++) {
			sliders[i]->setBounds(l.removeFromTop(sliderHeight));
		}
	}

	// get featuresets vector sorted by it's similarity to the input featureset
	std::vector<int> sortBySimilarity(AFFeatureSet& s, std::vector<AFFeatureSet>& sets, int numResults=1) {
		resultingIndices.resize(sets.size());
		
		// test every file for similarity, store similarity with the file's index into a vector.
		for (int i = 0; i < sets.size(); i++) {
			AFFeatureSet& s_2 = sets[i];

			if (s.filePath == s_2.filePath) {
				resultingIndices[i] = std::pair<float, int>(10000000000.0f, i);
			}
			else {
				resultingIndices[i] = std::pair<float, int>(getEuclideanDistance(s, s_2), i);
			}
		}

		// sort vector, to put the most similar files first.
		std::sort(resultingIndices.begin(), resultingIndices.end());
		
		// get indices of the amount of similar files which the user requested.
		std::vector<int> sortedIndices;
		sortedIndices.resize(numResults);
		for (int i = 0; i < numResults; i++) {
			sortedIndices[i] = resultingIndices[i].second;
		}

		return sortedIndices;
	}
private:
	// get the euclidean distance between two AFFeatureSets
	float getEuclideanDistance(AFFeatureSet& s1, AFFeatureSet& s2) {
		float sum = 0.0f;
		for (int i = 0; i < s1.numValues; i++) {
			sum += weights[i] * pow(s1.values[i] - s2.values[i], 2.0f);
		}
		return sqrt(sum);
	}

	std::vector<std::pair<float, int>> resultingIndices;

	std::vector<float> weights;
	std::vector<LabelSlider*> sliders;
};