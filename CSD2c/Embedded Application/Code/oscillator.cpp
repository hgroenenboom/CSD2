#include "oscillator.h"

float Oscillator::interpolate(float index) {
	int i = (int)floor(index);
	int i2 = (i + 1) % 2048;
	float weight = index - (float)i;
	float value = (1.0f - weight) * currentWave[i] + weight * currentWave[i2];
	return value;
}

Oscillator::Oscillator(int samplerate, WaveTable* wavetable) 
	: samplerate(samplerate),
	wavetable(wavetable)
{
	currentWave = wavetable->waves[0].wave;
}
	
void Oscillator::nextAudioBlock(float* buffer, int buffersize) {
	for(int i = 0; i < buffersize; i++) {
		buffer[i] = amplitude * interpolate(phase * 2048.0f);
		phase += (frequency / samplerate);
		phase = fmod(phase, 1.0f);
		//cout << "Inside oscillator callback: currentwave " << currentWave[i] << endl;
	}
}

void Oscillator::addToNextAudioBlock(float* buffer, int buffersize) {
	for(int i = 0; i < buffersize; i++) {
		buffer[i] += amplitude * interpolate(phase * 2048.0f);
		phase += (frequency / samplerate);
		phase = fmod(phase, 1.0f);
		//cout << "Inside oscillator callback: currentwave " << currentWave[i] << endl;
	}
}





MVOscillator::MVOscillator(int samplerate, WaveTable* wavetable, int numvoices=8) 
	: numVoices(numvoices),
	wt(wavetable)
{
	normAmp = 1.0f / (float)numVoices;
	cout << "Normalizing amp: " << normAmp << endl;
	
	oscArray = new Oscillator*[numVoices];
	detuneArray = new float[numVoices];
	if(wt != nullptr) {
		for(int i = 0; i < numVoices; i++) {
		   oscArray[i] = new Oscillator(samplerate, wt);
		   // cout << "created " << i << "nd oscillator" << endl;
		}
	}
	
	if(oscArray != nullptr) {
		createDetuneArray(detune);
		setMidiNote(midi);
	}
}

void MVOscillator::nextAudioBlock(float* buffer, int buffersize) {
	for(int i = 0; i < buffersize; i++) { 
		buffer[i] = 0.0f; 
	}
	for(int i = 0; i < numVoices; i++) {
		oscArray[i]->addToNextAudioBlock(buffer, buffersize);
	}
	for(int i = 0; i < buffersize; i++) {
		buffer[i] *= normAmp;
		buffer[i] *= amplitude;
	}
}

void MVOscillator::addNextAudioBlock(float* buffer, int buffersize) {  

	for(int i = 0; i < buffersize; i++) {
		storage[i] = 0.0f;
	}
	for(int i = 0; i < numVoices; i++) {
		oscArray[i]->addToNextAudioBlock(storage, buffersize);
	}
	for(int i = 0; i < buffersize; i++) {
		storage[i] *= normAmp;
		storage[i] *= amplitude;
		buffer[i] += storage[i];
	}
	
}
	// delete(temp);

void MVOscillator::createDetuneArray(float detune) {
	for(int i = 0; i < numVoices; i++) {
		float randomNum = ((rand() % 1000) * 0.001f - 0.5f) * (0.5f * detune);
		// cout << "new random detune value: " << i << " is " << randomNum << endl;
		detuneArray[i] = randomNum;
	}
}

void MVOscillator::setMidiNote(float midi) {
	this->midi = midi;
	for(int i = 0; i < numVoices; i++) {
		float freq = mtof(midi + detuneArray[i]);
		// cout << "new freq note at " << i << " is " << freq << endl;
		oscArray[i]->setFrequency(freq);
	}
}

void MVOscillator::setAmplitude(float amp) {
	amplitude = amp;
	// for(int i = 0; i < numVoices; i++) {
		// oscArray[i].setAmplitude(amp);
	// }
}

void MVOscillator::setSampleRate(float samplerate) {
	this->samplerate = samplerate;
	for(int i = 0; i < numVoices; i++) {
		oscArray[i]->setSampleRate(samplerate);
	}
}

void MVOscillator::setWaveType(int type) {
	this->waveType = type;
	for(int i = 0; i < numVoices; i++) {
		oscArray[i]->setWaveType(type);
	}
}

float MVOscillator::mtof(float midi) {
	return 440.0f * pow(2.0f, (midi - 69.0f) / 12.0f);
}