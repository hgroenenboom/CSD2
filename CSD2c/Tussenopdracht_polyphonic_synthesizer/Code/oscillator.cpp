#include "oscillator.h"

Oscillator::Oscillator(int samplerate, WaveTable* wavetable) 
	: samplerate(samplerate),
	wavetable(wavetable)
{	
	// Initialize oscillator with first wave in wavetable.
	currentWave = wavetable->waves[0].wave;
}

Oscillator::~Oscillator() {
	
}

// TODO: declare variables outside of the function, to reduce dynamic memory allocation.
float Oscillator::interpolate(float index) {
	// get indices to interpolate between
	int i = (int)floor(index);
	int i2 = (i + 1) % 2048;
	
	// interpolate
	float weight = index - (float)i;
	float value = (1.0f - weight) * currentWave[i] + weight * currentWave[i2];
	return value;
}

void Oscillator::nextAudioBlock(float* buffer, int buffersize) {
	for(int i = 0; i < buffersize; i++) {
		// replace buffer value.
		buffer[i] = amplitude * interpolate(phase * 2048.0f);
		// Adjust oscillator phase.
		phase += (frequency / samplerate);
		phase = fmod(phase, 1.0f);
	}
}

void Oscillator::addToNextAudioBlock(float* buffer, int buffersize) {
	for(int i = 0; i < buffersize; i++) {
		buffer[i] += amplitude * interpolate(phase * 2048.0f);
		
		phase += (frequency / samplerate);
		phase = fmod(phase, 1.0f);
	}
}





MVOscillator::MVOscillator(int samplerate, WaveTable* wavetable, int numvoices=8) 
	: numVoices(numvoices),
	wt(wavetable)
{
	// Calculated the needed normalization amplitude.
	normAmp = 1.0f / (float)numVoices;
	
	// Create new oscillator objects.
	detuneArray = new float[numVoices];
	oscillators = new Oscillator*[numVoices];
	if(wt != nullptr) {
		for(int i = 0; i < numVoices; i++) {
		   oscillators[i] = new Oscillator(samplerate, wt);
		}
	}
	
	// Initialize detune in the owned oscillators.
	if(oscillators != nullptr) {
		createDetuneArray(detune);
		setMidiPitch(midiPitch);
	} else {
		cout << "Error: oscillators not created.";
	}
}

MVOscillator::~MVOscillator() {
	delete(detuneArray);
	for(int i = 0; i < numVoices; i++) {
		delete(oscillators[i]);
	}
	delete(oscillators);
}

void MVOscillator::nextAudioBlock(float* buffer, int buffersize) {
	// Zero fill buffer
	for(int i = 0; i < buffersize; i++) { 
		buffer[i] = 0.0f; 
	}
	// Add amplitudes from all oscillators to the buffer.
	for(int i = 0; i < numVoices; i++) {
		oscillators[i]->addToNextAudioBlock(buffer, buffersize);
	}
	// Modify the amplitude.
	for(int i = 0; i < buffersize; i++) {
		buffer[i] *= amplitude;
		buffer[i] *= normAmp;
	}
}

void MVOscillator::addNextAudioBlock(float* buffer, int buffersize) {  
	// Zero fill the storage buffer
	for(int i = 0; i < buffersize; i++) {
		storage[i] = 0.0f;
	}
	// Add amplitudes from all oscillators to the storage buffer.
	for(int i = 0; i < numVoices; i++) {
		oscillators[i]->addToNextAudioBlock(storage, buffersize);
	}
	// Modify the amplitude and add storage buffer values to the input buffer.
	for(int i = 0; i < buffersize; i++) {
		storage[i] *= amplitude;
		storage[i] *= normAmp;
		
		buffer[i] += storage[i];
	}
	
}

void MVOscillator::createDetuneArray(float detune) {
	// Generate a detuned midi value for every oscillator.
	for(int i = 0; i < numVoices; i++) {
		float randomNum = ((rand() % 1000) * 0.001f - 0.5f) * (0.5f * detune);
		detuneArray[i] = randomNum;
	}
}

void MVOscillator::setMidiPitch(float midipitch) {
	this->midiPitch = midipitch;
	for(int i = 0; i < numVoices; i++) {
		// calculate detuned frequency for the oscillator.
		float freq = mtof(midiPitch + detuneArray[i]);
		oscillators[i]->setFrequency(freq);
	}
}

void MVOscillator::setAmplitude(float amp) {
	amplitude = amp;
	for(int i = 0; i < numVoices; i++) {
		oscillators[i]->setAmplitude(amp);
	}
}

void MVOscillator::setSampleRate(float samplerate) {
	this->samplerate = samplerate;
	for(int i = 0; i < numVoices; i++) {
		oscillators[i]->setSampleRate(samplerate);
	}
}

void MVOscillator::setWaveType(int type) {
	this->waveType = type;
	for(int i = 0; i < numVoices; i++) {
		oscillators[i]->setWaveType(type);
	}
}

float MVOscillator::mtof(float midipitch) {
	// calculate the frequency in Hz from the midi pitch:
	return 440.0f * pow(2.0f, (midipitch - 69.0f) / 12.0f);
}