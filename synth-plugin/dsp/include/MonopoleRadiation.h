#pragma once

namespace ModalSynth
{
class MonopoleRadiation
{
private:
    float m_gain{};
    float m_x1{};
    float m_x2{};

public:
    void initialize(float sampleRate);
    void processBlock(float *in, float *out, unsigned int length);
};
};
