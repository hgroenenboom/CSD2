#pragma once

#include "MDArray.h"
#include "../JuceLibraryCode/JuceHeader.h"

// Hiermee kan je kiezen uit welke scope je wilt breaken door je scope een naam te geven.
#define named(blockname) goto blockname; \
                         blockname##_skip: if (0) \
                         blockname:

#define break(blockname) goto blockname##_skip

class AudioFile 
{
public:
	AudioFile(File path)
	{
		init();
		open(path);
	}
	AudioFile() 
	{
		init();
	}

	~AudioFile() 
	{
		delete reader;
		delete audio;
	}

	File file;
	AudioFormatReader* reader = nullptr;
	AudioFormatManager formatManager;
	std::function<void()> fileLoadedCallback = nullptr;

	long bitDepth = -1;
	long numSamples = -1;
	int numChannels = -1;
	String filename;
	String fullPath;
	bool fileLoaded = false;
	int sampleRate = 0;

	MDArray<float>* audio = nullptr;

	void open(File file)
	{
		loadingAudiofile();
		fileLoaded = false;

		if (reader != nullptr) 
			delete reader;
		if (audio != nullptr)
			delete audio;

		this->file = file;
		reader = formatManager.createReaderFor(file); // [10]

		if (reader != nullptr)
		{
			numChannels = reader->numChannels;
			bitDepth = reader->bitsPerSample;
			numSamples = reader->lengthInSamples;
			filename = file.getFileName();
			fullPath = file.getFullPathName();
			sampleRate = reader->sampleRate;

			read();
		}
		
		fileLoaded = true;
		audiofileLoaded();
		if (fileLoadedCallback != nullptr)
			fileLoadedCallback();
	}

private:
	void init() {
		formatManager.registerBasicFormats();
	}

	void read()
	{
		fileLoaded = false;
		audio = new MDArray<float>(numChannels, numSamples);
		reader->read(audio->getPointers(), numChannels, 0, numSamples);
		stripSilence();
		fileLoaded = true;
	}

	void stripSilence()
	{
		bool startSampleFound = false;
		bool endSampleFound = false;
		int startSample = 0;
		int endSample = numSamples;
		
		named (outer)
		for (int i = 0; i < numSamples; i++) 
		{
			for (int c = 0; c < numChannels; c++)
			{
				const float v = abs((*audio)[c][i]);
				if ( v > 0.000001f )
				{
					startSample = i;
					break(outer);
				} 			
			}
		}

		named(outer_2)
		for (int i = numSamples - 1; i == 0; i--)
		{
			for (int c = 0; c < numChannels; c++)
			{
				const float v = abs((*audio)[c][i]);
				if (v > 0.000001f)
				{
					endSample = i;
					break(outer_2);
				}
			}
		}

		MDArray<float>* temp = new MDArray<float>(2, endSample - startSample);
		
		for (int c = 0; c < numChannels; c++)
		{
			for (int i = 0; i < endSample - startSample; i++) {
				(*temp)[c][i] = (*audio)[c][i + startSample];
			}
		}

		delete audio;
		audio = temp;
		numSamples = endSample - startSample-1;
	}

	virtual void loadingAudiofile() {}
	virtual void audiofileLoaded() {}
};