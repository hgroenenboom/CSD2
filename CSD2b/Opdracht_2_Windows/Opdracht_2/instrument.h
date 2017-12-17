	#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <iostream>
#include <string>

using namespace std;

class Instrument {
	public:
		Instrument();
		// ~Instrument();
		
		void makeSound(int midi);
		string midiToNote(int midi);
		
		void printInfo();
		
		string getName();
		string setName(string newName);
		string getType();
		string setType(string newType);
		string getSound();
		string setSound(string newSound);
		int getLowestNote();
		int setLowestNote(int newLowestNote);
		int getNoteRange();
		int setNoteRange(int newNoteRange);
	private:
		string name = "default instrument";
		string type = "default";
		string sound = "empty";
		int lowestNote = 0;
		int noteRange = 0;
		string noteNames[12] = {"C", "Cis", "D", "Dis", "E", "F", "Fis", "G", "Gis", "A", "Ais", "B"};
};

#endif