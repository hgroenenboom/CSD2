#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayableAudiofile.h"
#include "FolderManager.h"
#include "FolderManagerComponent.h"
#include "AFFeatureSet.h"
#include "SimilarityChecker.h"
#include "AudioAnalyzer.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

	void searchSimilarAudio();
private:
    //==============================================================================
    // Your private member variables go here...
	AudioAnalyzer audioAnalyser;
	PlayableAudioFile af;
	const int numSimilarFiles = 10;
	PlayableAudioFile* similarAfs;
	FolderManager folderManager;
	SimilarityChecker similarityChecker;

	TextButton analyseFoldersButton;
	DrawableText analysisFilesText;

	Viewport folderViewPort;
	FolderManagerComponent folderManagerC;
	TextButton searchButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
