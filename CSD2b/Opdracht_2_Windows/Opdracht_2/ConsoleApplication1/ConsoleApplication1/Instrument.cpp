#include "Instrument.h"
#include <thread>
#include <chrono>
//#include "stdafx.h"

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
	for (int i = 0; i<n; i++) {
		Instrument::makeSound();
		std::this_thread::sleep_for(std::chrono::seconds(time));
	}
}

void Instrument::newTonalScale()
{
	int randomBasePitch = std::rand() % 12;
	int shuffelTonalScale = std::rand() % 7;

	
}

void Instrument::changeArray(std::string inputArray[], int remergePoint)
{
	std::string newArray[7];
	for (int i = 0; i < sizeof(inputArray); i++)
	{
		newArray[i] = inputArray[i];
	}
}

