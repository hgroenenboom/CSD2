/*
  ==============================================================================

    PlayableAudiofile.cpp
    Created: 13 Jul 2019 12:40:02am
    Author:  HAROL

  ==============================================================================
*/

#include "PlayableAudiofile.h"

PlayableAudioFile::PlayableAudioFile(bool enableInwardDrag, bool enableOutwardDrag)
	: inwardDragEnabled(enableInwardDrag)
	, outwardDragEnabled(enableOutwardDrag)
{
	addAndMakeVisible(&playButton);
	playButton.setButtonText("play audiofile");
	playButton.onClick = [&] { playStopButtonClicked(); };
	playButton.setColour(TextButton::buttonColourId, kleuren.purpleColour);

	addAndMakeVisible(&openButton);
	openButton.setButtonText("open new audiofile");
	openButton.onClick = [&] { openButtonClicked(); };
	openButton.setColour(TextButton::buttonColourId, kleuren.purpleColour);

	fileLoadedCallback = [&]() {
		textField.setText(fullPath);
	};
	addAndMakeVisible(&textField);
}

void PlayableAudioFile::paint(Graphics&)  {
	if (fileDraggedOver) {
		openButton.setColour(TextButton::ColourIds::buttonColourId, Colours::aquamarine);
	}
	else {
		openButton.setColour(TextButton::ColourIds::buttonColourId, Colours::black);
	}
}

void PlayableAudioFile::resized()  {
	auto s = getLocalBounds();
	s.reduce(2, 2);
	auto s_b = s.removeFromBottom(30);

	playButton.setBounds(s.removeFromLeft((int)(0.5f * s.getWidth())));
	openButton.setBounds(s);
	textField.setBounds(s_b);
}

void PlayableAudioFile::openButtonClicked() {
	FileChooser chooser("Select a Wave file to play...", File());                                        // [7]

	if (chooser.browseForFileToOpen())                                    // [8]
	{
		File file(chooser.getResult());                                  // [9]
		openFile(file);
	}
}

void PlayableAudioFile::openFile(File file) {
	open(file.getFullPathName().toStdString());
	read();
	position = 0.0f;
}

void PlayableAudioFile::releaseResources() {
}

void PlayableAudioFile::prepareToPlay(int samplesPerBlock, int samplerate) {
}

void PlayableAudioFile::getAudioBlock(AudioBuffer<float> buffer, int bufSize) {
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

void PlayableAudioFile::playStop() {
	position = 0.0f;
	isPlaying = !isPlaying;
}

void PlayableAudioFile::playStopButtonClicked() {
	playStop();
	if (isPlaying) {
		playButton.setButtonText("stop audiofile");
	}
	else {
		playButton.setButtonText("play audiofile");
	}
}

void PlayableAudioFile::loadingAudiofile() 
{
	this->setEnabled(false);
}

void PlayableAudioFile::audiofileLoaded() 
{
	this->setEnabled(true);
}

bool PlayableAudioFile::isInterestedInFileDrag(const StringArray &files)  {
	return inwardDragEnabled;
}
void PlayableAudioFile::fileDragEnter(const StringArray &files, int x, int y)  {
	if (files[0].endsWith(".wav")) {
		fileDraggedOver = true;
		repaint();
	}
}
void PlayableAudioFile::fileDragExit(const StringArray &files)  {
	fileDraggedOver = false;
	repaint();
}
void PlayableAudioFile::filesDropped(const StringArray &files, int x, int y)  {
	if (files[0].endsWith(".wav")) {
		openFile(files[0]);
		fileDraggedOver = false;
		repaint();
	}
}