#include <cassert>
#include "Synthesizer.h"
#include "Utils.h"
#include <iostream>


namespace ModalSynth
{
Synthesizer::Synthesizer()
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i].reset(new Voice(m_lfoBuffer, m_tempBuffers));
}

void Synthesizer::initialize(float sampleRate, int dspBufferSize)
{
    BarResonator::initModeShapes();

    for (auto& buffer : m_tempBuffers)
        buffer.resize(dspBufferSize);

    m_lfoBuffer.resize(dspBufferSize);

    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->initialize(sampleRate);

    m_tubeLfo.initialize(sampleRate);
}

void Synthesizer::noteOn(int note, float velocity, float position)
{
    assert((0 < note) && (note < 128));

    for (int i = 0; i < MAX_NUM_VOICES; i++)
    {
        m_voices[i]->incrementTimestamp();
    }

    Voice* voice = m_activeNotes[note];
    if (voice != nullptr)
    {
        voice->retrigger(velocity, position);
        return;
    }

    voice = getAvailableVoice();
    if (voice == nullptr)
    {
        voice = getVoiceToSteal();
        int noteToSteal = voice->getNote();
        m_activeNotes[noteToSteal] = nullptr;
    }

    voice->noteOn(note, velocity, position);
    m_activeNotes[note] = voice;
}

void Synthesizer::noteOff(int note)
{
    assert((0 < note) && (note < 128));

    if (m_activeNotes[note] != nullptr)
    {
        m_activeNotes[note]->noteOff();
        m_activeNotes[note] = nullptr;
    }
}

Voice* Synthesizer::getAvailableVoice()
{
    // Return the oldest available voice
    Voice* voice{};
    int maxTimestamp = -1;

    for (int i = 0; i < MAX_NUM_VOICES; i++)
    {
        if ((m_voices[i]->isVoiceActive() == false) && 
                (m_voices[i]->getTimestamp() > maxTimestamp))
        {
            maxTimestamp = m_voices[i]->getTimestamp();
            voice = m_voices[i].get();
        }
    }
    return voice;
}

Voice* Synthesizer::getVoiceToSteal()
{
    Voice* voice1{};
    Voice* voice2{};

    int maxTimestamp1 = -1;
    int maxTimestamp2 = -1;

    // get oldest 2 voices
    for (int i = 0; i < MAX_NUM_VOICES; i++)
    {
        int currentTimestamp = m_voices[i]->getTimestamp();

        if (currentTimestamp > maxTimestamp1)
        {
            maxTimestamp2 = maxTimestamp1;
            maxTimestamp1 = currentTimestamp;

            voice2 = voice1;
            voice1 = m_voices[i].get();
        }
        else if ((currentTimestamp > maxTimestamp2) && 
                (currentTimestamp < maxTimestamp1))
        {
            maxTimestamp2 = currentTimestamp;
            voice2 = m_voices[i].get();
        }
    }
    if (voice2 == nullptr)
    {
        return voice1;
    }

    return (voice1->getNote() > voice2->getNote()) ? voice1 : voice2;
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

    float contactModulus = MIN_CONTACT_MODULUS + malletHeadStiffness * 
                                (MAX_CONTACT_MODULUS - MIN_CONTACT_MODULUS);

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

void Synthesizer::setBarMetallic(float value)
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setBarMetallic(value);
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

void Synthesizer::setPedalValue(float pedalValue)
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setPedalValue(pedalValue);
}

void Synthesizer::setLeftEarPosition(const Vector3& leftEarPosition,
        bool immediate)
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setLeftEarPosition(leftEarPosition, immediate);
}

void Synthesizer::setLeftEarDirection(const Vector3& leftEarDirection,
        bool immediate)
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setLeftEarDirection(leftEarDirection, immediate);
}

void Synthesizer::setRightEarPosition(const Vector3& rightEarPosition,
        bool immediate)
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setRightEarPosition(rightEarPosition, immediate);
}

void Synthesizer::setRightEarDirection(const Vector3& rightEarDirection,
        bool immediate)
{
    for (int i{ 0 }; i < MAX_NUM_VOICES; i++)
        m_voices[i]->setRightEarDirection(rightEarDirection, immediate);
}
};
