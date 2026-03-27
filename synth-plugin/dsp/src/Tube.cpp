#include <cmath>
#include "Tube.h"


namespace ModalSynth
{
Tube::Tube(std::vector<float>& lfoBuffer) : m_lfoBuffer{lfoBuffer}
{
}

void Tube::initialize(float sampleRate)
{
    m_delayLine.initialize(sampleRate);
}

void Tube::setFreq(float freq)
{
    m_delayLine.setDelay(0.5f / freq);
}

void Tube::processBlock(float* in, float* out, unsigned int N)
{
    for (unsigned long i = 0; i < N; i++)
    {
        float lfo = m_lfoBuffer[i];
        float lfoGain = (0.1f + 0.9f * lfo);

        float x = in[i] * lfoGain;
        
        float feedback = -m_feedbackGain * m_delayLine.read();
        m_delayLine.write(x + feedback);

        out[i] = feedback * lfoGain; 
    }
}

void Tube::clear()
{
    m_delayLine.clear();
}
};
