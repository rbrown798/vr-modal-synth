#pragma once

#include <vector>

namespace ModalSynth
{
const float MAX_DELAY_SECONDS{ 0.025f };

class DelayLine
{
private:
    std::vector<float>  m_delayBuffer;
    float               m_sampleRate{};
    size_t              m_readIndex{};
    size_t              m_writeIndex{};
    size_t              m_mask{};
    float               m_frac{};
public:
    DelayLine() = default;
    void initialize(float sampleRate);
    void setDelay(float delayInSeconds);
    void write(float x);
    float read();
    void processBlock(float* in, float* out, unsigned long length);
    void clear();
};
};

