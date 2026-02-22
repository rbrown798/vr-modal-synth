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
    DelayLine       m_leftDelay;
    DelayLine       m_rightDelay;
    LowPass1P       m_leftDelayTimeLPF;
    LowPass1P       m_rightDelayTimeLPF;
    float           m_leftDelayTime{};
    float           m_rightDelayTime{};
    Vector3         m_sourcePosition;
    Vector3         m_leftEarPosition;
    Vector3         m_leftEarDirection;
    Vector3         m_rightEarPosition;
    Vector3         m_rightEarDirection;
    float           m_leftGain{};
    float           m_rightGain{};

    void update();

public:
    Spatializer() = default;
    void initialize(float sampleRate);
    void processBlock(float* in, float* out, unsigned long length);
    void setSourcePosition(Vector3 sourcePosition, bool immediate=false);
    void setLeftEarPosition(Vector3 leftEarPosition, bool immediate=false);
    void setLeftEarDirection(Vector3 leftEarDirection, bool immediate=false);
    void setRightEarPosition(Vector3 rightEarPosition, bool immediate=false);
    void setRightEarDirection(Vector3 rightEarDirection, bool immediate=false);
    void clear();
};
};
