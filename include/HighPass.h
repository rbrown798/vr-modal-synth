#pragma once


namespace ModalSynth
{
class HighPass
{
private:
    float m_x1{};

public:
    void processBlock(float *in, float *out, unsigned int length);
};
};
