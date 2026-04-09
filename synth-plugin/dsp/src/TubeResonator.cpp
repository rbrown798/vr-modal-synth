#include <cmath>
#include "TubeResonator.h"


namespace ModalSynth
{
TubeResonator::TubeResonator(std::vector<float>& lfoBuffer) : m_lfoBuffer{lfoBuffer}
{
}

void TubeResonator::initialize(float sampleRate)
{
    m_delayLine.initialize(sampleRate);
}

void TubeResonator::setFreq(float freq)
{
    m_delayLine.setDelay(0.5f / freq);
}

void TubeResonator::processBlock(float* in, float* out, unsigned int length)
{
    for (unsigned int i = 0; i < length; i++)
    {
        float lfo = m_lfoBuffer[i];
        float lfoGain = (0.1f + 0.9f * lfo);

        float x = in[i] * lfoGain;
        
        float feedback = -m_feedbackGain * m_delayLine.read();
        m_delayLine.write(x + feedback);

        out[i] = feedback * lfoGain; 
    }
}

void TubeResonator::clear()
{
    m_delayLine.clear();
}
};
