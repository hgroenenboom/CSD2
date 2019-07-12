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

// simple folder class
class Folder {
public:
	std::string path = "";
	bool enabled = false;

	void openFolder();
};

/****************************************************************/
/****************************************************************/

class FolderManager {
public:
	FolderManager(int numFolders = 10);

	// add folder to Folder vector
	void addFolder(int slot, std::string path);
	void addFolder(std::string path);

	void removeFolder(int slot);

	// disable or enable a folder for analysis
	void toggleFolder(int slot, bool state);

	void openFolder(int slot);

	String getFolderPath(int slot);

	// get a vector of paths of all audiofiles in the selected folders
	std::vector<std::string> findAudioFilesInFolders();
private:
	std::vector<Folder> folders;

	// Helper function for easily finding elements inside vectors.
	// returns whether an element is found, and if it's found, on which index in the vector.
	template < typename T>
	std::pair<bool, int > findInVector(const std::vector<T>  & vecOfElements, const T  & element);

};

