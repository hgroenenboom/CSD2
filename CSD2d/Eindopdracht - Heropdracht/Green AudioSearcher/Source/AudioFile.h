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
	AudioFile() 
	{
		formatManager.registerBasicFormats();
	}

	~AudioFile() 
	{
	}

	void read()
	{
		audio = new MDArray<float>(numChannels, numSamples);
		reader->read( audio->getPointers(), numChannels, 0, numSamples );
		stripSilence();
	}

	File file;
	AudioFormatReader* reader = nullptr;
	AudioFormatManager formatManager;

	long bitDepth = -1;
	long numSamples = -1;
	int numChannels = -1;
	String filename;

	MDArray<float>* audio;
protected:
	void open(File file)
	{
		this->file = file;
		reader = formatManager.createReaderFor(file); // [10]

		if (reader != nullptr)
		{
			numChannels = reader->numChannels;
			bitDepth = reader->bitsPerSample;
			numSamples = reader->lengthInSamples;
			filename = file.getFileName();

			read();
		}
	}

private:
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
		numSamples = endSample - startSample;
	}
};