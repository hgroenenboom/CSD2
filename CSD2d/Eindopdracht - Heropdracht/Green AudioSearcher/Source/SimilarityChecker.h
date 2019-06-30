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

class SimilarityChecker{
public:
	SimilarityChecker() {

	}

	void sortBySimilarity(AFFeatureSet& s, std::vector<AFFeatureSet>& sets) {
		resultingIndices.resize(sets.size());
		
		for (AFFeatureSet& s_2 : sets) {
			if (&s == &s_2) {
				resultingIndices.push_back( std::numeric_limits<float>::max() );
			}
			else {
				resultingIndices.push_back(getEuclideanDistance(s, s_2));
			}
		}
	}
private:
	float getEuclideanDistance(AFFeatureSet& s1, AFFeatureSet& s2) {
		float sum = 0.0f;
		for (int i = 0; i < s1.numValues; i++) {
			sum += pow(s1.values[i] - s2.values[i], 2.0f);
		}
		return sqrt(sum);
	}

	std::vector<int> resultingIndices;
};