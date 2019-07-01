#pragma once

#include <algorithm>
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
	AudioFile() {
	};
	~AudioFile()
	{
		deleteAudio();
	}

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
	void stripSilence()
	{
		int startSample = 0;
		int endSample = numSamples;
		
		// find startsample
		named (outer)
		for (int i = 0; i < numSamples; i++) 
		{
			for (int c = 0; c < numChannels; c++)
			{
				const float v = std::abs(audio[c][i]);
				if ( v > 0.000001f )
				{
					startSample = i;
					break(outer);
				} 			
			}
		}

		// find endsample
		named(outer_2)
		for (int i = numSamples - 1; i == 0; i--)
		{
			for (int c = 0; c < numChannels; c++)
			{
				const float v = std::abs(audio[c][i]);
				if (v > 0.000001f)
				{
					endSample = i;
					break(outer_2);
				}
			}
		}

		// create temporary MD array
		float** temp = new float*[numChannels];
		for (int i = 0; i < numChannels; i++) {
			temp[i] = new float[endSample - startSample];
		}
		
		// copy trimmed audio to temporary array
		for (int c = 0; c < numChannels; c++)
		{
			for (int i = 0; i < endSample - startSample; i++) {
				temp[c][i] = audio[c][i + startSample];
			}
		}

		// swap pointers and adjust length
		deleteAudio();
		audio = temp;
		numSamples = endSample - startSample-1;
	}

	// generate new multidimensional array
	void newAudio(int numChans, int numSamps) {
		numChannels = numChans;
		numSamples = numSamps;

		audio = new float*[numChans];
		for (int i = 0; i < numChans; i++) {
			audio[i] = new float[numSamps];
		}
	}

	// delete multidimensional array
	void deleteAudio() {
		if (audio != nullptr) {
			for (int i = 0; i < numChannels; i++) {
				delete[] audio[i];
			}
			delete[] audio;
			audio = nullptr;
		}
	}
};
