#pragma once

#include <vector>
#include <memory>
#include "Voice.h"
#include "SinOsc.h"
#include "Vector3.h"


namespace ModalSynth
{
const int MAX_NUM_VOICES{ 8 };
const float DEFAULT_MALLET_HEAD_DENSITY{ 8000.f };

class Synthesizer
{
private:
    std::unique_ptr<Voice>  m_voices[MAX_NUM_VOICES]{};
    Voice*                  m_activeNotes[128]{};
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
    Voice* getAvailableVoice();
    Voice* getVoiceToSteal();
    void renderBlock(float* outBuffer, unsigned int length, int outChannels);
    void setMalletHeadDiameter(float malletHeadDiameter);
    void setMalletHeadDensity(float malletHeadDensity);
    void setMalletHeadStiffness(float malletHeadStiffness);
    void setBarTimbre(float barTimbre);
    void setBarDamping(float barDamping);
    void setTubeOn(bool isTubeOn);
    void setMotorOn(bool isMotorOn);
    void setMotorFrequency(float motorFrequency);
    void setLeftEarPosition(const Vector3& leftEarPosition, 
            bool immediate=false);
    void setLeftEarDirection(const Vector3& leftEarDirection, 
            bool immediate=false);
    void setRightEarPosition(const Vector3& rightEarPosition, 
            bool immediate=false);
    void setRightEarDirection(const Vector3& rightEarDirection, 
            bool immediate=false);
};
};
