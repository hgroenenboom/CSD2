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

void SimpleSynth::proces(array<float, 256> audioBuffer) {
	for(int i = 0; i < sizeof(audioBuffer) / sizeof(audioBuffer[0]); i++) {
		audioBuffer[i] = oscillatorPointer->getSample();
		cout << "at " << i << " audioBuffer is " << audioBuffer[i] << endl;
		oscillatorPointer->tick();
	}
}