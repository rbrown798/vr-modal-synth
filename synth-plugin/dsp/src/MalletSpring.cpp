#include <cmath>
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

    float poleRadius = expf(-m_dampingRatio * undampedAngularFreq * 
                                    m_samplingInterval);

    m_a1 = -2.0f * poleRadius * cosf(angularFreq * m_samplingInterval);
    m_a2 = poleRadius * poleRadius;
    m_b0 = (m_radius * m_radius) * (1.f + m_a1 + m_a2) / m_springConstant;
    // radius^2 is to make amplitude proportional to surface area
}

void MalletSpring::setMass(float mass)
{
    m_mass = mass;
    setCoefs();
}

void MalletSpring::setRadius(float radius)
{
    m_radius = radius;
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
