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
    m_spatializer(tempBuffer1, tempBuffer2), 
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
    m_barRadiation.setCutoff(200.f);
    m_tubeRadiation.setCutoff(100.f);
    m_malletRadiation.setCutoff(1000.f);

    m_spatializer.initialize(sampleRate);
    m_spatializer.setSourcePosition(Vector3(-3.f, 0.f, 0.f));
    m_spatializer.setLeftEarPosition(Vector3(-0.085f, -0.5, 0.f));
    m_spatializer.setLeftEarDirection(Vector3(-0.66913f, 0.f, 0.74314f));
    m_spatializer.setRightEarPosition(Vector3(0.085f, -0.5f, 0.f));
    m_spatializer.setRightEarDirection(Vector3(0.66913f, 0.f, 0.74314f));
}

void Voice::noteOn(int note, float velocity, float position)
{
    float freq = 440.f * powf(2.f, static_cast<float>(note - 69) / 12.f);
    float pos = static_cast<float>(note - 60) * 0.05f;
    m_spatializer.setSourcePosition(Vector3(pos, 0.f, 0.f));

    m_modalBank.clear();
    m_modalBank.setFreq(freq);
    m_modalBank.setPosition(position);
    m_modalBank.setDamping(m_barDamping);

    m_tube.clear();
    m_tube.setFreq(freq);

    m_impactForce.play(velocity);
}

void Voice::noteOff()
{
    // temporary solution
    //m_modalBank.clear();
    m_modalBank.setDamping(std::max(0.4f, m_barDamping));
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

    // bar and mallet should output surface velocity, not positon.
    // both are LTI, so differentiator can be applied beforehand.
    m_differentiator.processBlock(temp1, temp1, length);

    m_modalBank.processBlock(temp1, temp2, length);
 
    if (m_isTubeOn)
    {
        m_tube.processBlock(temp2, temp3, length);
    }
    m_tubeRadiation.processBlock(temp3, temp3, length);
    gain(temp3, temp3, length, 0.03f);  // Resonator gain
                                        
    m_barRadiation.processBlock(temp2, temp2, length);
    gain(temp2, temp2, length, 0.8f);   // Bar gain
                                        
    mix(temp2, temp3, temp2, length);

    m_malletSpring.processBlock(temp1, temp1, length);
    m_malletRadiation.processBlock(temp1, temp1, length); 

    gain(temp1, temp1, length, 30000.f); // Mallet gain
    mix(temp1, temp1, temp2, length);

    gain(temp1, temp3, length, 0.005f);
    
    m_spatializer.processBlock(temp3, outBuffer, length);

    //for (unsigned int n{ 0 }; n < length; n++)
    //{
    //    for (int ch{ 0 }; ch < outChannels; ch++)
    //    {
    //        outBuffer[n * outChannels + ch] += temp1[n];
    //    }
    //}
}

void Voice::setBarTimbre(float barTimbre)
{
    m_modalBank.setTimbre(barTimbre);
}

void Voice::setBarDamping(float barDamping)
{
    // TODO: Fix this, probably will cause problems for note off
    m_barDamping = barDamping;
    m_modalBank.setDamping(barDamping);
}

void Voice::setMalletHeadRadius(float malletHeadRadius)
{
    m_impactForce.setMalletHeadRadius(malletHeadRadius);
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
};
