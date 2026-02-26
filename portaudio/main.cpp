#include <iostream>
#include <stdexcept>
#include <portaudio.h>
#include <portmidi.h>
#include "SynthEngine.h"

#include <string>

// TODO:    - Better voice handling
//          - Fix mallet spring aliasing
//          - Check delay line stuff
//          - Randomize position
//          - Spatialization
//          - simd
//          - make long / int consistent
//          - LFO on/off toggle
//          - Deal with stereo
//          - handle errors properly in SynthEngine
//          - set midi device properly
//          - determine mallet spring constant
//          - choose better gain values 
//          - experiment with mode gains


int main()
{
    const float          SAMPLE_RATE         { 44100 };
    const unsigned int   FRAMES_PER_BUFFER   { 256 };

    ModalSynth::SynthEngine engine;

    try 
    {
        engine.initialize(SAMPLE_RATE, FRAMES_PER_BUFFER);
        engine.start();

        std::cout << "Enter 'q' to quit: " << std::endl;
        std::string command{};
        float value{};

        while (command != "q")
        {
            std::cin >> command;

            if (command != "q")
            {
                std::cin >> value;
                std::cout << "val: " << value << std::endl;
                engine.handleCommand(command, value);
            }
        }
    }
    catch (const std::runtime_error& error)
    {
        std::cout << error.what() << std::endl;
    }

    try
    {
        engine.stop();
    }
    catch (const std::runtime_error& error)
    {
        std::cout << error.what() << std::endl;
    }

    try
    {
        engine.terminate();
    }
    catch (const std::runtime_error& error)
    {
        std::cout << error.what() << std::endl;
    }

    return 0;
}
