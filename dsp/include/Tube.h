#pragma once

#include "DelayLine.h"


namespace ModalSynth
{
class Tube
{
private:
    DelayLine           m_delayLine;
    float               m_feedbackGain{0.99f};
    std::vector<float>& m_lfoBuffer;
    bool                m_isLfoOn{false};

public:
    Tube(std::vector<float>& lfoBuffer);
    void initialize(float sampleRate);
    void setFreq(float freq);
    void processBlock(float* in, float* out, unsigned int N);
    void clear();
};
};
