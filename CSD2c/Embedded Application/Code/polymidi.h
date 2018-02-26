#ifndef _POLYMIDI_H_
#define _POLYMIDI_H_

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
	PolyMidi(int num_voices=8) 
		: numVoices(num_voices) 
	{
		activeNotes = new int[numVoices];
		notes = new MidiNote*[numVoices];
		for(int i = 0; i < numVoices; i++) {
			notes[i] = new MidiNote();
		}
	}
	
	int newNote(int vel, int pitch) {
		int temp = insertNewNote(vel, pitch);
		cout << "newnote = " << temp << ", with vel " << vel << " and pitch " << pitch << endl;
		printAllNotes();
		return temp;
	}
	
	void insertNewActiveNote(int index){
		int temp[numVoices];
		memcpy(temp, activeNotes, numVoices * sizeof(int));
		for (int i = 0; i < numVoices; i++) {
			activeNotes[i] = temp[(i + 1) % numVoices];
		}
		activeNotes[numVoices] = index;
	}

	int insertNewNote(int vel, int pitch) {
		int selectedNote = 0;
		// turning off a note, when velocity is 0
		if(vel == 0) {
			for(int i = 0; i < numVoices; i++) {
				if(notes[i]->pitch == pitch) {
					notes[i]->vel = 0;
					notes[i]->busy = false;
					selectedNote = i;
				}
			}
		}
		// if velocity is greater then zero, try to find a new free note, if there is no free note,
		// overwrite the oldest selected note.
		else if(vel > 0) {
			bool foundFreeNote = false;
			int count = 0;
			while(count < 8 && foundFreeNote != true) {
				if(notes[count]->busy != true) {
					notes[count]->pitch = pitch;
					notes[count]->vel = vel;
					notes[count]->busy = true;
					insertNewActiveNote(count);
					foundFreeNote = true;
					selectedNote = count;
					break;
				}
				count++;
			}
			if (!foundFreeNote) {
				int oldestNote = activeNotes[0];
				selectedNote = oldestNote;
				notes[oldestNote]->vel = 0;
				notes[oldestNote]->pitch = pitch;
				notes[oldestNote]->vel = vel;
			}
		}
		
		return selectedNote;
	}
	
	int findIndexForMidiPitch(int pitch) {
		for(int i = 0; i < numVoices; i++) {
			if(notes[i]->pitch == pitch) {
				return i;
			}
		}
		return -1;
	}
	
	void printAllNotes() {
		cout << "Notes: \n";
		for(int i = 0; i < numVoices; i++) {
			cout << i << " : vel = " << notes[i]->vel << ", pitch = " << notes[i]->pitch << endl;
		}
	}
	
	int numVoices = 8;
	int* activeNotes;
	MidiNote** notes;
};

#endif