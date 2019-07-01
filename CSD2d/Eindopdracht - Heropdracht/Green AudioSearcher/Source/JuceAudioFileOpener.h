/*
  ==============================================================================

    JuceAudioFileOpener.h
    Created: 1 Jul 2019 2:30:18pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include <juce_audio_formats/juce_audio_formats.h>
#include "AudioFile.h"

class JuceAudioFileOpener {
public:
	JuceAudioFileOpener() {
		formatManager.registerBasicFormats();
	}

	~JuceAudioFileOpener() {
		delete reader;
	}

	// open a audiofile and write data into the passed AudioFile object
	void open(AudioFile* af, File f) {
		// init
		if (reader != nullptr)
			delete reader;
		file = File(f);
		reader = formatManager.createReaderFor(f); // [10]

		// load audiofile attributes
		if (reader != nullptr)
		{
			af->numChannels = reader->numChannels;
			af->bitDepth = reader->bitsPerSample;
			af->numSamples = reader->lengthInSamples;
			af->filename = file.getFileName().toStdString();
			af->fullPath = file.getFullPathName().toStdString();
			af->sampleRate = reader->sampleRate;
		}

		// read audiofile data
		read(af);
	}
private:
	void read(AudioFile* af)
	{
		af->fileLoaded = false;
		af->newAudio(af->numChannels, af->numSamples);
		reader->read(af->audio, af->numChannels, 0, af->numSamples);
		af->stripSilence();
		af->fileLoaded = true;
	}

	// JUCE functionality
	File file;
	AudioFormatReader* reader = nullptr;
	AudioFormatManager formatManager;
};