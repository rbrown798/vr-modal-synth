#pragma once

#include <vector>
#include "Vector3.h"
#include "DelayLine.h"
#include "LowPass1P.h"


namespace ModalSynth
{
class Spatializer
{
private:
    DelayLine               m_leftDelay;
    DelayLine               m_rightDelay;
    LowPass1P               m_leftDelayTimeLPF;
    LowPass1P               m_rightDelayTimeLPF;
    float                   m_leftDelayTime;
    float                   m_rightDelayTime;
    Vector3                 m_sourcePosition;
    Vector3                 m_leftEarPosition;
    Vector3                 m_leftEarDirection;
    Vector3                 m_rightEarPosition;
    Vector3                 m_rightEarDirection;
    float                   m_leftGain;
    float                   m_rightGain;
    std::vector<float>&     m_tempBuffer1;
    std::vector<float>&     m_tempBuffer2;

    void update();

public:
    Spatializer(std::vector<float>& tempBuffer1, 
            std::vector<float>& tempBuffer2);
    void initialize(float sampleRate);
    void processBlock(float* in, float* out, unsigned long length);
    void setSourcePosition(Vector3 sourcePosition, bool immediate=true);
    void setLeftEarPosition(Vector3 leftEarPosition, bool immediate=true);
    void setLeftEarDirection(Vector3 leftEarDirection, bool immediate=true);
    void setRightEarPosition(Vector3 rightEarPosition, bool immediate=true);
    void setRightEarDirection(Vector3 rightEarDirection, bool immediate=true);
    void clear();
};
};
