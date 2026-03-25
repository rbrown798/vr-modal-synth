#include "DipoleRadiation.h"


namespace ModalSynth
{
void DipoleRadiation::processBlock(float *in, float *out, unsigned int length)
{
    for (unsigned int i{ 0 }; i < length; i++)
    {
        float x = in[i];

        out[i] = x - 3.f * m_x1 + 3.f * m_x2 - m_x3;

        m_x3 = m_x2;
        m_x2 = m_x1;
        m_x1 = x;
    }
}
}
