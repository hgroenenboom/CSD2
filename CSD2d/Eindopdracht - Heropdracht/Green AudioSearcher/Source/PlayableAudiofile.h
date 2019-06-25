#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioFile.h"

class PlayableAudioFile : public Component, public AudioFile {
public:
	PlayableAudioFile()
	{
		addAndMakeVisible(&playButton);
		playButton.setButtonText("play audiofile");
		playButton.onClick = [&] { playStopButtonClicked(); };

		addAndMakeVisible(&openButton);
		openButton.setButtonText("open new audiofile");
		openButton.onClick = [&] { openButtonClicked(); };
	}

	void paint(Graphics&) override {}
	void resized() override {
		auto s = getLocalBounds();
		s.reduce(6, 6);

		playButton.setBounds( s.removeFromLeft( (int)( 0.5f * s.getWidth() ) ) );
		openButton.setBounds( s );
	}

	void openButtonClicked() {
		FileChooser chooser("Select a Wave file to play...", File());                                        // [7]

		if (chooser.browseForFileToOpen())                                    // [8]
		{
			File file(chooser.getResult());                                  // [9]
			openFile(file);
		}
	}

	void openFile(File file) {
		open(file);
		position = 0.0f;
	}

	void releaseResources() {
	}

	void prepareToPlay(int samplesPerBlock, int samplerate) {
	}

	void getAudioBlock(AudioBuffer<float> buffer, int bufSize) {
		if (isPlaying) {
			float** f = buffer.getArrayOfWritePointers();
			const float startPosition = position;
			const float speed = 1.0f; // should calculate via samplerate

			for (int c = 0; c < numChannels; c++) {
				position = startPosition;
				for (int i = 0; i < bufSize; i++) {
					const float w = position - floor(position);
					const int pos1 = floor(position);
					const int pos2 = ceil(position);

					f[c][i] = (1.0f - w) * (*audio)[c][pos1] + w * (*audio)[c][pos2];

					position += speed;
					if (position > numSamples) {
						position -= numSamples;
					}
				}
			}
		}
	}

	void playStop() {
		position = 0.0f;
		isPlaying = !isPlaying;
	}
private:
	TextButton playButton;
	TextButton openButton;

	float position = 0;
	bool isPlaying = false;

	void playStopButtonClicked() {
		playStop();
	}
};