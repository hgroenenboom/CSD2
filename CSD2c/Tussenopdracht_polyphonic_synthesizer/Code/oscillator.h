#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "wavetable.h"

#include <math.h>
#include <time.h>

#define PI_2 6.28318530717959

// An oscillator object will be able to fill an input buffer with a wave
// extracted from a wavetable object. Every oscillator will have it's own 
// phase and amplitude.
class Oscillator{
public:
	Oscillator(int samplerate, WaveTable* wavetable);
	~Oscillator();
	
	// Fills the input buffer with amplitudes from the used wave.
    void nextAudioBlock(float* buffer, int buffersize);
	// Add amplitude values from the used wave to the input buffer .
	void addToNextAudioBlock(float* buffer, int buffersize);
	// Used in NextAudioBlock functions to interpolate between wavetable values.
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
	// locally stored integer to represent the waveType of the the oscillator.
	int waveType = 0;
};

// The MVOcillator: Multi-Voiced Oscillator 
// The MVOscillator contains multiple oscillator objects. 
// This is primarily used for detuning / chorus effect like oscillators.
class MVOscillator{
public:
	MVOscillator(int samplerate, WaveTable* wavetable, int numvoices);
	~MVOscillator();
	
	// Fills the input buffer with amplitudes from the oscillators.
	void nextAudioBlock(float* buffer, int buffersize);
	// Add amplitude values from the oscillators to the input buffer .
	void addNextAudioBlock(float* buffer, int buffersize);
	// Create a detune pitch array, with random detune centered around zero.
	// The amount of detune can be controlled. The values will be added to the midi pitch.
	void createDetuneArray(float detuneAmount=0.2f);
	
	
	void setMidiPitch(float midipitch);
	void setAmplitude(float amp);
	void setSampleRate(float samplerate);
	void setWaveType(int type);
	
	// convert midi pitch to frequency/
	float mtof(float midiPitch);
	
	// TODO: these variables should eventually be private
	
	// These variables are initialized inside the constructor and should never be nullptr.
	WaveTable* wt = nullptr;
	Oscillator** oscillators = nullptr;
	float* detuneArray = nullptr;
	
	int numVoices = 8;
	// Variable for volume compensation regarding the number of voices.
	float normAmp = 1.0f;
	
	int samplerate = 44100;
	float detune = 0.2f;
	float midiPitch = 48;
	float amplitude = 1.0f;
	int waveType = 0;
	
	// Used for storing the calculated process data:
	// This has to happen because of the needed operations in addNextAudioBlock.
	float storage[4096];
};


#endif
