#pragma once

//#include "MalletProperties.h"

namespace ModalSynth
{
const float MALLET_SPRING_CONSTANT{ 5.59e+05f }; 
const float MALLET_DAMPING_RATIO{ 0.072f }; 

class MalletSpring
{
private:
    float m_springConstant{ MALLET_SPRING_CONSTANT };
    float m_dampingRatio{ MALLET_DAMPING_RATIO };
    float m_mass{};
    float m_samplingInterval{};

    float m_sampleRate{};

    float m_a1{};
    float m_a2{};
    //float m_b1{1.0f};
    float m_b0{};

    //float m_x1{};
    //float m_x2{};
    float m_y1{};
    float m_y2{};

    void setCoefs();

public:
    MalletSpring() = default;
    void initialize(float sampleRate);
    void setMass(float mass);
    void processBlock(float *in, float *out, unsigned int length);
};
};


