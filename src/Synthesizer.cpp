#include <cassert>
#include "Synthesizer.h"
#include "Utils.h"

// TODO: Make sure mass spring damper is stable!!! Analyze stability

namespace ModalSynth
{
Synthesizer::Synthesizer()
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i].reset(new Voice(m_lfoBuffer, m_tempBuffer1, m_tempBuffer2, 
                    m_tempBuffer3));
}

void Synthesizer::initialize(float sampleRate, int dspBufferSize)
{
    ModalBank::initModeShapes();

    m_tempBuffer1.resize(dspBufferSize);
    m_tempBuffer2.resize(dspBufferSize);
    m_tempBuffer3.resize(dspBufferSize);
    m_lfoBuffer.resize(dspBufferSize);

    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->initialize(sampleRate);

    m_tubeLfo.initialize(sampleRate);
}

void Synthesizer::noteOn(int note, float velocity, float position)
{
    assert((0 < note) && (note < 128));

    m_voices[m_currentVoice]->noteOn(note, velocity, position);
    m_activeKeys[note] = m_voices[m_currentVoice].get();
    m_currentVoice = (m_currentVoice + 1) % MAX_NUM_VOICES;
}

void Synthesizer::noteOff(int note)
{
    assert((0 < note) && (note < 128));

    if (m_activeKeys[note] != nullptr)
        m_activeKeys[note]->noteOff();
    m_activeKeys[note] = nullptr;
}

void Synthesizer::renderBlock(float* outBuffer, unsigned int length, 
                              int outChannels)
{
    if (m_isMotorOn == true)
    {
        m_tubeLfo.renderBlock(m_lfoBuffer.data(), length);

        for (unsigned int i = 0; i < length; i++)
            m_lfoBuffer[i] = fabsf(m_lfoBuffer[i]);
    }
    else
    {
        std::fill(m_lfoBuffer.begin(), m_lfoBuffer.end(), 1.f);
    }

    std::fill(outBuffer, outBuffer + length * outChannels, 0.0f);

    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
    {
        m_voices[i]->renderBlock(outBuffer, length, outChannels);
    }
}

void Synthesizer::setMalletHeadDiameter(float malletHeadDiameter)
{
    float radius = 0.5f * malletHeadDiameter * 0.01f;

    m_malletHeadVolume = (4.f / 3.f) * PI * radius * radius * radius;
    //float mass = m_malletHeadDensity * m_malletHeadVolume;

    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setMalletHeadRadius(radius);

    setMass();
}

void Synthesizer::setMalletHeadDensity(float malletHeadDensity)
{
    m_malletHeadDensity = malletHeadDensity;
    setMass();
}

void Synthesizer::setMass()
{
    float mass = m_malletHeadDensity * m_malletHeadVolume;

    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setMalletHeadMass(mass);
}

void Synthesizer::setMalletHeadStiffness(float malletHeadStiffness)
{
    assert((malletHeadStiffness >= 0) && (malletHeadStiffness <= 1));

    //float maxContactModulus = 80e+09f;
    //float minContactModulus = 480000099.f;

	// Testing different values Feb 7, 2026:
    float minContactModulus = 1.32e+07f;  // soft rubber + aluminum
	float maxContactModulus = 2.51e+09f;  // ABS + aluminum

    float contactModulus = minContactModulus + malletHeadStiffness * 
                                (maxContactModulus - minContactModulus);

    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setContactModulus(contactModulus);
}

void Synthesizer::setBarTimbre(float barTimbre)
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setBarTimbre(barTimbre);
}

void Synthesizer::setBarDamping(float barDamping)
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setBarDamping(barDamping);
}

void Synthesizer::setTubeOn(bool isTubeOn)
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setTubeOn(isTubeOn);
}

void Synthesizer::setMotorOn(bool isMotorOn)
{
    m_isMotorOn = isMotorOn;
}

void Synthesizer::setMotorFrequency(float motorFrequency)
{
    m_tubeLfo.setFreq(motorFrequency);
}
};
