#include <iostream>
#include <array>
#include <math.h>
#include "synth.h"

#define _USE_MATH_DEFINES

using namespace std;

int main() {
	SimpleSynth synth(1);
	synth.oscillatorPointer->setFrequency(10);
	
	array<float, 256> audioBuffer;
	
	for(int i = 0; i < 1; i++) {
		cout << "NEWSTUFFLOLNEWSTUFFLOLNEWSTUFFLOL" << endl;
		synth.proces(audioBuffer);
	}
}