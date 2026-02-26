#pragma once

#include <thread>
#include <string>
#include <unordered_map>
#include <portaudio.h>
#include <portmidi.h>
#include "Synthesizer.h"
#include "RingBuffer.h"


namespace ModalSynth 
{
struct MidiMessage
{
    int status;
    int data1;
    int data2;
};

const std::unordered_map<std::string, int> COMMAND_MAP = {
    {"timb", 21}, {"damp", 22}, {"diam", 23}, {"stif", 24}, {"trem", 25}, 
    {"lpos", 26}
};

class SynthEngine
{
private:
    Synthesizer                     m_synth;
    RingBuffer<MidiMessage, 8192>   m_midiMessages;
    PaStream*                       m_audioStream{};
    PortMidiStream*                 m_midiStream{};
    float                           m_sampleRate{};
    int                             m_framesPerBuffer{};
    std::atomic<bool>               m_isRunning{};
    std::thread                     m_thread;

public:
    void initialize(float sampleRate, int framesPerBuffer);
    void terminate();
    void start();
    void stop();
    void run();
    void handleMessage(MidiMessage msg);
    void handleCommand(std::string command, float value);
    void renderBlock(float* out, unsigned long N, int outChannels);
    static int audioCallback(const void* inputBuffer,
                             void *outputBuffer, 
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo *timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData);
};
};
