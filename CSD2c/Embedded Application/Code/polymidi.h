#ifndef _POLYMIDI_H_
#define _POLYMIDI_H_

#include <iostream>
#include <cstring>

using namespace std;

class MidiNote
{
public:
	int pitch = 0;
	int vel = 0;
	bool busy = false;
};

class PolyMidi
{
public:
	PolyMidi(int num_voices=8);
	~PolyMidi();
	
	// Create new midi note from incoming velocity and pitch. 
	int newNote(int vel, int pitch);
	// Find the corresponding note for a midi pitch value. (outputs -1 if not found)
	int findIndexForMidiPitch(int pitch);
	// Print a list of all midi notes, with their corresponding states.
	void printAllNotes();
	
	int numVoices = 8;
	int* activeNotes;
	MidiNote** notes;
	
private:
	void insertNewActiveNote(int index);
	int insertNewNote(int vel, int pitch);
};

#endif