#include "polymidi.h"

PolyMidi::PolyMidi(int num_voices) 
	: numVoices(num_voices) 
{
	// create objects for saving note information.
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
	// process the input note and return the resulting midi note index.
	int index = processMidiNote(vel, pitch);
	
	// print information about the midi notes.
	cout << "\tnewnote for midi note " << index << ", with vel " << vel << " and pitch " << pitch << endl;
	return index;
}
	
void PolyMidi::updateActiveNotes(int new_note){
	// shift all values in activeNotes to the left, starting from the index of new_note in activeNotes.
	// Then insert new_note into the last value of activeNotes.

	// find the index where new_note is lastly played, and save this index.
	// If it's not yet played, it will stay -1.
	int startShift = -1;
	for (int i = 0; i < numVoices; i++) {
		// if the selected note is the new_note, start the shift
		if(startShift == -1 && activeNotes[i] == new_note) {
		startShift = i;
		}
	}
	
	// Start shifting activeNotes values to the left when the loop is past the found index.
	for (int i = 0; i < numVoices - 1; i++) {
		// if the new_note has not yet been found, simply do nothing

		// shift the notes left, thus overwriting the new_note value in the array.
		if(i >= startShift) {
			activeNotes[i] = activeNotes[(i + 1) % numVoices];
		}
	}

	// set last active note to be new_note.
	activeNotes[numVoices - 1] = new_note;
}

int PolyMidi::processMidiNote(int vel, int pitch) {
	// create an empty temporary integer, which will be overwritten with the index 
	// of the selected midi note.
	int selectedNote = -1;
	
	// when velocity is 0, turn off the corresponding note. 
	if(vel == 0) {
		for(int i = 0; i < numVoices; i++) {
			if(notes[i]->pitch == pitch) {
				notes[i]->vel = 0;
				notes[i]->busy = false;
				selectedNote = i;
			}
		}
	}
	// if velocity is greater then 0, try to find a new free note.
	// 		if there is no free note, overwrite the oldest selected note.
	else if(vel > 0) {
		bool foundFreeNote = false;
		int count = 0;
		// Try to find a free midi note:
		while(count < 8 && foundFreeNote != true) {
			if(notes[count]->busy != true) {
				// process note information
				notes[count]->pitch = pitch;
				notes[count]->vel = vel;
				notes[count]->busy = true;
				
				// break loop and save index.
				updateActiveNotes(count);
				foundFreeNote = true;
				selectedNote = count;
				break;
			}
			count++;
		}
		// if no free note is found, steal the oldest voice.
		if (!foundFreeNote) {
			// get oldest note
			int oldestNote = activeNotes[0];
			selectedNote = oldestNote;
			
			// process note information
			notes[oldestNote]->vel = 0;
			notes[oldestNote]->pitch = pitch;
			notes[oldestNote]->vel = vel;
		
			updateActiveNotes(selectedNote);
		}
	}
	
	// Making sure there's no bogus output.
	if(selectedNote < 0) {
		cout << "Error: Something went wrong while processing the notes." << endl;
		return 0;
	}
	
	return selectedNote;
}

int PolyMidi::findIndexForMidiPitch(int pitch) {
	// try to find a midi note with the same pitch, else return -1.
	for(int i = 0; i < numVoices; i++) {
		if(notes[i]->pitch == pitch) {
			return i;
		}
	}
	return -1;
}
	
void PolyMidi::printAllNotes() {
	// print midi information of all notes.
	cout << "\n\tNotes: \n";
	for(int i = 0; i < numVoices; i++) {
		cout << "\t\tMidinote: " << i << " : vel = " << notes[i]->vel << ", pitch = " << notes[i]->pitch << ", busy = " << notes[i]->busy << endl;
	}
	
	// print information about the order in which the notes are played.
	cout << "\tactiveNotesArray: ";
	for(int i = 0; i < numVoices; i++) {
		cout << activeNotes[i] << ". ";
	}
	cout << endl;
}