#ifndef _POLYMIDI_H_
#define _POLYMIDI_H_

#include <iostream>
#include <cstring>

using namespace std;

// MidiNote: MidiNote saves the information of a single midi note.
class MidiNote
{
public:
	int pitch = 0;
	int vel = 0;
	bool busy = false;
};

// PolyMidi: PolyMidi saves multiple midi notes and handles polyphonic voice routing.
// 		PolyMidi uses voice stealing and has a midinote array for the given amount of voices.
class PolyMidi
{
public:
	PolyMidi(int num_voices=8);
	~PolyMidi();
	
	// Process new midi note and output information about the midi notes.
	int newNote(int vel, int pitch);
	// Output the corresponding midi note for a midi pitch value. (outputs -1 if not found)
	int findIndexForMidiPitch(int pitch);
	// Print a list of all midi notes, with their corresponding states.
	// Used for debugging and console usage.
	void printAllNotes();
	
	int numVoices = 8;
	MidiNote** notes;
	// the active notes array will save the order in which the notes are called.
	// This enables voice stealing for the oldest notes.
	int* activeNotes;
	
private:
	// update the active note array.
	void updateActiveNotes(int index);
	// Process new midi note.
	int processMidiNote(int vel, int pitch);
};

#endif