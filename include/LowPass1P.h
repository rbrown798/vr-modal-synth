#pragma once

namespace ModalSynth 
{
class LowPass1P
{
private:
    float m_sampleRate{};
    float m_cutoffFreq{};
    float m_b0{};
    float m_a1{};
    float m_y1{};

    void setCoefs();

public:
    void initialize(float sampleRate);
    void processBlock(float* in, float* out, unsigned long length);
    void setCutoff(float cutoffFreq);
    void clear();
};
};
