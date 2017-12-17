#include <iostream>
#include "Instrument.h"
//#include "stdafx.h"

int main()
{
	Instrument instrument("Hi I'm a fucking sound yooooo!");
	std::cout << "\n";

	instrument.makeSound();
	instrument.playTone();
	instrument.makeMultipleSounds(7, 2);

	return(0);
}