#include <algorithm>
#include "Spatializer.h"
#include "Utils.h"


namespace ModalSynth
{
void Spatializer::update()
{
    Vector3 leftResultant = m_sourcePosition - m_leftEarPosition;
    float leftDistance = leftResultant.norm();
    m_leftGain = (0.5f / leftDistance) * (1.f + leftResultant.dot(
                m_leftEarDirection) / leftDistance);

    // Don't let gain exceed 1. First scale down so that a gain of 1 occurs 
    // 10 cm from the source (otherwise the max gain will be one meter away) 
    m_leftGain = std::min(0.1f * m_leftGain, 1.f);

    // Divide distance by speed of sound
    m_leftDelayTime = leftDistance * 0.00291545f;

    Vector3 rightResultant = m_sourcePosition - m_rightEarPosition;
    float rightDistance = rightResultant.norm();
    m_rightGain = (0.5f / rightDistance) * (1.f + rightResultant.dot(
                m_rightEarDirection) / rightDistance);

    m_rightGain = std::min(0.1f * m_rightGain, 1.f);

    m_rightDelayTime = rightDistance * 0.00291545f;
}

void Spatializer::initialize(float sampleRate)
{
    m_leftDelay.initialize(sampleRate);
    m_rightDelay.initialize(sampleRate);
    m_leftDelayTimeLPF.initialize(sampleRate);
    m_rightDelayTimeLPF.initialize(sampleRate);
    m_leftDelayTimeLPF.setCutoff(30.f);
    m_rightDelayTimeLPF.setCutoff(30.f);
}

void Spatializer::processBlock(float* in, float* out, unsigned long length)
{
    for (unsigned long i{ 0 }; i < length; i++)
    {
        float leftDelayTime = m_leftDelayTimeLPF.processSample(m_leftDelayTime);
        float rightDelayTime = m_rightDelayTimeLPF.processSample(
                                                        m_rightDelayTime);
        m_leftDelay.setDelay(leftDelayTime);
        m_rightDelay.setDelay(rightDelayTime);

        m_leftDelay.write(in[i]);
        m_rightDelay.write(in[i]);

        out[2 * i] += m_leftGain * m_leftDelay.read();
        out[2 * i + 1] += m_rightGain * m_rightDelay.read();
    }
}

void Spatializer::setSourcePosition(Vector3 sourcePosition, bool immediate)
{
    m_sourcePosition = sourcePosition;
    update();

    if (immediate == true)
    {
        m_leftDelayTimeLPF.set(m_leftDelayTime);
        m_rightDelayTimeLPF.set(m_rightDelayTime);
    }
}

void Spatializer::setLeftEarPosition(Vector3 leftEarPosition, bool immediate)
{
    m_leftEarPosition = leftEarPosition;
    update();

    if (immediate == true)
    {
        m_leftDelayTimeLPF.set(m_leftDelayTime);
        m_rightDelayTimeLPF.set(m_rightDelayTime);
    }
}

void Spatializer::setLeftEarDirection(Vector3 leftEarDirection, bool immediate)
{
    m_leftEarDirection = leftEarDirection;
    update();

    if (immediate == true)
    {
        m_leftDelayTimeLPF.set(m_leftDelayTime);
        m_rightDelayTimeLPF.set(m_rightDelayTime);
    }
}

void Spatializer::setRightEarPosition(Vector3 rightEarPosition, bool immediate)
{
    m_rightEarPosition = rightEarPosition;
    update();

    if (immediate == true)
    {
        m_leftDelayTimeLPF.set(m_leftDelayTime);
        m_rightDelayTimeLPF.set(m_rightDelayTime);
    }
}

void Spatializer::setRightEarDirection(Vector3 rightEarDirection, 
        bool immediate)
{
    m_rightEarDirection = rightEarDirection;
    update();

    if (immediate == true)
    {
        m_leftDelayTimeLPF.set(m_leftDelayTime);
        m_rightDelayTimeLPF.set(m_rightDelayTime);
    }
}

void Spatializer::clear()
{
    m_leftDelay.clear();
    m_rightDelay.clear();
    m_leftDelayTimeLPF.clear();
    m_rightDelayTimeLPF.clear();
}
};
