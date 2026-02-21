#include <cassert>
#include <cmath>
#include "LowPass1P.h"
#include "Utils.h"


namespace ModalSynth
{
void LowPass1P::initialize(float sampleRate)
{
    m_sampleRate = sampleRate;
    setCoefs();
}

void LowPass1P::processBlock(float* in, float* out, unsigned long length)
{
    for (unsigned long i{ 0 }; i < length; i++)
    {
        float y = m_b0 * in[i] - m_a1 * m_y1;

        m_y1 = y;

        out[i] = y;
    }
}

void LowPass1P::setCutoff(float cutoffFreq)
{
    assert((0.f < cutoffFreq) && (cutoffFreq < m_sampleRate / 2.f));
    m_cutoffFreq = cutoffFreq;
    setCoefs();
}

void LowPass1P::clear()
{
    m_y1 = 0.f;
}

void LowPass1P::setCoefs()
{
    float omega = 2.f * PI * m_cutoffFreq;
    m_a1 = -expf(-omega / m_sampleRate);
    m_b0 = 1.f - fabsf(m_a1);
}
};
