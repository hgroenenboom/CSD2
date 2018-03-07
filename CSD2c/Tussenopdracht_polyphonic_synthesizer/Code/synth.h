#ifndef SYNTH_H
#define SYNTH_H

#include "oscillator.h"
#include "polymidi.h"

using namespace std;

// Synth: A monophonic synthesizer class. This class has a Multivoiced Oscillator.
// 		TODO: Modify the synth to use envelopes.
class Synth{
public:
    Synth(WaveTable* wt);

	// Process a input buffer.
    void process(float* buffer, int buffersize);
	// Process add should only be used if Synth is part of a polySynth
	void processAdd(float* buffer, int buffersize);
	
    void setSamplerate(int sr);
	void setAmplitude(float amp);
	void setWaveType(int type);
	void setMidiNote(float vel, float pitch);
	void setMidiPitch(float midiP);

    int oscType = 1;
	float midiPitch = 32;

	WaveTable* wt;
	MVOscillator mvOscillator;
};

// PolySynth: The polysynth has multiple synth objects and a PolyMidi object which can handle
// 		midi voice routing. The polysynth class can receive midi notes and route the notes to
//		the corresponding synthesizer objects. 
class PolySynth{
public:
	// Init the polysynth with a pointer to a wavetable object and choose the amount of voices.
	PolySynth(WaveTable* wt, int numsynths=8);
	~PolySynth();
	
	// Fill the buffer with the amplitude values of all synth objects.
	void process(float* buffer, int buffersize);
	
	// Insert a new midi note into the polyMidi object and route the midi to the 
	// corresponding synth object.
	void newNote(int vel, int pitch);
	
	// Set the wave type of all synth objects.
	void setWaveType(int choice);
	
	WaveTable* wt;
	Synth** synths;
	
	int numSynths = 8;
	// Normalize the output to the amount of synths.
	float normAmp = 1.0f;
	
	// The polymidi object used to handle polyphonic midi routing.
	PolyMidi polyMidi;
};

#endif
