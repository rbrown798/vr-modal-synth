#pragma once


namespace ModalSynth
{
const float MALLET_SPRING_CONSTANT{ 5.59e+05f }; 
const float MALLET_DAMPING_RATIO{ 0.072f }; 

class MalletResonator
{
private:
    float m_springConstant{ MALLET_SPRING_CONSTANT };
    float m_dampingRatio{ MALLET_DAMPING_RATIO };
    float m_mass{};
    float m_samplingInterval{};
    float m_radius{};

    float m_sampleRate{};

    float m_a1{};
    float m_a2{};
    float m_b0{};

    float m_y1{};
    float m_y2{};

    void setCoefs();

public:
    MalletResonator() = default;
    void initialize(float sampleRate);
    void setMass(float mass);
    void setRadius(float radius);
    void processBlock(float *in, float *out, unsigned int length);
};
};


