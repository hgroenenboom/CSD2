#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LoadableAudioFile.h"

class PlayableAudioFile : public Component, public LoadableAudioFile, public FileDragAndDropTarget {
public:
	PlayableAudioFile(bool enableInwardDrag = false, bool enableOutwardDrag = true)
		: inwardDragEnabled(enableInwardDrag)
		, outwardDragEnabled(enableOutwardDrag)
	{
		addAndMakeVisible(&playButton);
		playButton.setButtonText("play audiofile");
		playButton.onClick = [&] { playStopButtonClicked(); };

		addAndMakeVisible(&openButton);
		openButton.setButtonText("open new audiofile");
		openButton.onClick = [&] { openButtonClicked(); };

		fileLoadedCallback = [&]() {
			textField.setText( fullPath );
		};
		addAndMakeVisible(&textField);
	}

	void paint(Graphics&) override {
		if (fileDraggedOver) {
			openButton.setColour(TextButton::ColourIds::buttonColourId, Colours::aquamarine);
		}
		else {
			openButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black);
		}
	}

	void resized() override {
		auto s = getLocalBounds();
		s.reduce(2, 2);
		auto s_b = s.removeFromBottom( 30 );

		playButton.setBounds( s.removeFromLeft( (int)( 0.5f * s.getWidth() ) ) );
		openButton.setBounds( s );
		textField.setBounds(s_b);
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
		open(file.getFullPathName().toStdString());
		read();
		position = 0.0f;
	}

	void releaseResources() {
	}

	void prepareToPlay(int samplesPerBlock, int samplerate) {
	}

	void getAudioBlock(AudioBuffer<float> buffer, int bufSize) {
		if (isPlaying && fileLoaded) {
			float** f = buffer.getArrayOfWritePointers();
			const float startPosition = position;
			const float speed = 1.0f; // should calculate via samplerate

			for (int c = 0; c < numChannels; c++) {
				position = startPosition;
				for (int i = 0; i < bufSize; i++) {
					const float w = position - floor(position);
					const int pos1 = floor(position);
					const int pos2 = ceil(position);

					f[c][i] += (1.0f - w) * audio[c][pos1] + w * audio[c][pos2];

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
	TextEditor textField;

	float position = 0;
	bool isPlaying = false;
	bool inwardDragEnabled = false;
	bool outwardDragEnabled = false;

	void playStopButtonClicked() {
		playStop();
		if (isPlaying) {
			playButton.setButtonText("stop audiofile");
		}
		else {
			playButton.setButtonText("play audiofile");
		}
	}

	virtual void loadingAudiofile() override
	{
		this->setEnabled(false);
	}
	
	virtual void audiofileLoaded() override 
	{
		this->setEnabled(true);
	}

	bool fileDraggedOver = false;
	bool isInterestedInFileDrag(const StringArray &files) override {
		return inwardDragEnabled;
	}
	void fileDragEnter(const StringArray &files, int x, int y) override {
		if (files[0].endsWith(".wav")) {
			fileDraggedOver = true;
			repaint();
		}
	}
	void fileDragExit(const StringArray &files) override {
		fileDraggedOver = false;
		repaint();
	}
	void filesDropped(const StringArray &files, int x, int y) override {
		if (files[0].endsWith(".wav")) {
			openFile(files[0]);
			fileDraggedOver = false;
			repaint();
		}
	}
};