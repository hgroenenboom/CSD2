#pragma once

#include <string>
#include <functional>

// Hiermee kan je kiezen uit welke scope je wilt breaken door je scope een naam te geven.
#define named(blockname) goto blockname; \
                         blockname##_skip: if (0) \
                         blockname:

#define break(blockname) goto blockname##_skip

class AudioFile 
{
public:
	AudioFile();
	~AudioFile();

	// callbacks
	std::function<void()> fileLoadedCallback = nullptr;

	// audiofile variables
	long bitDepth = -1;
	long numSamples = -1;
	int numChannels = -1;
	std::string filename;
	std::string fullPath;
	bool fileLoaded = false;
	int sampleRate = 0;
	// the actual audiobuffer
	float** audio = nullptr;

	// callback functions for inheritance
	virtual void loadingAudiofile() {}
	virtual void audiofileLoaded() {}

	// strip silence from MDArray<float>
	void stripSilence();

	// generate new multidimensional array
	void newAudio(int numChans, int numSamps);

	// delete multidimensional array
	void deleteAudio();
};
