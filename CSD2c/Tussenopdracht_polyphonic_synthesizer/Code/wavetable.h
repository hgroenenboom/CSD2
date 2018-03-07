#ifndef _WAVETABLE_H_
#define _WAVETABLE_H_

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#define PI 3.14159265

using namespace std;

// A wave object will hold a single period from a specific wave. 
// A wavetable object will have multiple wave objects, between which it can switch.
// We use inheritance to predefine often used wave types.
class Wave{
public:
	Wave(char* newname);
	~Wave();

	// virtual void fillArray();
	void antiAliase() {};
	
	const static int numValues = 2048;
	float wave[numValues];
	float DOUBLE_PI = 2.0f * PI;
	char* name;
};

class SineWave : public Wave {
public:
	SineWave();

	void fillArray();
};

class SawWave : public Wave {
public:
	SawWave();

	void fillArray();
};

class SquareWave : public Wave {
public:
	SquareWave();

	void fillArray();
};

// A waveTable has an expandable vector of wave objects. 
// The WaveTable class provides functionality for getting wave information and 
// switching between waves.
class WaveTable{
public:
	WaveTable();
	
	// Get a wave array pointer by using the corresponding wave name.
	float* getWaveByName(char* input);
	// Get a wave array pointer by using the corresponding wave index.
	float* getWaveByIndex(int index);
	
	// Print info about the waves inside the waves vector.
	void printWaveNames();
	
	// Debugging functions:
	// Print multiple amplitude values of every wave.
	void printAllWaves();
	
	vector<Wave> waves;
	int amountOfWaves = 0;
};

#endif