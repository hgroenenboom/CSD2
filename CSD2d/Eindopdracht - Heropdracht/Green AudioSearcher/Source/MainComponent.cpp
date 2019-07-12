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
	
	audioAnalyser.fileAnalysed = [&]() {
		numFiles = audioAnalyser.getFeatureSets().size();
	};
	audioAnalyser.filesAnalysed = [&]() {
		isAnalysing = false;
	};

	analyseFoldersButton.setButtonText("analyse folders");
	analyseFoldersButton.setColour(TextButton::buttonColourId, Colours::lightgreen.darker(0.6f));
	addAndMakeVisible(&analyseFoldersButton);
	analyseFoldersButton.onClick = [&]() {
		analyseFoldersButton.setButtonText("analysing folders...");
		isAnalysing = true;
		analyseFoldersButton.setEnabled(false);
		auto files = folderManager.findAudioFilesInFolders();
		analyseFoldersButton.setButtonText("analysing files: " + std::to_string(files.size()));
		audioAnalyser.analyseAudioFiles(files);
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
	startTimerHz(30);
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
    g.fillAll ( Colours::lightgrey/*getLookAndFeel().findColour (ResizableWindow::backgroundColourId)*/);

	g.setColour(Colours::purple.withSaturation(0.1f).withAlpha(0.6f).withBrightness(0.3f));
	for (auto r : rectanglesToFill) {
		g.fillRect(r);
	}
}

void MainComponent::resized()
{
	const int border = 5;
	rectanglesToFill.clear();

	auto right = getLocalBounds();
	auto left = right.removeFromLeft( 0.5f*getWidth() );

	auto topLeft = left.removeFromTop(0.2f * getHeight());
	topLeft.reduce(border, border);
	rectanglesToFill.push_back(topLeft);

	auto analyseSpace = topLeft.removeFromBottom(30);
	analyseFoldersButton.setBounds( analyseSpace.removeFromLeft( 0.5f * analyseSpace.getWidth() ) );
	analysisFilesText.setBoundingBox( Parallelogram<float>( analyseSpace.reduced(border, border).toFloat() ) );
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

void MainComponent::timerCallback() {
	if (isAnalysing) {
		analyseFoldersButton.setButtonText("analysing files: " + std::to_string(audioAnalyser.numFilesAnalysed) + " / " + std::to_string(audioAnalyser.filesToAnalyse));
		analysisFilesText.setText("num files: " + std::to_string(audioAnalyser.getFeatureSets().size()));
	}
	else {
		analyseFoldersButton.setButtonText("analyse folders");
		analysisFilesText.setText("num files: " + std::to_string(audioAnalyser.getFeatureSets().size()));
		analyseFoldersButton.setEnabled(true);
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
					similarAfs[i].read();
				}
			}
		}
	}
}