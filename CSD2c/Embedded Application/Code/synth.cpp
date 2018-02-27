#include "synth.h" 

Synth::Synth(WaveTable* wt) : 
	wt(wt),
	mvOscillator(44100, wt, 8)
{
	// cout << "inside synth constructor" << endl;
	mvOscillator.setMidiNote(midiNote);
}

void Synth::process(float* buffer, int buffersize) {
	mvOscillator.nextAudioBlock(buffer,buffersize);
}

void Synth::processAdd(float* buffer, int buffersize) {
	mvOscillator.addNextAudioBlock(buffer,buffersize);
}

void Synth::setSamplerate(int sr) {
	mvOscillator.setSampleRate(sr);
}
	
void Synth::setAmplitude(float amp) {
	mvOscillator.setAmplitude(amp);
}

void Synth::setWaveType(int type) {
	if(type >= 0 && type < wt->amountOfWaves) {
		mvOscillator.setWaveType(type);
	} else {
		cout << "Oscillator type out of range... Try:"  << endl;
		wt->printWaveNames();
	}
}
	
void Synth::setMidiNote(float vel, float pitch) {
	setMidiPitch(pitch);
	float tempVel = 0.0f;
	if(vel <= 0.0001f) {
		tempVel = 0.0f;
	} else {
		tempVel = (vel / 127.0f) * 0.5f + 0.5f;
	}
	// cout << "velocity: " << tempVel << endl;
	setAmplitude(tempVel);
}
	
void Synth::setMidiPitch(float midiP) {
	midiNote = midiP;
	mvOscillator.setMidiNote(midiP);
}




PolySynth::PolySynth(WaveTable* wt, int numSynths)
	: wt(wt), 	
	  numSynths(numSynths),
	  polyMidi(numSynths)
{
	// Fill synth array with the chosen numbers of synths.
	synths = new Synth*[numSynths];
	for(int i = 0; i < numSynths; i++) {
		synths[i] = new Synth(wt);
		synths[i]->setAmplitude(0.0f);
	}
	
	normAmp = 1.0f / (float)numSynths;
}

PolySynth::~PolySynth() {
	for(int i = 0; i < numSynths; i++) {
		delete(synths[i]);
	}
	delete(synths);
}

void PolySynth::process(float* buffer, int buffersize) {
	for(int i = 0; i < buffersize; i++) {
		buffer[i] = 0.0f;
	}
	for(int i = 0; i < numSynths; i++) {
		synths[i]->processAdd(buffer, buffersize);
	}
	for(int i = 0; i < buffersize; i++) {
		buffer[i] *= normAmp;
	}
}

void PolySynth::newNote(int vel, int pitch) {
	int note = polyMidi.newNote(vel, pitch);
	synths[note]->setMidiNote(vel, pitch);
}

void PolySynth::setWaveType(int choice) {
	for(int i = 0; i < numSynths; i++) {
		synths[i]->setWaveType(choice);
	}
}