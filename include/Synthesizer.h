#pragma once

#include <vector>
#include <memory>
#include "Voice.h"
#include "SinOsc.h"


namespace ModalSynth
{
const int MAX_NUM_VOICES{ 10 };
const float DEFAULT_MALLET_HEAD_DENSITY{ 8000.f };

class Synthesizer
{
private:
    std::unique_ptr<Voice>  m_voices[MAX_NUM_VOICES]{};
    Voice*                  m_activeKeys[128]{};
    int                     m_currentVoice{};
    std::vector<float>      m_tempBuffer1;
    std::vector<float>      m_tempBuffer2;
    std::vector<float>      m_tempBuffer3;

    std::vector<float>      m_lfoBuffer;
    SinOsc                  m_tubeLfo{1.f};
    bool                    m_isMotorOn{true};

    float m_malletHeadDensity{};
    float m_malletHeadVolume{}; 
    void setMass();

public:
    Synthesizer();
    void initialize(float sampleRate, int dspBufferSize);
    void noteOn(int note, float velocity, float position);
    void noteOff(int note);
    void renderBlock(float* outBuffer, unsigned int length, int outChannels);
    void setMalletHeadDiameter(float malletHeadDiameter);
    void setMalletHeadDensity(float malletHeadDensity);
    void setMalletHeadStiffness(float malletHeadStiffness);
    void setBarTimbre(float barTimbre);
    void setBarDamping(float barDamping);
    void setTubeOn(bool isTubeOn);
    void setMotorOn(bool isMotorOn);
    void setMotorFrequency(float motorFrequency);
};
};
