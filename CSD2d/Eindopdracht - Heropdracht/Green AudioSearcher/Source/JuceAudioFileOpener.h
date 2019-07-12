/*
  ==============================================================================

    JuceAudioFileOpener.h
    Created: 1 Jul 2019 2:30:18pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include "AudioFile.h"
#include "../JuceLibraryCode/JuceHeader.h"

class JuceAudioFileOpener {
public:
	JuceAudioFileOpener();
	~JuceAudioFileOpener();

	// open a audiofile and write data into the passed AudioFile object
	bool open(AudioFile& af, File f);
	void read(AudioFile& af);
private:
	// JUCE functionality
	File file;
	AudioFormatReader* reader = nullptr;
	AudioFormatManager formatManager;
};