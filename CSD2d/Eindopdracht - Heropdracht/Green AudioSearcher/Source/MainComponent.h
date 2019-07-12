#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayableAudiofile.h"
#include "FolderManager.h"
#include "FolderManagerComponent.h"
#include "AFFeatureSet.h"
#include "SimilarityChecker.h"
#include "AudioAnalyzer.h"
#include "Colours.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, public Timer
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
	void timerCallback() override;

	void searchSimilarAudio();
private:
    //==============================================================================
    // Your private member variables go here...
	AudioAnalyzer audioAnalyser;
	PlayableAudioFile af;
	const int numSimilarFiles = 10;
	PlayableAudioFile* similarAfs;
	SimilarityChecker similarityChecker;

	TextButton analyseFoldersButton;
	DrawableText analysisFilesText;

	Viewport folderViewPort;
	FolderManager folderManager;
	FolderManagerComponent folderManagerC;
	TextButton searchButton;
	
	std::vector<Rectangle<int>> rectanglesToFill;

	int numFiles = 0;
	int numFilesAnalysed = 0;
	int numFilesToAnalyse = 0;
	bool isAnalysing = false;

	Green_AudioSearcher::Kleur kleuren;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
