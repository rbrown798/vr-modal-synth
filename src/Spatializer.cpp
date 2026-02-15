#include "Spatializer.h"
#include "Utils.h"


namespace ModalSynth
{
Spatializer::Spatializer(std::vector<float>& tempBuffer1, 
        std::vector<float>& tempBuffer2) : m_tempBuffer1{tempBuffer1}, 
    m_tempBuffer2{tempBuffer2}
{
}

void Spatializer::update()
{
    Vector3 leftResultant = m_sourcePosition - m_leftEarPosition;
    float leftDistance = leftResultant.norm();
    m_leftGain = (0.5f / leftDistance) * (1.f + leftResultant.dot(
                m_leftEarDirection) / leftDistance);

    m_leftDelay.setDelay(leftDistance * 0.00291545);

    Vector3 rightResultant = m_sourcePosition - m_rightEarPosition;
    float rightDistance = rightResultant.norm();
    m_rightGain = (0.5f / rightDistance) * (1.f + rightResultant.dot(
                m_rightEarDirection) / rightDistance);

    m_rightDelay.setDelay(rightDistance * 0.00291545);
}

void Spatializer::initialize(float sampleRate)
{
    m_leftDelay.initialize(sampleRate);
    m_rightDelay.initialize(sampleRate);
}

void Spatializer::processBlock(float* in, float* out, unsigned long length)
{
    m_leftDelay.processBlock(in, m_tempBuffer1.data(), length);
    gain(m_tempBuffer1.data(), m_tempBuffer1.data(), length, m_leftGain);

    m_rightDelay.processBlock(in, m_tempBuffer2.data(), length);
    gain(m_tempBuffer2.data(), m_tempBuffer2.data(), length, m_rightGain);

    for (unsigned long i{ 0 }; i < length; i++)
    {
        out[2 * i] += m_tempBuffer1[i];
        out[2 * i + 1] += m_tempBuffer2[i];
    }
}

void Spatializer::setSourcePosition(Vector3 sourcePosition)
{
    m_sourcePosition = sourcePosition;
    update();
}

void Spatializer::setLeftEarPosition(Vector3 leftEarPosition)
{
    m_leftEarPosition = leftEarPosition;
    update();
}

void Spatializer::setLeftEarDirection(Vector3 leftEarDirection)
{
    m_leftEarDirection = leftEarDirection;
    update();
}

void Spatializer::setRightEarPosition(Vector3 rightEarPosition)
{
    m_rightEarPosition = rightEarPosition;
    update();
}

void Spatializer::setRightEarDirection(Vector3 rightEarDirection)
{
    m_rightEarDirection = rightEarDirection;
    update();
}
};
