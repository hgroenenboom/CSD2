#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
	: af(true, true)
	, similarityChecker(&audioAnalyser),
	folderManagerC(folderManager),
	folderViewPort()
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
	
	analyseFoldersButton.setButtonText("analyse folders");
	addAndMakeVisible(&analyseFoldersButton);
	analyseFoldersButton.onClick = [&]() {
		DrawableText t;
		t.setBoundingBox(Parallelogram<float>(getLocalBounds().toFloat()));
		t.setText("Wait for analysis to complete...");
		t.setAlpha(0.5f);
		addAndMakeVisible(&t);
		audioAnalyser.analyseAudioFiles( folderManager.findAudioFilesInFolders() );
		analysisFilesText.setText("num files: " + std::to_string(audioAnalyser.getFeatureSets().size()));
	};
	addAndMakeVisible(&analysisFilesText);

	addAndMakeVisible(&folderViewPort);
	folderViewPort.setViewedComponent(&folderManagerC);

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
	const int border = 5;

	auto right = getLocalBounds();
	auto left = right.removeFromLeft( 0.5f*getWidth() );

	auto topLeft = left.removeFromTop(0.2f * getHeight());
	topLeft.reduce(border, border);
	auto analyseSpace = topLeft.removeFromBottom(30);
	analyseFoldersButton.setBounds( analyseSpace.removeFromLeft( 0.5f * analyseSpace.getWidth() ) );
	analysisFilesText.setBoundingBox( Parallelogram<float>( analyseSpace.toFloat() ) );
	folderViewPort.setBounds(topLeft);

	left.reduce(border, border);
	af.setBounds(left.removeFromTop(0.2f * getHeight()));
	similarityChecker.setBounds(left.removeFromTop(0.5f * getHeight()));
	searchButton.setBounds(left);

	right.reduce(border, border);
	const float height_per_box = right.getHeight() / (float)(numSimilarFiles);
	for (int i = 0; i < numSimilarFiles; i++) {
		similarAfs[i].setBounds(right.removeFromTop(height_per_box));
	}
}

void MainComponent::searchSimilarAudio() {
	if (af.fullPath != "") {
		auto sets = audioAnalyser.getFeatureSets();

		if (sets.size() != 0) {
			bool succes = false;
			AFFeatureSet set = audioAnalyser.analyseAudio(af.fullPath, &succes);

			if (succes) {
				auto indicesSortedBySimilarity = similarityChecker.sortBySimilarity(set, sets, 10);
				for (int i = 0; i < 10; i++) {
					similarAfs[i].open(sets[indicesSortedBySimilarity[i]].filePath);
				}
			}
		}
	}
}