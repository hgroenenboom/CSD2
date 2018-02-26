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

void optionMenu() {
    std::cout << "\n\n\
	-- Menu --\n\
	Press 'q' when you want to quit the program.\n\
	Press 'a' if you want to change the amplitude. \n\
	Press 'o' to change the oscillator type. \n\
	Press 'f' to change the frequency.\n";
}

int main(int argc,char **argv)
{
	WaveTable wt;
	wt.getWaveByIndex(4);
	wt.printWaveNames();
    Synth synth(&wt);
	PolySynth polySynth(&wt);

    double amplitude = 0.5;
    double* ampPointer = &amplitude;

    //create a JackModule instance
    JackModule jack;
    //assign a function to the JackModule::onProces
    jack.onProcess = [&](jack_default_audio_sample_t *inBuf,
        jack_default_audio_sample_t *outBuf, jack_nframes_t nframes, double samplerate) {
            // synth.process(outBuf, nframes);
			polySynth.process(outBuf, nframes);

            return 0;
        };

    // init the jack, use program name as JACK client name
    jack.init(/*argv[0]*/"programma");
    jack.autoConnect();


    //keep the program running and listen for user input, q = quit
    bool running = true;

    char c[100];
    optionMenu();
    while (running)
    {

        std::cin >> c;

        switch(c[0])
        {
            case 'q':
            {
                running = false;
                std::cout << "Quiting... ";
                break;
            }
            case 'a':
            {
                std::cout << "\tSet new amplitude value    : ";

                double temp = 0.0;
                std::string s;
                std::cin >> s;
                try
                {
                    temp = stod(s);
                    if(temp >= 0.0 && temp <= 1.0) {
                        synth.setAmplitude(temp);
                        std::cout << "\t\tamplitude = " << amplitude << std::endl;
                    } else {
                        std::cout << "\t\t! Error amplitude out of range" << std::endl;
                    }
                }
                catch(std::exception& e) { // cathes a throw with type exception. Called by native function like stod.
                    std::cout << "\t\t! Error: " << e.what();
                }

                optionMenu();
                break;
            }
            case 'o':
            {
                wt.printWaveNames();
                int choice;
                try {
                    std::cin >> choice;
                } catch(std::exception& e) {
                    std::cout << e.what();
                }

                if(choice >= 0 && choice <= wt.amountOfWaves) {
                    polySynth.setWaveType(choice);
                }

                optionMenu();
                break;
            }
            case 'f':
            {
                std::cout << "\tEnter new frequency    : ";
                float midiP;
                try {
                    std::cin >> midiP;
                } catch(std::exception& e) {
                    std::cout << e.what();
                }

                if(midiP > 0.0f && midiP < 127.0f) {
					int index = polySynth.polyMidi.findIndexForMidiPitch(midiP);
					cout << "IndexForMidiPitch: " << index << endl;
					if(index == -1) {
						polySynth.newNote(127, midiP);
					} else {
						polySynth.newNote(0, midiP);
					}
                } else {
					cout << "Error: frequency must be a value between 0 and 127\n";
				}

                optionMenu();
                break;
            }
        }
    }

    //end the program
    return 0;
} // main()
