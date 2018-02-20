#include <iostream>
#include <array>
#include <math.h>
#include "jack_module.h"
#include "synth.h"

#define _USE_MATH_DEFINES

using namespace std;

int main(int argc, char *argv[]) {
	JackModule jack;
	// use program name as jack client name
	jack.init(argv[0]);

	SimpleSynth synth(1);
	synth.oscillatorPointer->setFrequency(10);

	int bufferSize = 256;
	float* audioBuffer = new float[bufferSize];

	for(int i = 0; i < 1; i++) {
		cout << "AUDIOCALLBACKBUFFER" << endl;
		synth.proces(audioBuffer, bufferSize);
	}

	delete audioBuffer;
}
