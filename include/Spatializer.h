#pragma once

#include <vector>
#include "Vector3.h"
#include "DelayLine.h"


namespace ModalSynth
{
class Spatializer
{
private:
    DelayLine               m_leftDelay;
    DelayLine               m_rightDelay;
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
    void setSourcePosition(Vector3 sourcePosition);
    void setLeftEarPosition(Vector3 leftEarPosition);
    void setLeftEarDirection(Vector3 leftEarDirection);
    void setRightEarPosition(Vector3 rightEarPosition);
    void setRightEarDirection(Vector3 rightEarDirection);
};
};
