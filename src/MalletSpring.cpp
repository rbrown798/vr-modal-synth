#include <cmath>
#include "MalletSpring.h"

// Alternatively could make stuff static?

namespace ModalSynth
{
void MalletSpring::initialize(float sampleRate) 
{
    m_sampleRate = sampleRate;
    m_samplingInterval = 1.0f / sampleRate;
    
    setCoefs();
}

//void MalletSpring::setCoefs()
//{
//    float T = 1.0f / m_sampleRate;
//
//    float a = m_malletProperties.getMass() / (T * T) + 
//                m_malletProperties.getDampingConstant() / (2.0f * T);
//
//    float b = m_malletProperties.getSpringConstant() - 
//                m_malletProperties.getMass() / (T * T) * 2.0f;
//
//    float c = m_malletProperties.getMass() / (T * T) - 
//                m_malletProperties.getDampingConstant() / (2.0f * T);
//
//    m_b1 = 1.0 / a;
//    m_a1 = b / a;
//    m_a2 = c / a;
//} 

void MalletSpring::setCoefs()
{
    float undampedAngularFreq = sqrtf(m_springConstant / m_mass);

    float angularFreq = undampedAngularFreq * 
        sqrtf(1.0f - m_dampingRatio * m_dampingRatio);

    float poleRadius = expf(-m_dampingRatio * undampedAngularFreq * 
                                    m_samplingInterval);

    m_a1 = -2.0f * poleRadius * cosf(angularFreq * m_samplingInterval);
    m_a2 = poleRadius * poleRadius;
    m_b0 = 1.0f / m_mass;
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
        //y = m_b1 * m_x1 - m_a1 * m_y1 - m_a2 * m_y2;

        /*m_x2 = m_x1;
        m_x1 = *in++;
        m_y2 = m_y1;
        m_y1 = y;*/

        // (*out++) += 0.05f * y;

        y = m_b0 * in[i] - m_a1 * m_y1 - m_a2 * m_y2;

        m_y2 = m_y1;
        m_y1 = y;

        out[i] = y; // += 0.05f * y;
    }
}
};
