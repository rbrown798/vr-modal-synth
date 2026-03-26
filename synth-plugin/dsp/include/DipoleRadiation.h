#pragma once

namespace ModalSynth
{
class DipoleRadiation
{
private:
    float m_gain{};
    float m_x1{};
    float m_x2{};
    float m_x3{};

public:
    void initialize(float sampleRate);
    void processBlock(float *in, float *out, unsigned int length);
};
};
