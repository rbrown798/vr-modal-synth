#include "DipoleRadiation.h"


namespace ModalSynth
{
void DipoleRadiation::initialize(float sampleRate)
{
    m_gain = sampleRate * sampleRate * sampleRate;
}

void DipoleRadiation::clear()
{
    m_x1 = 0.f;
    m_x2 = 0.f;
    m_x3 = 0.f;
}

void DipoleRadiation::processBlock(float *in, float *out, unsigned int length)
{
    for (unsigned int i{ 0 }; i < length; i++)
    {
        float x = in[i];

        out[i] = m_gain * (x - 3.f * m_x1 + 3.f * m_x2 - m_x3);

        m_x3 = m_x2;
        m_x2 = m_x1;
        m_x1 = x;
    }
}
}
