#include <cmath>
#include <iostream> // testing
#include "MalletSpring.h"


namespace ModalSynth
{
void MalletSpring::initialize(float sampleRate) 
{
    m_sampleRate = sampleRate;
    m_samplingInterval = 1.0f / sampleRate;
    
    setCoefs();
}

void MalletSpring::setCoefs()
{
    float undampedAngularFreq = sqrtf(m_springConstant / m_mass);

    float angularFreq = undampedAngularFreq * 
        sqrtf(1.0f - m_dampingRatio * m_dampingRatio);

    std::cout << "norm angularFreq: " << angularFreq * m_samplingInterval 
        << std::endl;

    float poleRadius = expf(-m_dampingRatio * undampedAngularFreq * 
                                    m_samplingInterval);

    m_a1 = -2.0f * poleRadius * cosf(angularFreq * m_samplingInterval);
    m_a2 = poleRadius * poleRadius;
    m_b0 = (1.f + m_a1 + m_a2) / m_springConstant;
}

void MalletSpring::setMass(float mass)
{
    m_mass = mass;
    setCoefs();
}

void MalletSpring::processBlock(float *in, float *out, unsigned int length)
{
    float y;
    for (unsigned int i = 0; i < length; i++)
    {
        y = m_b0 * in[i] - m_a1 * m_y1 - m_a2 * m_y2;

        m_y2 = m_y1;
        m_y1 = y;

        out[i] = y;     
    }
}
};
