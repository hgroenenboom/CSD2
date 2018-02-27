#ifndef SYNTH_H
#define SYNTH_H

#include "oscillator.h"
#include "polymidi.h"

using namespace std;

class Synth{
public:
    Synth(WaveTable* wt);

    void process(float* buffer, int buffersize);
	// Process add is dangerous and should only be used if Synth is part of a polySynth
	void processAdd(float* buffer, int buffersize);
	
    void setSamplerate(int sr);
	void setAmplitude(float amp);
	void setWaveType(int type);
	void setMidiNote(float vel, float pitch);
	void setMidiPitch(float midiP);

    int oscType = 1;
	float midiNote = 32;

	WaveTable* wt;
	MVOscillator mvOscillator;
};

class PolySynth{
public:
	PolySynth(WaveTable* wt, int numSynths=8);
	~PolySynth();
	
	// Zero fill the buffer and add the buffer values for all internal synths to the buffer.
	void process(float* buffer, int buffersize);
	
	// Insert a new midi note into the polyMidi object 
	// and route the midi to the corresponding synth object.
	void newNote(int vel, int pitch);
	// Set the wave type of all synth objects.
	void setWaveType(int choice);
	
	int numSynths = 8;
	float normAmp = 1.0f;
	Synth** synths;
	WaveTable* wt;
	
	PolyMidi polyMidi;
};

#endif
