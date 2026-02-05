#pragma once

#include <memory>
#include <vector>
#include "ImpactForce.h"
#include "ModalBank.h"
#include "MalletSpring.h"
#include "Tube.h"
#include "HighPass.h"


namespace ModalSynth
{
class Voice
{
private:
    ImpactForce             m_impactForce;
    ModalBank               m_modalBank;
    MalletSpring            m_malletSpring;
    Tube                    m_tube;
    bool                    m_isTubeOn{true};
    HighPass                m_highPass;
    std::vector<float>&     m_tempBuffer1;
    std::vector<float>&     m_tempBuffer2;
    std::vector<float>&     m_tempBuffer3;
    float                   m_barDamping{};

public:
    Voice(std::vector<float>& lfoBuffer, std::vector<float>& tempBuffer1, 
            std::vector<float>& tempBuffer2, std::vector<float>& tempBuffer3); 

    void initialize(float sampleRate);
    void update();
    void noteOn(int note, float velocity, float position);
    void noteOff();
    void setBarTimbre(float barTimbre);
    void setBarDamping(float barDamping);
    void setMalletHeadRadius(float malletHeadRadius);
    void setMalletHeadMass(float malletHeadMass);
    void setContactModulus(float contactModulus);
    void renderBlock(float* outBuffer, unsigned int length, int outChannels);
    void setTube(bool isTubeOn);
};
};
