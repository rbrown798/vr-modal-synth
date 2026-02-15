#include <cmath>
#include <iostream>
#include <cassert>
#include "DelayLine.h"
#include "Utils.h"


namespace ModalSynth
{
void DelayLine::initialize(float sampleRate)
{
    m_sampleRate = sampleRate;
    int maxDelaySamples = static_cast<int>(ceilf(sampleRate * 
                                                        MAX_DELAY_SECONDS));

    size_t delayBufferSize = static_cast<size_t>(nextPowerOf2(maxDelaySamples));
    m_mask = delayBufferSize - 1;

    m_delayBuffer.resize(delayBufferSize);
    clear();
}

void DelayLine::setDelay(float delayInSeconds)
{
    float delayInSamplesf = delayInSeconds * m_sampleRate;
    assert(delayInSamplesf < static_cast<float>(m_delayBuffer.size()));

    float delayInSamplesFloor = floorf(delayInSamplesf);
    m_frac = delayInSamplesf - delayInSamplesFloor;

    size_t delayInSamples = static_cast<size_t>(delayInSamplesFloor);

    m_readIndex = (m_writeIndex - delayInSamples) & m_mask;
}

void DelayLine::write(float x)
{
    m_delayBuffer[m_writeIndex] = x;
    m_writeIndex = (m_writeIndex + 1) & m_mask;
}

float DelayLine::read()
{
    float y = m_delayBuffer[m_readIndex];
    size_t lastIndex = (m_readIndex - 1) & m_mask;

    float y1 = m_delayBuffer[lastIndex];

    m_readIndex = (m_readIndex + 1) & m_mask;
    return (1.0f - m_frac) * y1 + m_frac * y;
}

void DelayLine::processBlock(float* in, float* out, unsigned long length)
{
    for (unsigned long i{ 0 }; i < length; i++)
    {
        write(in[i]);
        out[i] = read();
    }
}

void DelayLine::clear()
{
    std::fill(m_delayBuffer.begin(), m_delayBuffer.end(), 0.0f);
}
};
