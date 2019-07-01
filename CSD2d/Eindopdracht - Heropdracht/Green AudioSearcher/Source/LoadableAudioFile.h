/*
  ==============================================================================

    LoadableAudioFile.h
    Created: 1 Jul 2019 2:33:28pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include "AudioFile.h"
#include "JuceAudioFileOpener.h"

class LoadableAudioFile : public AudioFile {
public:
	LoadableAudioFile(std::string filePath)
	{
		open(filePath);
	}
	LoadableAudioFile()
	{
	}

	// open new audiofile
	void open(std::string filePath)
	{
		// init
		loadingAudiofile();
		fileLoaded = false;
		if (audio != nullptr)
			deleteAudio();

		// open file
		audioFileOpener.open(this, File(filePath) );

		// finish
		fileLoaded = true;
		audiofileLoaded();
		if (fileLoadedCallback != nullptr)
			fileLoadedCallback();
	}


private:
	// used for opening an audiofile via the JUCE frameworks
	JuceAudioFileOpener audioFileOpener;
};