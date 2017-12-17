#include <iostream>

class Instrument
{
public:
	Instrument(std::string newString);
	void makeSound();
	void playTone();
	void makeMultipleSounds(int n, int time);
private:
	std::string sound = "Empty sound";
};