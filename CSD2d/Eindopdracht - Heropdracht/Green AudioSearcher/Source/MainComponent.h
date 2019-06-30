#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayableAudiofile.h"
#include "FolderManager.h"
#include "AFFeatureSet.h"
#include "SimilarityChecker.h"

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

private:
    //==============================================================================
    // Your private member variables go here...
	
	std::string pars[3] = { "a", "b", "c" };
	float vals[3] = { 0.24124f, 0.23535f, 0.7623f };

	PlayableAudioFile af;
	PlayableAudioFile* similarAfs;
	FolderManager folderManager;
	AFFeatureSet set;
	SimilarityChecker simChecker;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
