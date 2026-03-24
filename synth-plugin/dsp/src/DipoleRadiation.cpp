#include "DipoleRadiation.h"


namespace ModalSynth
{
void DipoleRadiation::processBlock(float *in, float *out, unsigned int length)
{
    for (unsigned int i{ 0 }; i < length; i++)
    {
        out[i] = in[i] - 3.f * m_x1 + 3.f * m_x2 - m_x3;
        m_x3 = m_x2;
        m_x2 = m_x1;
        m_x1 = in[i];
    }
}
};
