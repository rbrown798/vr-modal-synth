#include <iostream>
#include <cassert>
#include <thread>
#include <portaudio.h>
#include <portmidi.h>
#include "SynthEngine.h"


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


int main()
{
    const float          SAMPLE_RATE         { 44100 };
    const unsigned int   FRAMES_PER_BUFFER   { 256 };

    ModalSynth::SynthEngine engine;

    engine.initialize(SAMPLE_RATE, FRAMES_PER_BUFFER);
    engine.start();

    std::cout << "Enter 'q' to quit: " << std::endl;
    char input = ' ';
    while (input != 'q')
        std::cin >> input;

    engine.stop();
    engine.terminate();

    return 0;
}
