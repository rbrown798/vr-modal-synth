#include <cassert>
#include <cmath>
#include "HighPass1P.h"
#include "Utils.h"


namespace ModalSynth
{
void HighPass1P::initialize(float sampleRate)
{
    m_sampleRate = sampleRate;
    setCoefs();
}

void HighPass1P::processBlock(float* in, float* out, unsigned long length)
{
    for (unsigned long i{ 0 }; i < length; i++)
    {
        float y = m_b0 * in[i] + m_b1 * m_x1 - m_a1 * m_y1;

        m_x1 = in[i];
        m_y1 = y;

        out[i] = y;
    }
}

void HighPass1P::setCutoff(float cutoffFreq)
{
    assert((0.f < cutoffFreq) && (cutoffFreq < m_sampleRate / 2.f));
    m_cutoffFreq = cutoffFreq;
    setCoefs();
}

void HighPass1P::clear()
{
    m_x1 = 0.f;
    m_y1 = 0.f;
}

void HighPass1P::setCoefs()
{
    float omega = 2.f * PI * m_cutoffFreq;
    m_a1 = -expf(-omega / m_sampleRate);
    m_b0 = 0.5f * (1.f - m_a1);
    m_b1 = -m_b0;
}
};
