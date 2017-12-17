#include "oscilator.h"

void Oscillator::tick() {
	phase += 1.0f / (44100 / frequency);
}

SineGenerator::SineGenerator() {
		
}

float SineGenerator::getSample() {
	return sin(phase * 2 * M_PI) ;
}

float SquareGenerator::getSample() {
	// speciale if statement
	return ((int)(phase * 2) % 2 == 1) ? 1 : -1;
}