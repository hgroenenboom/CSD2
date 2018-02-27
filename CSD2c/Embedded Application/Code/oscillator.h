#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "wavetable.h"

#include <math.h>
#include <time.h>

#define PI_2 6.28318530717959

class Oscillator{
public:
    // virtual constructor met de samplerate?
	Oscillator(int samplerate, WaveTable* wavetable);
	~Oscillator();
	
    void nextAudioBlock(float* buffer, int buffersize);
	void addToNextAudioBlock(float* buffer, int buffersize);
	float interpolate(float index);
	
    void setSampleRate(float s) { samplerate = s; }
	void setWaveType(int type) { 
		waveType = type;
		currentWave = wavetable->waves[type].wave;	
	}
    void setAmplitude(float a) { amplitude = a; }
    void setFrequency(float f) { frequency = f; }
	void setPhase(float p) { phase = p; }

	
private:
	// char* waveName;  // not used
	WaveTable* wavetable;
	float* currentWave = nullptr;
	
    float samplerate = 44100.0f;
    float amplitude = 1.0f;
    // one cycle is 0-1 phase
	float phase = (rand() % 1000) * 0.001f;
    float frequency = 440.0f;
	int waveType = 0;
};



class MVOscillator{
public:
	MVOscillator(int samplerate, WaveTable* wavetable, int numvoices);
	~MVOscillator();
	
	void nextAudioBlock(float* buffer, int buffersize);
	void addNextAudioBlock(float* buffer, int buffersize);
	void createDetuneArray(float detune);
	
	void setMidiNote(float midi);
	void setAmplitude(float amp);
	void setSampleRate(float samplerate);
	void setWaveType(int type);
	
	float mtof(float midi);
	
	
	WaveTable* wt;
	Oscillator** oscArray = nullptr;
	float* detuneArray = nullptr;
	
	int numVoices = 8;
	float normAmp = 1.0f;
	
	int samplerate = 44100;
	float detune = 0.2f;
	float midi = 48;
	float amplitude = 1.0f;
	int waveType = 0;
	
	// Used for storing the calculated process data:
	// This has to happen because of the needed operations in addNextAudioBlock.
	float storage[4096];
};


#endif
