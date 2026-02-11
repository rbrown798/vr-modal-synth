#include "Dif2.h"


namespace ModalSynth 
{
void Dif2::processBlock(float* in, float* out, unsigned int length)
{
    for (unsigned int i{ 0 }; i < length; i++)
    {
        out[i] = 0.25f * in[i] - 0.5f * m_x1 + 0.25f * m_x2;
        m_x2 = m_x1;
        m_x1 = in[i];
    }
}
};
