/*
  ==============================================================================

    FolderManager.h
    Created: 30 Jun 2019 1:02:24pm
    Author:  HAROL

  ==============================================================================
*/

#pragma once
#include <string>
#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioAnalyzer.h"

// Helper function for easily finding elements inside vectors.
template < typename T>
std::pair<bool, int > findInVector(const std::vector<T>  & vecOfElements, const T  & element)
{
	std::pair<bool, int > result;

	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);

	if (it != vecOfElements.end())
	{
		result.second = distance(vecOfElements.begin(), it);
		result.first = true;
	}
	else
	{
		result.first = false;
		result.second = -1;
	}

	return result;
}

class Folder {
public:
	std::string path = "";
	bool enabled = true;
};

class FolderManager {
public:
	FolderManager()
		: folders(10)
	{
	}

	void addFolder(int slot, std::string path) {
		folders[slot].path = path;
	}
	void addFolder(std::string path) {
		bool foundEmpty = false;
		for (int i = 0; i < folders.size(); i++) {
			if (!foundEmpty && folders[i].path == "") {
				foundEmpty = true;
				folders[i].path = path;
			}
		}
	}

	void removeFolder(int slot) {
		folders[slot].path = "";
	}

	void toggleFolder(int slot, bool state) {
		folders[slot].enabled = state;
	}

	void analyseFolders() {
		// temporarily add folders here
		addFolder(0, "C:\\Users\\HAROL\\Dropbox\\Muziek\\Samples\\Created\\DrumKits");
		addFolder("C:\\Users\\HAROL\\Dropbox\\Muziek\\Samples\\Created\\DrumKits");

		// check which folders should be analyzed
		std::vector<Folder> foldersToAnalyse;
		for (int i = 0; i < folders.size(); i++) {
			if (folders[i].enabled = true && folders[i].path != "") {
				foldersToAnalyse.push_back(folders[i]);
			}
		}

		// find all files which should be analyzed
		std::vector<File> filesToAnalyse;
		for (Folder& f : foldersToAnalyse) {
			// find all wave files
			DirectoryIterator d_iter( File(f.path), true, "*.wav");
			while (d_iter.next())
			{
				File newFile = d_iter.getFile();

				// dont add duplicates
				if (!findInVector<File>(filesToAnalyse, newFile).first) {
					filesToAnalyse.push_back(newFile);
				};
			}
		}

		featureSets.clear();
		for (File& f : filesToAnalyse) {
			AudioFile af(f);
			bool succes = false;
			AFFeatureSet set = audioAnalyzer.analyseAudio(&af, &succes);
			if (succes) {
				featureSets.push_back(set);
			}
		}
	}

private:
	std::vector<Folder> folders;
	std::vector<AFFeatureSet> featureSets;
	AudioAnalyzer audioAnalyzer;

	//bool databaseExists = false;
	//bool isDatabaseLoaded = false;
	//std::string databasePath = "database.json";
	//File databaseFile;
};

