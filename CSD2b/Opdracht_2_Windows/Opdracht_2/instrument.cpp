#include "instrument.h"
#include <sstream> 

Instrument::Instrument(){
	// cout << "Instrument constructor" << endl;
}

void Instrument::makeSound(int midi) {
	if(midi < lowestNote || midi > lowestNote+noteRange){
		cout << "///Instrumental error for " << name << " on note " << midiToNote(midi) << ": Input note is out of range..." << endl;
	}else{
	cout << "AUDIO | " << name << ": " << sound << " | at note: " << midiToNote(midi) << endl;
	}
}

string Instrument::midiToNote(int midi){
	stringstream ss;
	ss << midi/12;
	string octave = ss.str();
	return(noteNames[midi%12] + octave);
}

void Instrument::printInfo(){
	cout << "---------------------------------------" << endl;
	cout << "Info for instrument: " << name << endl;
	cout << "Instrument type: " << type << endl;
	cout << "Note range: " << midiToNote(lowestNote) << "(" << lowestNote << ") to " << midiToNote(lowestNote + noteRange) << "(" << lowestNote + noteRange << ")" << endl;
	cout << endl << endl;
}

string Instrument::getName(){
	return(name); }

string Instrument::setName(string newName){
	name = newName; }

string Instrument::getType(){
	return(type); }

string Instrument::setType(string newType){
	type = newType; }

string Instrument::getSound(){
	return(sound); }

string Instrument::setSound(string newSound){
	sound = newSound; }

int Instrument::getLowestNote(){
	return(lowestNote); }

int Instrument::setLowestNote(int newLowestNote){
	lowestNote = newLowestNote; }

int Instrument::getNoteRange(){
	return(noteRange); }

int Instrument::setNoteRange(int newNoteRange){
	noteRange = newNoteRange; }