/*
  ==============================================================================

    FolderManager.cpp
    Created: 13 Jul 2019 12:39:45am
    Author:  HAROL

  ==============================================================================
*/

#include "FolderManager.h"

void Folder::openFolder() {
	FileChooser chooser("Select folder to analyse", File());
	if (chooser.browseForDirectory())                                    // [8]
	{
		File file(chooser.getResult());                                  // [9]
		path = file.getFullPathName().toStdString();
		enabled = true;
	}
}

FolderManager::FolderManager(int numFolders)
	: folders(numFolders)
{
}

// add folder to Folder vector
void FolderManager::addFolder(int slot, std::string path) {
	folders[slot].path = path;
	folders[slot].enabled = true;
}
void FolderManager::addFolder(std::string path) {
	bool foundEmpty = false;
	for (int i = 0; i < folders.size(); i++) {
		if (!foundEmpty && folders[i].path == "") {
			foundEmpty = true;
			addFolder(i, path);
		}
	}
}

void FolderManager::removeFolder(int slot) {
	folders[slot].path = "";
	folders[slot].enabled = false;
}

// disable or enable a folder for analysis
void FolderManager::toggleFolder(int slot, bool state) {
	folders[slot].enabled = state;
}

std::vector<std::string> FolderManager::findAudioFilesInFolders() {
	// check which folders should be analyzed
	std::vector<Folder> foldersToAnalyse;
	for (int i = 0; i < folders.size(); i++) {
		if (folders[i].enabled == true && folders[i].path != "") {
			foldersToAnalyse.push_back(folders[i]);
		}
	}

	// find all files which should be analyzed
	std::vector<std::string> filesToAnalyse;
	for (Folder& f : foldersToAnalyse) {
		// find all wave files
		DirectoryIterator d_iter(File(f.path), true, "*.wav");
		while (d_iter.next())
		{
			std::string newFile = d_iter.getFile().getFullPathName().toStdString();

			// dont add duplicates
			if (!findInVector<std::string>(filesToAnalyse, newFile).first) {
				filesToAnalyse.push_back(newFile);
			};
		}
	}

	return filesToAnalyse;
}

void FolderManager::openFolder(int slot) {
	folders[slot].openFolder();
}

String FolderManager::getFolderPath(int slot) {
	return folders[slot].path;
}

template < typename T>
std::pair<bool, int > FolderManager::findInVector(const std::vector<T>  & vecOfElements, const T  & element)
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