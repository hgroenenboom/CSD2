//
//	Polyphonic detuned midi synth
//	Made by Harold Groenenboom
//
//	TODO: 
// 	- Add filter
//	- Add envelopes
//	- Add anti-aliasing
//	- Add midi input.
//	- Clean up console input
//	

#include <iostream>
#include <thread>
#include <string>
#include <exception> // used for expection handling

#include "wavetable.h"
#include "synth.h"
#include "jack_module.h"
#include "math.h"

#define PI_2 6.28318530717959

using namespace std;

template <class T>
T numberInput () {
	T val;
	try {
		std::string s;
		std::cin >> s;
        val = stod(s);
		return val;
	} catch(std::exception& e) {
		std::cout << "\t\t!Error: Wrong input";
		return -1.0;
	}
}

void optionMenu() {
    std::cout << "\n\n-- Menu --\n\
    Press 'm' to show the option Menu. \n\
	Press 'q' when you want to quit the program.\n\
	Press 'a' if you want to change the master amplitude. \n\
	Press 'o' to change the wave type. \n\
	Press 'f' to input the midi pitch of the note you \n\
	\twant to turn on or off.\n\
	Press 'p' to input the midi pitch with detailed debug output.\n";
}

int main(int argc,char **argv)
{
	WaveTable wt;
	wt.printWaveNames();
	
	PolySynth polySynth(&wt);

	// Master amplitude
    double masterAmplitude = 0.5;

    //create a JackModule instance
    JackModule jack;
    //assign a function to the JackModule::onProces
    jack.onProcess = [&](jack_default_audio_sample_t *inBuf,
        jack_default_audio_sample_t *outBuf, jack_nframes_t nframes, double samplerate) {
            // get audio from the poly synth
			polySynth.process(outBuf, nframes);

			for(int i = 0; i < nframes; i++) {
				outBuf[i] *= masterAmplitude;
			}
			
            return 0;
        };

    // init the jack, use program name as JACK client name
    jack.init(/*argv[0]*/"jack_synthesizer");
    jack.autoConnect();



	//keep the program running and listen for user input, q = quit
    bool running = true;

	// Handle user input:
    char c[100];
	// display input options.
    optionMenu();
	
    while (running)
    {
		// write user input into character array.
		cout << "In: ";
        std::cin >> c;

		// find corresponding option.
        switch(c[0])
        {
			case 'm':
			{
				optionMenu();
				break;
			}

            case 'q':
            {
				// end while loop
                running = false;
                std::cout << "Quiting... ";
                break;
            }

            case 'a':
            {
                std::cout << "\tmaster amplitude = ";
                double amp = numberInput<double>();

				// set master amplitude.
				if(amp >= 0.0 && amp <= 1.0) {
					masterAmplitude = amp;
					std::cout << "\t\tmaster amplitude = " << masterAmplitude;
				} else {
					std::cout << "\t\t! Error master amplitude out of range";
				}
				
                break;
            }

            case 'o':
            {
				// print available wave types
				cout << "\t";
                wt.printWaveNames();

				// Check if input is a integer
				cout << "\twavetype = ";
                int choice = numberInput<int>();

				// If input is in range, change wavetype
                if(choice >= 0 && choice <= wt.amountOfWaves) {
                    polySynth.setWaveType(choice);
					cout << "\tNew wavetype: " << wt.waves[choice].name;
                } else {
					cout << "\t!Error: Wavetype out of range... ";
				}

                break;
            }

            case 'f':
            {
                std::cout << "\tmidi pitch = ";

				// check if input is float
                float midiP = numberInput<float>();

				// if input pitch is in the normal midi range, 
                if(midiP > 0.0f && midiP < 127.0f) {
					// Here were checking whether a new note should be created,
					// or whether a note should be turned off.
					// This piece of code should be easily replacable when working with real
					// midi input.

					// check if the midi pitch is used.
					int index = polySynth.polyMidi.findIndexForMidiPitch(midiP);

					// if note used, create a new note. Else turn the corresponding note off.
					if(index == -1) {
						polySynth.newNote(127, midiP);
					} else {
						polySynth.newNote(0, midiP);
					}
                } else {
					cout << "\t\t!Error: midi pitch must be a value between 0 and 127";
				}

                break;
            }

			case 'p':
            {
                std::cout << "\tmidi pitch = ";

                float midiP = numberInput<float>();

                if(midiP > 0.0f && midiP < 127.0f) {
					int index = polySynth.polyMidi.findIndexForMidiPitch(midiP);

					if(index == -1) {
						polySynth.newNote(127, midiP);
					} else {
						polySynth.newNote(0, midiP);
					}
                } else {
					cout << "\t\t!Error: midi pitch must be a value between 0 and 127";
				}

				polySynth.polyMidi.printAllNotes();

                break;
            }
        }

		cout << endl << endl;
    } // end user input while loop.

    //end the program
    return 0;
}; // main()