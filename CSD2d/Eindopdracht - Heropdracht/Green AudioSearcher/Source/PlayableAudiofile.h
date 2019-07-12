#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LoadableAudioFile.h"
#include "Colours.h"

class PlayableAudioFile : public Component, public LoadableAudioFile, public FileDragAndDropTarget {
public:
	PlayableAudioFile(bool enableInwardDrag = false, bool enableOutwardDrag = true);

	// Component methods
	void paint(Graphics&) override;
	void resized() override;

	void openButtonClicked();
	void openFile(File file);

	// UNUSED
	void releaseResources();
	void prepareToPlay(int samplesPerBlock, int samplerate);

	void getAudioBlock(AudioBuffer<float> buffer, int bufSize);
	void playStop();
private:
	TextButton playButton;
	TextButton openButton;
	TextEditor textField;

	float position = 0;
	bool isPlaying = false;
	bool inwardDragEnabled = false;
	bool outwardDragEnabled = false;

	Green_AudioSearcher::Kleur kleuren;

	void playStopButtonClicked();

	virtual void loadingAudiofile() override;
	virtual void audiofileLoaded() override;

	// Highlighting droppable audiofiles.
	bool fileDraggedOver = false;
	bool isInterestedInFileDrag(const StringArray &files) override;
	void fileDragEnter(const StringArray &files, int x, int y) override;
	void fileDragExit(const StringArray &files) override;
	void filesDropped(const StringArray &files, int x, int y) override;
};