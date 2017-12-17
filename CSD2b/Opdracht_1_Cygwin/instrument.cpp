#include "instrument.h"
#include <thread>
#include <chrono>

Instrument::Instrument(std::string newString)
{
	sound = newString;
}

void Instrument::makeSound()
{
	std::cout << sound << "\n";
}

void Instrument::playTone()
{
	std::cout << "The length of the music is " << sound.length() << "s.\n";
}

void Instrument::makeMultipleSounds(int n, int time)
{
	for (int i=0; i<n; i++) {
		Instrument::makeSound();
		std::this_thread::sleep_for(std::chrono::seconds(time));
	}
}