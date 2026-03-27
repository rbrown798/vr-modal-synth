#include <immintrin.h>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include "Voice.h"
#include "Utils.h"


namespace ModalSynth
{

Voice::Voice(std::vector<float>& lfoBuffer, std::vector<float>& tempBuffer1, 
        std::vector<float>& tempBuffer2, std::vector<float>& tempBuffer3) : 
    m_tube(lfoBuffer), 
    m_tempBuffer1{tempBuffer1}, m_tempBuffer2{tempBuffer2}, 
    m_tempBuffer3{tempBuffer3}
{
}

void Voice::initialize(float sampleRate)
{
    m_impactForce.initialize(sampleRate);
    m_modalBank.initialize(sampleRate);
    m_malletSpring.initialize(sampleRate);
    m_tube.initialize(sampleRate);

    m_barRadiation.initialize(sampleRate);
    m_tubeRadiation.initialize(sampleRate);
    m_malletRadiation.initialize(sampleRate);

    m_spatializer.initialize(sampleRate);
    m_spatializer.setSourcePosition(Vector3(-3.f, 0.f, 0.f), true);
    m_spatializer.setLeftEarPosition(Vector3(-0.085f, -0.2, 0.6f), true);
    m_spatializer.setLeftEarDirection(Vector3(-0.66913f, 0.f, 0.74314f), true);
    m_spatializer.setRightEarPosition(Vector3(0.085f, -0.2f, 0.6f), true);
    m_spatializer.setRightEarDirection(Vector3(0.66913f, 0.f, 0.74314f), true);
}

void Voice::noteOn(int note, float velocity, float position)
{
    m_note = note;

    float freq = 440.f * powf(2.f, static_cast<float>(note - 69) / 12.f);

    float barXPos = (7.f * static_cast<float>(note / 12) + 
                        NOTE_XPOS_TABLE[note % 12] - NOTE_XPOS_OFFSET) * 
                    (BAR_WIDTH + BAR_PAD);

    float barLength = 4.912847188f / sqrtf(freq);

    float barYPos = 0.5f * barLength;

    if (std::fmodf(NOTE_XPOS_TABLE[note % 12], 1.f) == 0.f)
        barYPos *= -1.f;

    m_spatializer.clear();
    m_spatializer.setSourcePosition(Vector3(barXPos, barYPos, 0.f), true);

    m_modalBank.clear();
    m_modalBank.setFreq(freq);
    m_modalBank.setPosition(position);
    m_modalBank.setDamping(getBarTotalDamping());

    m_tube.clear();
    m_tube.setFreq(freq);

    m_barRadiation.clear();
    m_tubeRadiation.clear();
    m_malletRadiation.clear();

    m_impactForce.play(velocity);

    m_isActive = true;
    m_timestamp = 0;
}

float Voice::getBarTotalDamping() const
{
    return std::min(m_barDamping * (1.f + PEDAL_DAMP_COEF * m_pedalDamping), 
            MAX_DAMPING);
}

void Voice::noteOff()
{
    m_modalBank.setDamping(std::max(0.4f, getBarTotalDamping()));
    m_isActive = false;
}

void Voice::retrigger(float velocity, float position)
{
    m_modalBank.setPosition(position);
    m_impactForce.play(velocity);

    m_timestamp = 0;
}

void Voice::renderBlock(float* outBuffer, unsigned int length, int outChannels)
{ 
    std::fill(m_tempBuffer1.begin(), m_tempBuffer1.end(), 0.0f);
    std::fill(m_tempBuffer2.begin(), m_tempBuffer2.end(), 0.0f);
    std::fill(m_tempBuffer3.begin(), m_tempBuffer3.end(), 0.0f);

    float* temp1 = m_tempBuffer1.data();
    float* temp2 = m_tempBuffer2.data();
    float* temp3 = m_tempBuffer3.data();

    m_impactForce.renderBlock(temp1, length);

    m_modalBank.processBlock(temp1, temp2, temp3, length);
    m_barRadiation.processBlock(temp2, temp2, length);
    gain(temp2, temp2, length, BAR_GAIN);

    if (m_isTubeOn)
    {
        m_tube.processBlock(temp3, temp3, length);
        m_tubeRadiation.processBlock(temp3, temp3, length);
        gain(temp3, temp3, length, TUBE_GAIN);
        mix(temp2, temp3, temp2, length);
    }

    m_malletSpring.processBlock(temp1, temp1, length);
    m_malletRadiation.processBlock(temp1, temp1, length); 
    gain(temp1, temp1, length, MALLET_GAIN);
    mix(temp1, temp1, temp2, length);

    gain(temp1, temp1, length, 0.1f);
    m_spatializer.processBlock(temp1, outBuffer, length);
}

void Voice::setBarTimbre(float barTimbre)
{
    m_modalBank.setTimbre(barTimbre);
}

void Voice::setBarDamping(float barDamping)
{
    m_barDamping = barDamping;

    // Only set if the note is currently playing so it doesn't compete with 
    // note off
    if (m_isActive)
        m_modalBank.setDamping(getBarTotalDamping());
}

void Voice::setBarMetallic(float value)
{
    m_modalBank.setMetallic(value);
}

void Voice::setMalletHeadRadius(float malletHeadRadius)
{
    m_impactForce.setMalletHeadRadius(malletHeadRadius);
    m_malletSpring.setRadius(malletHeadRadius);
}

void Voice::setMalletHeadMass(float malletHeadMass)
{
    m_impactForce.setMalletHeadMass(malletHeadMass);
    m_malletSpring.setMass(malletHeadMass);
}

void Voice::setContactModulus(float contactModulus)
{
    m_impactForce.setContactModulus(contactModulus);
}

void Voice::setTubeOn(bool isTubeOn)
{
    m_isTubeOn = isTubeOn;
}

void Voice::setPedalValue(float pedalValue)
{
    m_pedalDamping = 1.f - pedalValue;
    m_modalBank.setDamping(getBarTotalDamping());
}

void Voice::setSourcePosition(const Vector3& sourcePosition, bool immediate)
{
    m_spatializer.setSourcePosition(sourcePosition, immediate);
}

void Voice::setLeftEarPosition(const Vector3& leftEarPosition, bool immediate)
{
    m_spatializer.setLeftEarPosition(leftEarPosition, immediate);
}

void Voice::setLeftEarDirection(const Vector3& leftEarDirection, bool immediate)
{
    m_spatializer.setLeftEarDirection(leftEarDirection, immediate);
}

void Voice::setRightEarPosition(const Vector3& rightEarPosition, bool immediate)
{
    m_spatializer.setRightEarPosition(rightEarPosition, immediate);
}

void Voice::setRightEarDirection(const Vector3& rightEarDirection, 
        bool immediate)
{
    m_spatializer.setRightEarDirection(rightEarDirection, immediate);
}

bool Voice::isVoiceActive() const
{
    return m_isActive;
}

int Voice::getTimestamp() const
{
    return m_timestamp;
}

void Voice::incrementTimestamp()
{
    m_timestamp++;
}

int Voice::getNote() const
{
    return m_note;
}
};
