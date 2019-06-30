/*
  ==============================================================================

    AudioAnalyzer.cpp
    Created: 30 Jun 2019 5:18:06pm
    Author:  HAROL

  ==============================================================================
*/

#include "AudioAnalyzer.h"

AudioAnalyzer::AudioAnalyzer() {
	for (int i = 0; i < NUMPARAMS; i++) {
		p[i] = PARAMS[i];
	}
};

