#ifndef GUITAR_H
#define GUITAR_H

#include <iostream>
#include <string>
#include "instrument.h"

using namespace std;

class Guitar: public Instrument {
	public:
		Guitar(string newType = "plucked string", string inputSound = "Plingggggggg!");
};

#endif