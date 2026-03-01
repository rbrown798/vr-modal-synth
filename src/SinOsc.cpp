#include <cassert>
#include <iostream>
#include <cmath>
#include "SinOsc.h"
#include "Utils.h"


namespace ModalSynth
{
SinOsc::SinOsc(float freq) : m_freq{freq}
{
}

void SinOsc::initialize(float sampleRate)
{
    m_samplingInterval = 1.f / sampleRate;
}

void SinOsc::renderBlock(float* out, unsigned int N)
{
    float phaseIncr = 2.f * PI * m_freq * m_samplingInterval;

    for (unsigned int i{ 0 }; i < N; i++)
    {
        out[i] = sinf(m_phase); 
        m_phase += phaseIncr;

        if (m_phase >= 2.f * PI)
            m_phase -= 2.f * PI;
    }
}

void SinOsc::setFreq(float freq)
{
    assert((freq > 0) && (freq <= 20000));
    m_freq = freq;
}
};
