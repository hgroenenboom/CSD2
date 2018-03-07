#include "synth.h" 

Synth::Synth(WaveTable* wt) : 
	wt(wt),
	mvOscillator(44100, wt, 8)
{
	mvOscillator.setMidiPitch(midiPitch);
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
	/// set midi pitch:
	setMidiPitch(pitch);
	
	// Set amplitude:
	float amp = 0.0f;
	if(vel <= 0.0001f) {
		amp = 0.0f;
	} else {
		amp = (vel / 127.0f) * 0.5f + 0.5f;
	}
	
	setAmplitude(amp);
}

void Synth::setMidiPitch(float midiP) {
	midiPitch = midiP;
	mvOscillator.setMidiPitch(midiP);
}




PolySynth::PolySynth(WaveTable* wt, int numsynths)
	: wt(wt), 	
	  numSynths(numsynths),
	  polyMidi(numsynths)
{
	// Create chosen amount of synthesizer objects.
	synths = new Synth*[numSynths];
	for(int i = 0; i < numSynths; i++) {
		synths[i] = new Synth(wt);
		synths[i]->setAmplitude(0.0f);
	}
	
	// Calculate the normalizing amplitude based on the number of synths.
	normAmp = 1.0f / (float)numSynths;
}

PolySynth::~PolySynth() {
	for(int i = 0; i < numSynths; i++) {
		delete(synths[i]);
	}
	delete(synths);
}

void PolySynth::process(float* buffer, int buffersize) {
	// Init input buffer.
	for(int i = 0; i < buffersize; i++) {
		buffer[i] = 0.0f;
	}
	// Add amplitude values of all synthesizers to the buffer.
	for(int i = 0; i < numSynths; i++) {
		synths[i]->processAdd(buffer, buffersize);
	}
	// Normalize output amplitude.
	for(int i = 0; i < buffersize; i++) {
		buffer[i] *= normAmp;
	}
}

void PolySynth::newNote(int vel, int pitch) {
	// get the corresponding voice of an input note from the PolyMidi.newNote() function.
	int voice = polyMidi.newNote(vel, pitch);
	synths[voice]->setMidiNote(vel, pitch);
}

void PolySynth::setWaveType(int choice) {
	// if choice is in range, set wave type:
	if(choice >= 0 && choice < wt->amountOfWaves) {
		for(int i = 0; i < numSynths; i++) {
			synths[i]->setWaveType(choice);
		}
	} else {
		cout << "Oscillator type out of range... Try:"  << endl;
		wt->printWaveNames();
	}
	
}