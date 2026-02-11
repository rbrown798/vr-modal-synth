#pragma once


namespace ModalSynth
{
class Dif2  // Second difference
{
private:
    float m_x1{};
    float m_x2{};

public:
    void processBlock(float* in, float* out, unsigned int length);
};
};
