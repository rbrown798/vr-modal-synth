#pragma once

#include "DelayLine.h"


namespace ModalSynth
{
const float TUBE_FEEDBACK_GAIN{ 0.9f }; // 0.99

class Tube
{
private:
    DelayLine           m_delayLine;
    float               m_feedbackGain{ TUBE_FEEDBACK_GAIN };
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
