#include "MonopoleRadiation.h"


namespace ModalSynth
{
void MonopoleRadiation::initialize(float sampleRate)
{
    m_gain = sampleRate * sampleRate;
}

void MonopoleRadiation::processBlock(float *in, float *out, unsigned int length)
{
    for (unsigned int i{ 0 }; i < length; i++)
    {
        float x = in[i];

        out[i] = m_gain * (x - 2.f * m_x1 + m_x2);

        m_x2 = m_x1;
        m_x1 = x;
    }
}
}
