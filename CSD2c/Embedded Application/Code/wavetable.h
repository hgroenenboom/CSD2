#ifndef _WAVETABLE_H_
#define _WAVETABLE_H_

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#define PI 3.14159265

using namespace std;

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

class WaveTable{
public:
	WaveTable();
	
	float* getWaveByName(char* input);
	float* getWaveByIndex(int index);
	
	void printWaveNames();
	void printAllWaves();
	
	vector<Wave> waves;
	int amountOfWaves = 0;
};

#endif