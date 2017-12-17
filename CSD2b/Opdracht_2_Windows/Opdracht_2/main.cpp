#include <iostream>
#include "instrument.h"
#include "marimba.h"
#include "guitar.h"

int main() 
{
	Instrument instrument;
	Marimba marimba;
	Guitar guitar;
	
	marimba.makeSound(36);
	
	marimba.setSound("TIiinngggg....");
	marimba.makeSound(393);
	marimba.makeSound(40);
	guitar.makeSound(58);
	marimba.makeSound(42);
}