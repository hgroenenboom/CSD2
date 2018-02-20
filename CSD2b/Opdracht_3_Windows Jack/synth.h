#include <array>
#include <iostream>
#include "oscilator.h"

using namespace std;

class Synthesizer{
	public:
		void proces(float* audioBuffer);

		float frequency = 2000;
		float amplitude = 1;
};

class SimpleSynth : Synthesizer {
	public:
		SimpleSynth(int waveType);

		void proces(float* audioBuffer, int bufferSize);

		SquareGenerator squareOscillator;
		SineGenerator sineOscillator;
		Oscillator *oscillatorPointer;
		int waveType = 0;

		/*
		enum{
			saw = 1,
			square = 2
		} */
};
