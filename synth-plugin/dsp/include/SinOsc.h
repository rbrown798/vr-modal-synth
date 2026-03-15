#pragma once

namespace ModalSynth
{
class SinOsc
{
private:
    float m_freq{};
    float m_samplingInterval{};
    float m_phase{};

public:
    SinOsc(float freq);
    void initialize(float sampleRate);
    void renderBlock(float* out, unsigned int N);
    void setFreq(float freq);
};
};
