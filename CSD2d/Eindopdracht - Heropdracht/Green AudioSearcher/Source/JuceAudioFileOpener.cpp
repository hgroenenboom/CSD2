/*
  ==============================================================================

    JuceAudioFileOpener.cpp
    Created: 13 Jul 2019 12:39:36am
    Author:  HAROL

  ==============================================================================
*/

#include "JuceAudioFileOpener.h"

JuceAudioFileOpener::JuceAudioFileOpener() {
	formatManager.registerBasicFormats();
}

JuceAudioFileOpener::~JuceAudioFileOpener() {
	delete reader;
}

// open a audiofile and write data into the passed AudioFile object
bool JuceAudioFileOpener::open(AudioFile& af, File f) {
	// init
	if (reader != nullptr) {
		delete reader;
		reader = nullptr;
	}
	file = File(f);
	reader = formatManager.createReaderFor(f); // [10]

											   // load audiofile attributes
											   //jassert(reader != nullptr);
	if (reader != nullptr)
	{
		jassert(reader->numChannels > 0);
		af.numChannels = reader->numChannels;
		af.bitDepth = reader->bitsPerSample;
		af.numSamples = reader->lengthInSamples;
		af.filename = file.getFileName().toStdString();
		af.fullPath = file.getFullPathName().toStdString();
		af.sampleRate = reader->sampleRate;

		return true;
	}
	return false;
}

void JuceAudioFileOpener::read(AudioFile& af)
{
	af.fileLoaded = false;
	af.newAudio(af.numChannels, af.numSamples);
	reader->read(af.audio, af.numChannels, 0, af.numSamples);
	af.stripSilence();
	af.fileLoaded = true;
}