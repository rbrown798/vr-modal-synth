#pragma once

#include <cmath>


namespace ModalSynth 
{
class ImpactForce
{
private:
    float m_dt{};
    float m_hertzSpringConstant{};
    float m_contactModulus{};
    float m_malletHeadRadius{};
    float m_malletHeadMass{};
    float m_malletPosition{};
    float m_malletVelocity{};

    void update();

public:
    void initialize(float sampleRate);
    void play(float velocity);
    void renderBlock(float* outbuffer, unsigned int length);
    void setMalletHeadRadius(float malletHeadRadius);
    void setMalletHeadMass(float malletHeadMass);
    void setContactModulus(float contactModulus);
};
};
