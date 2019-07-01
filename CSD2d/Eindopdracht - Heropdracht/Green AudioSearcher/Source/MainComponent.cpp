#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
	: af(true, true)
	, similarityChecker(&audioAnalyser)
{
	similarAfs = new PlayableAudioFile[numSimilarFiles];

    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        setAudioChannels (2, 2);
    }
	
	openFolderButton.setButtonText("open folder");
	addAndMakeVisible(&openFolderButton);
	openFolderButton.onClick = [&]() {
		FileChooser chooser("Select folder to analyse", File());                                        // [7]
		if (chooser.browseForDirectory())                                    // [8]
		{
			File file(chooser.getResult());                                  // [9]
			folderManager.addFolder(0, file.getFullPathName().toStdString());

			setEnabled(false);
			folderManager.analyseFolders();
			setEnabled(true);
		}
	};

	for (int i = 0; i < numSimilarFiles; i++) {
		addAndMakeVisible(&similarAfs[i]);
	}
	addAndMakeVisible(&af);

	addAndMakeVisible(&searchButton);
	searchButton.onClick = [&]() {
		searchSimilarAudio();
	};
	searchButton.setButtonText("search");

	addAndMakeVisible(&similarityChecker);

	setSize(800, 600);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	af.prepareToPlay(samplesPerBlockExpected, sampleRate);
	for (int i = 0; i < numSimilarFiles; i++) {
		similarAfs[i].prepareToPlay(samplesPerBlockExpected, sampleRate);
	}
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
	af.getAudioBlock( *bufferToFill.buffer, bufferToFill.numSamples );
	for (int i = 0; i < numSimilarFiles; i++) {
		similarAfs[i].getAudioBlock( *bufferToFill.buffer, bufferToFill.numSamples);
	}
}

void MainComponent::releaseResources()
{
	delete[] similarAfs;
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
	auto right = getLocalBounds();
	auto left = right.removeFromLeft( 0.5f*getWidth() );
	auto topLeft = left.removeFromTop(0.2f * getHeight());
	openFolderButton.setBounds(topLeft);

	af.setBounds(left.removeFromTop(0.2f * getHeight()));
	similarityChecker.setBounds(left.removeFromTop(0.5f * getHeight()));
	searchButton.setBounds(left);


	const float height_per_box = right.getHeight() / (float)(numSimilarFiles);
	for (int i = 0; i < numSimilarFiles; i++) {
		similarAfs[i].setBounds(right.removeFromTop(height_per_box));
	}
}

void MainComponent::searchSimilarAudio() {
	auto sets = folderManager.getFeatureSets();

	bool succes = false;
	AFFeatureSet set = audioAnalyser.analyseAudio(af.fullPath, &succes);

	if (succes) {
		auto indicesSortedBySimilarity = similarityChecker.sortBySimilarity(set, sets, 10);
		for (int i = 0; i < 10; i++) {
			similarAfs[i].open( sets[indicesSortedBySimilarity[i]].filePath );
		}
	}
}