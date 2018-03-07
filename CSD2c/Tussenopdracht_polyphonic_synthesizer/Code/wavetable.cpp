#include "wavetable.h"

Wave::Wave(char* newname) {
	name = newname;
}

Wave::~Wave () {
	delete(name);
}

SineWave::SineWave() 
	: Wave("Sine")
{
	fillArray();
}

void SineWave::fillArray() {
	// Fill wave array with a single sine period.
	for(int i = 0; i < 2048; i++) {
		wave[i] = sin(((float)i / 2048.0f) * DOUBLE_PI);
	}
	antiAliase();
}

SawWave::SawWave() 
	: Wave("Saw") 
{
	fillArray();
}

void SawWave::fillArray() {
	// Fill wave array with a single sawtooth period.
	for(int i = 0; i < 2048; i++) {
		wave[i] = 2.0f * ((float)i / 2048) - 1.0f;
	}
	antiAliase();
}

SquareWave::SquareWave() 
	: Wave("Square") 
{
	fillArray();	
}

void SquareWave::fillArray() {
	// Fill wave array with a single square period. 
	for(int i = 0; i < 2048; i++) {
		wave[i] = i < 1024 ? -1.0 : 1.0f;
	}
	antiAliase();
}

WaveTable::WaveTable() {
	// Initialize the wavetable with 3 basic waves.
	waves.push_back(SineWave());
	waves.push_back(SquareWave());
	waves.push_back(SawWave());
	amountOfWaves = waves.size();
}

float* WaveTable::getWaveByName(char* input) {
	// Loop through every wave inside waves to find the corresponding name.
	for(int i = 0; i < amountOfWaves; i++) {
		if(waves[i].name == input) {
			return waves[i].wave;
		}
	}
	cout << "Not a valid wave name" << endl;
}
	
float* WaveTable::getWaveByIndex(int index) {
	if(index >= 0 && index < amountOfWaves) {
		return waves[index].wave;
	} else {
		cout << "Wave index out of range... (index 0 to " << amountOfWaves-1 << ")" << endl;
		cout << "Returning wave 0: " << waves[0].name << endl;
		return waves[0].wave;
	}
}
	
void WaveTable::printWaveNames() {
	cout << "Stored waves: ";
	for(int i = 0; i < amountOfWaves; i++) {
		// Print the wave index:
		cout << i << "=" << waves[i].name;
		// Print the wave name:
		if(i != amountOfWaves - 1) { 
			cout << ", "; 
		}
	}
	cout << endl;
}
	
void WaveTable::printAllWaves() {
	for(int i = 0; i < amountOfWaves; i++) {
		cout << "Output of " << waves[i].name << endl;
		for(int j = 0; j < 100; j++) {
			// Print the first 100 amplitudes of the selected waves.
			cout << " " << waves[i].wave[j];
		}
		cout << endl;
	}
}