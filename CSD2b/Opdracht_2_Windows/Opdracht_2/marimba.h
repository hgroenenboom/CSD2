#ifndef MARIMBA_H
#define MARIMBA_H

#include <iostream>
#include <string>
#include "instrument.h"

using namespace std;

class Marimba: public Instrument {
	public:
		Marimba(string newType = "melodic percussion", string inputSound = "Piiiiing!");
};

#endif