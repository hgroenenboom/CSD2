#include <iostream>
#include <thread>
#include <string>
#include <exception> // used for expection handling

#include "synth.h"
#include "jack_module.h"
#include "math.h"

#define PI_2 6.28318530717959

void optionMenu() {
    std::cout << "\n\nPress 'q' when you want to quit the program.\nPress 'a' if you want to change the amplitude. \nPress 'o' to change the oscillator type. \nPress 'f' to change the frequency";
}

int main(int argc,char **argv)
{
    //Oscillator* oscP = nullptr;
    //oscP = new Saw;
    Synth<float> synth;

    double amplitude = 0.5;
    double* ampPointer = &amplitude;

    //create a JackModule instance
    JackModule jack;
    //assign a function to the JackModule::onProces
    jack.onProcess = [&](jack_default_audio_sample_t *inBuf,
        jack_default_audio_sample_t *outBuf, jack_nframes_t nframes, double samplerate) {

            //static double phase = 0;
            //static double frequency = 880;

            // for(int i = 0; i < nframes; i++) {
            //     outBuf[i] = amplitude * sin(phase * PI_2 );
            //     phase += frequency / samplerate;
            // }
            synth.process(outBuf, nframes);

            return 0;
        };

    // init the jack, use program name as JACK client name
    jack.init(/*argv[0]*/"Swaggerswag");
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
                        synth.oscpoint[ synth.oscType ]->setAmplitude(temp);
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
                std::cout << "\t0=sine, 1=saw    : ";
                int choice;
                try {
                    std::cin >> choice;
                } catch(std::exception& e) {
                    std::cout << e.what();
                }

                if(choice >= 0 && choice <= 1) {
                    synth.oscType = choice;
                }

                optionMenu();
                break;
            }
            case 'f':
            {
                std::cout << "\tEnter new frequency    : ";
                float choice;
                try {
                    std::cin >> choice;
                } catch(std::exception& e) {
                    std::cout << e.what();
                }

                if(choice > 3.0f && choice <= 18000.0f) {
                    synth.oscpoint[ synth.oscType ]->setFrequency(choice);
                }

                optionMenu();
                break;
            }
        }
    }

    //end the program
    return 0;
} // main()
