#include <iostream>
#include <array>
#include <math.h>

#define _USE_MATH_DEFINES

using namespace std;

class Oscillator {
	public:
		//pure virtual function declaration!!!
		virtual float getSample() = 0;
				
		void tick();
		
		setFrequency(int freq){ frequency = freq; }
		
		float phase = 0
			, amplitude
			, frequency = 33000
			, value = 0;
};

class SineGenerator : public Oscillator {
	public:
		SineGenerator();
		float getSample() override; //override
};

class SquareGenerator : public Oscillator {
	public:
		float getSample() override;
};