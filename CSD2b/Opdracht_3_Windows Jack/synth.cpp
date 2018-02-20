#include "synth.h"

void Synthesizer::proces(float* audioBuffer){

}

SimpleSynth::SimpleSynth(int waveType) :
			sineOscillator(),
			squareOscillator()
{
	this->waveType = waveType;
	switch(waveType) {
		case 1: oscillatorPointer = &sineOscillator;
			break;
		case 2:	oscillatorPointer = &squareOscillator;
			break;
	}
}

void SimpleSynth::proces(float* audioBuffer, int buffersize) {
	for(int i = 0; i < buffersize; i++) {
		audioBuffer[i] = oscillatorPointer->getSample();
		cout << "at " << i << " audioBuffer is " << audioBuffer[i] << endl;
		oscillatorPointer->tick();
	}
}
