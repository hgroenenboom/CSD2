#include <iostream>
#include <string>

class Instrument
{
public:
	Instrument(std::string newString);
	void makeSound();
	void playTone();
	void makeMultipleSounds(int n, int time);
	void newTonalScale();

	void changeArray(std::string inputArray[], int remergePoint);
private:
	std::string sound = "Empty sound";

	std::string TONALSCALE[7] = {"T", "T", "S", "T", "T", "T", "S"};
	std::string NOTES[12] = {"C", "Cis", "D", "Dis", "E", "F", "Fis", "G", "Gis", "A", "Ais", "B"};
	
	std::string currentTonalScale[7] = {};
	std::string currentNoteScale[7] = {};
	int currentPitch = 0;

	std::string currentNote = "empty";
};