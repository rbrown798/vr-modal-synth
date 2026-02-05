#include <cstdlib>
#include <stdexcept>
#include <thread>
#include "SynthEngine.h"


namespace ModalSynth 
{
void SynthEngine::initialize(float sampleRate, int framesPerBuffer)
{
    m_sampleRate = sampleRate;
    m_framesPerBuffer = framesPerBuffer;

    m_synth.initialize(sampleRate, framesPerBuffer);

    m_synth.setMalletHeadDiameter(4.0f);
    m_synth.setMalletHeadDensity(8000.0f);
    m_synth.setMalletHeadStiffness(0.5f);   // 0.8
    m_synth.setBarTimbre(1.0f);             // 0.0
    m_synth.setBarDamping(0.5f);            // 0.05
    m_synth.setTubeOn(true);

    PaError paErr = Pa_Initialize();
    if (paErr != paNoError)
    {
        throw std::runtime_error(
                std::string("Could not initialize PortAudio. PaError: ") +
                Pa_GetErrorText(paErr));
    }

    PmError pmErr = Pm_Initialize();
    if (pmErr != pmNoError)
    {
        throw std::runtime_error(
                std::string("Could not initialize PortMidi. PmError: ") + 
                Pm_GetErrorText(pmErr));
    }
}

void SynthEngine::terminate()
{
    PmError pmErr = Pm_Terminate();
    if (pmErr != pmNoError)
    {
        throw std::runtime_error(
                std::string("Could not terminate PortMidi. PmError: ") +
                Pm_GetErrorText(pmErr));
    }

    PaError paErr = Pa_Terminate();
    if (paErr != paNoError)
    {
        throw std::runtime_error(
                std::string("Could not terminate PortAudio. PaError: ") +
                Pa_GetErrorText(paErr));
    }
}

void SynthEngine::start()
{
    PaStreamParameters outParams;
    outParams.device = Pa_GetDefaultOutputDevice(); 
    outParams.channelCount = 1;
    outParams.hostApiSpecificStreamInfo = NULL;
    outParams.sampleFormat = paFloat32;
    outParams.suggestedLatency = Pa_GetDeviceInfo( 
                                    outParams.device)->defaultLowOutputLatency;

    PaError paErr = Pa_OpenStream(&m_audioStream, 
                                  NULL, 
                                  &outParams, 
                                  static_cast<double>(m_sampleRate),
                                  //paFramesPerBufferUnspecified,
                                  static_cast<unsigned int>(m_framesPerBuffer),
                                  paNoFlag,
                                  SynthEngine::audioCallback,
                                  this);
    if (paErr != paNoError)
    {
        throw std::runtime_error(
                std::string("Could not open audio output stream. PaError: ") +
                Pa_GetErrorText(paErr));
    }

    paErr = Pa_StartStream(m_audioStream);
    if (paErr != paNoError)
    {
        throw std::runtime_error(
                std::string("Could not start audio output stream. PaError: ") +
                Pa_GetErrorText(paErr));
    }

    // deal with this better
    int deviceSelection = 3;

    PmError pmErr = Pm_OpenInput(&m_midiStream, 
                                 deviceSelection, 
                                 nullptr, 
                                 512, 
                                 nullptr, 
                                 nullptr);
    if (pmErr != pmNoError)
    {
        throw std::runtime_error(
                std::string("Could not open midi input stream. PmError: ") +
                Pa_GetErrorText(pmErr));
    }

    m_isRunning.store(true);
    m_thread = std::thread(&SynthEngine::run, this);
}

void SynthEngine::stop()
{
    m_isRunning.store(false);
    m_thread.join();

    PmError pmErr = Pm_Close(m_midiStream);
    if (pmErr != pmNoError)
    {
        throw std::runtime_error(
                std::string("Could not close midi input stream. PmError: ") +
                Pm_GetErrorText(pmErr));
    }

    PaError paErr;
    paErr = Pa_StopStream(m_audioStream);
    if (paErr != paNoError)
    {
        throw std::runtime_error(
                std::string("Could not stop audio output stream. PaError: ") +
                Pa_GetErrorText(paErr));
    }

    paErr = Pa_CloseStream(m_audioStream);
    if (paErr != paNoError)
    {
        throw std::runtime_error(
                std::string("Could not close audio output stream. PaError: ") +
                Pa_GetErrorText(paErr));
    }
}

void SynthEngine::run()
{
    MidiMessage msg;
    while (m_isRunning.load())
    {
        if (Pm_Poll(m_midiStream) == true)
        {
            PmEvent event;

            if (Pm_Read(m_midiStream, &event, 1) == 1)
            {
                msg.status = Pm_MessageStatus(event.message);
                msg.data1 = Pm_MessageData1(event.message);
                msg.data2 = Pm_MessageData2(event.message);

                std::cout << msg.status << " " << msg.data1 << " " << msg.data2 
                    << " " << std::endl;

                m_midiMessages.push(msg);
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void SynthEngine::handleMessage(MidiMessage msg)
{
    if (msg.status == 0x99)
    {
        if (msg.data1 == 40)
            m_synth.setTubeOn(true);

        else if (msg.data1 == 41)
            m_synth.setTubeOn(false);

        else if (msg.data1 == 36)
            m_synth.setMotorOn(true);

        else if (msg.data1 == 37)
            m_synth.setMotorOn(false);
    }

    else if ((msg.status & 0xF0) == 0x90) // Note on
    {
        int note = msg.data1;
        float velocity = 5.0f * static_cast<float>(msg.data2) / 127.f;
        float position = 0.5f + 0.5f * (static_cast<float>(random()) / 
                static_cast<float>(RAND_MAX) - 1.f);

        m_synth.noteOn(note, velocity, position);
    }
    else if ((msg.status & 0xF0) == 0x80) // Note off
    {
        //int note = msg.data1;
        //m_synth.noteOff(note);
    }
    else if ((msg.status & 0xF0) == 0xB0)
    {
        if (msg.data1 == 21)
        {
            m_synth.setBarTimbre(static_cast<float>(msg.data2) / 127.f);
        } 
        else if (msg.data1 == 22)
        {
            m_synth.setBarDamping(static_cast<float>(msg.data2) / 127.f);
        }
        else if (msg.data1 == 23)
        {
            // NOTE: Seems to be aliasing right now.
            float t = static_cast<float>(msg.data2) / 127.f;
            float minDiameter = 1.f;
            float maxDiameter = 4.0f;
            m_synth.setMalletHeadDiameter(minDiameter + (maxDiameter - 
                        minDiameter) * t);
        }
        else if (msg.data1 == 24)
        {
            m_synth.setMalletHeadStiffness(static_cast<float>(
                        msg.data2) / 127.f);
        }
        else if (msg.data1 == 25)
        {
            float t = static_cast<float>(msg.data2) / 127.f;
            float start = 0.5f;
            float end = 4.f;
            m_synth.setMotorFrequency(start + (end - start) * t);
        }
    }
}

void SynthEngine::renderBlock(float* out, unsigned long N, int outChannels)
{
    MidiMessage msg;

    while (m_midiMessages.pop(msg))
        handleMessage(msg);

    m_synth.renderBlock(out, N, outChannels);
}

int SynthEngine::audioCallback(const void* inputBuffer,
                               void *outputBuffer, 
                               unsigned long framesPerBuffer,
                               const PaStreamCallbackTimeInfo *timeInfo,
                               PaStreamCallbackFlags statusFlags,
                               void *userData)
{
    (void)inputBuffer;
    float* out = static_cast<float*>(outputBuffer);

    SynthEngine* synthEngine = static_cast<SynthEngine*>(userData);
    synthEngine->renderBlock(out, framesPerBuffer, 1);

    return paContinue;
}
};
