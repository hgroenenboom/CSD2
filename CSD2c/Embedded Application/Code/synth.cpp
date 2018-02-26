#include "synth.h" 

Synth::Synth(WaveTable* wt) : 
	wt(wt),
	mvOscillator(44100, wt, 8)
{
	cout << "inside synth constructor" << endl;
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
	cout << "velocity: " << tempVel << endl;
	setAmplitude(tempVel);
}
	
void Synth::setMidiPitch(float midiP) {
	midiNote = midiP;
	mvOscillator.setMidiNote(midiP);
}