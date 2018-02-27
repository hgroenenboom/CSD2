#include "polymidi.h"

PolyMidi::PolyMidi(int num_voices) 
	: numVoices(num_voices) 
{
	activeNotes = new int[numVoices];
	notes = new MidiNote*[numVoices];
	for(int i = 0; i < numVoices; i++) {
		notes[i] = new MidiNote();
		activeNotes[i] = -1;
	}
}

PolyMidi::~PolyMidi() {
	delete(activeNotes);
	for(int i = 0; i < numVoices; i++) {
		delete(notes[i]);
	}
	delete notes;
}
	
int PolyMidi::newNote(int vel, int pitch) {
	int index = insertNewNote(vel, pitch);
	cout << "newnote for midi note " << index << ", with vel " << vel << " and pitch " << pitch << endl;
	printAllNotes();
	return index;
}
	
void PolyMidi::insertNewActiveNote(int index){
	int temp[numVoices];
	for (int i = 0; i < numVoices; i++) {
		temp[i] = activeNotes[i];
	}
	for (int i = 0; i < numVoices; i++) {
		activeNotes[i] = temp[(i + 1) % numVoices];
	}
	activeNotes[numVoices - 1] = index;
}

int PolyMidi::insertNewNote(int vel, int pitch) {
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
			insertNewActiveNote(selectedNote);
		}
	}
	cout << "selected note: " << selectedNote << endl;
	return selectedNote;
}
	
int PolyMidi::findIndexForMidiPitch(int pitch) {
	for(int i = 0; i < numVoices; i++) {
		if(notes[i]->pitch == pitch) {
			return i;
		}
	}
	return -1;
}
	
void PolyMidi::printAllNotes() {
	cout << "\nNotes: \n";
	for(int i = 0; i < numVoices; i++) {
		cout << "Midinote: " << i << " : vel = " << notes[i]->vel << ", pitch = " << notes[i]->pitch << ", busy = " << notes[i]->busy << endl;
	}
	cout << "activeNotesArray: ";
	for(int i = 0; i < numVoices; i++) {
		cout << activeNotes[i] << ". ";
	}
	cout << endl;
}