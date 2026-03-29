#include <cmath>
#include <iostream>
#include <cassert>
#include <immintrin.h>
#include "ModalBank.h"
#include "Utils.h"


namespace ModalSynth
{
float ModalBank::s_modeShapes[NUM_MODES][MODE_SHAPE_LUT_SIZE]{};

float ModalBank::getModeShape(float knL, float x)
{
    return sinhf(knL * x) + sin(knL * x) + (sin(knL) - sinh(knL))
        / (cosh(knL) - cos(knL)) * (cosh(knL * x) + cos(knL * x));
}

void ModalBank::initModeShapes()
{
    float knLValues[NUM_MODES] = { 4.73004f, 7.8532f, 10.99561f, 14.13717f, 
                                   17.27876f, 20.42035f, 23.56194f, 26.70354f };

    for (int i = 0; i < NUM_MODES; i++)
    {
        for (int j = 0; j < MODE_SHAPE_LUT_SIZE; j++)
        {
            s_modeShapes[i][j] = getModeShape(knLValues[i],
                static_cast<float>(j) /
                static_cast<float>(MODE_SHAPE_LUT_SIZE - 1));
        }

        // Normalize
        float maxValue = 0.0f;
        for (int j = 0; j < MODE_SHAPE_LUT_SIZE; j++)
        {
            if (fabsf(s_modeShapes[i][j]) > maxValue)
            {
                maxValue = fabsf(s_modeShapes[i][j]);
            }
        }

        for (int j = 0; j < MODE_SHAPE_LUT_SIZE; j++)
        {
            s_modeShapes[i][j] /= maxValue;
        }
    }
}

float ModalBank::readModeShapeLerp(int modeNumber, float position)
{
    float indexf = position * static_cast<float>(MODE_SHAPE_LUT_SIZE - 1);
    int index = static_cast<int>(floorf(indexf));
    float frac = indexf - static_cast<float>(index);

    return (1.0f - frac) * s_modeShapes[modeNumber][index] +
        frac * s_modeShapes[modeNumber][index + 1];
}

void ModalBank::initialize(float sampleRate)
{
    m_sampleRate = sampleRate;

    setDamping(0.2f);
    setFreq(880.0f);
    setTimbre(0.0f);

    for (int i = 0; i < NUM_MODES; i++)
    {
        m_outPosGain[i] = s_modeShapes[i][0];
        m_tubePosGain[i] = s_modeShapes[i][NUM_MODES/2];
    }
}

void ModalBank::setTimbre(float timbre)
{
    assert((0 <= timbre) && (timbre <= 1));

    for (int i{ 0 }; i < NUM_MODES; i++)
    {
        if (timbre < 0.5f)
        {
            m_ratios[i] = GLOCKEN_RATIOS[i] + 2.f * timbre * 
                                    (XYLO_RATIOS[i] - GLOCKEN_RATIOS[i]);
        }
        else
        {
            m_ratios[i] = XYLO_RATIOS[i] + 2.f * (timbre - 0.5f) * 
                                    (VIBES_RATIOS[i] - XYLO_RATIOS[i]);
        }
    }

    setCoefs();
}


float ModalBank::cubicPoly(float a, float b, float c, float d, float x)
{
    return ((a * x + b) * x + c) * x + d;
}

float ModalBank::lerp(float a, float b, float t)
{
    assert((0 <= t) && (t <= 1));
    return a + (b - a) * t;
}

void ModalBank::setDamping(float damping)
{
    m_globalDamping = MIN_BAR_GLOBAL_DAMPING + damping * 
                            (MAX_BAR_GLOBAL_DAMPING - MIN_BAR_GLOBAL_DAMPING);
}

void ModalBank::setMetallic(float value)
{
    m_overtoneDamping = value * MIN_BAR_OVERTONE_DAMPING + (1.f - value) *
                                    MAX_BAR_OVERTONE_DAMPING;
    setCoefs();
}

void ModalBank::setFreq(float f0)
{
    m_f0 = f0;
    m_forceInGain = sqrtf(f0);

    setCoefs();
}

void ModalBank::setPosition(float position)
{
    for (int i = 0; i < NUM_MODES; i++)
    {
        m_forcePosGain[i] = readModeShapeLerp(i, position);
    }
}

//#ifdef __SSE__
//void ModalBank::processBlock(float* inBuffer, float* outBuffer, 
//                             unsigned int length)
//{
//    const unsigned int FLOATS_IN_SSE_REGISTER = 4u;
//    const unsigned int NUM_MODES = 8u;
//    const unsigned int vectorizableModes = (NUM_MODES / 
//            FLOATS_IN_SSE_REGISTER) * FLOATS_IN_SSE_REGISTER;
//
//    for (unsigned int n{ 0 }; n < length; n++)
//    {
//        __m128 sumReg = _mm_setzero_ps();
//
//        unsigned int i{};
//        for (; i < vectorizableModes; i += FLOATS_IN_SSE_REGISTER)
//        {
//            __m128 forcePosGainReg = _mm_loadu_ps(m_forcePosGain + i);
//
//            __m128 b0Reg = _mm_loadu_ps(m_b0 + i);
//            __m128 b2Reg = _mm_loadu_ps(m_b2 + i);
//            __m128 a1Reg = _mm_loadu_ps(m_a1 + i);
//            __m128 a2Reg = _mm_loadu_ps(m_a2 + i);
//
//            __m128 x1Reg = _mm_loadu_ps(m_x1 + i);
//            __m128 x2Reg = _mm_loadu_ps(m_x2 + i);
//            __m128 y1Reg = _mm_loadu_ps(m_y1 + i);
//            __m128 y2Reg = _mm_loadu_ps(m_y2 + i);
//    
//            __m128 xReg = _mm_set1_ps(inBuffer[n]);
//            xReg = _mm_mul_ps(xReg, forcePosGainReg);
//
//            // y[n] = b0 * x[n]
//            __m128 yReg = _mm_mul_ps(b0Reg, xReg);
//
//            // y[n] += b2 * x[n - 2]
//            __m128 tempReg = _mm_mul_ps(b2Reg, x2Reg);
//            yReg = _mm_add_ps(yReg, tempReg);
//
//            // y[n] += a1 * y[n - 1]
//            tempReg = _mm_mul_ps(a1Reg, y1Reg);
//            yReg = _mm_sub_ps(yReg, tempReg);
//
//            // y[n] += a2 * y[n - 2[
//            tempReg = _mm_mul_ps(a2Reg, y2Reg);
//            yReg = _mm_sub_ps(yReg, tempReg);
//
//
//            _mm_storeu_ps(m_x2 + i, x1Reg);
//            _mm_storeu_ps(m_x1 + i, xReg);
//            _mm_storeu_ps(m_y2 + i, y1Reg);
//            _mm_storeu_ps(m_y1 + i, yReg);
//
//            sumReg = _mm_add_ps(sumReg, yReg);
//        }
//        // horizontal sum
//        sumReg = _mm_hadd_ps(sumReg, sumReg);
//        sumReg = _mm_hadd_ps(sumReg, sumReg);
//        float out = _mm_cvtss_f32(sumReg);
//
//        for (; i < NUM_MODES; i++)
//        {
//            float x = inBuffer[n] * m_forcePosGain[i];
//
//            float y = m_b0[i] * x + m_b2[i] * m_x2[i] - 
//                        m_a1[i] * m_y1[i] - m_a2[i] * m_y2[i];
//
//            m_x2[i] = m_x1[i];
//            m_x1[i] = x;
//            m_y2[i] = m_y1[i];
//            m_y1[i] = y;
//
//            out += y;
//        }
//
//        outBuffer[n] = out; 
//    }
//}
//#else
void ModalBank::processBlock(float* inBuffer, float* outBuffer1, 
        float* outBuffer2, unsigned int length)
{
    for (unsigned int n = 0; n < length; n++)
    {
        float in = -m_forceInGain * inBuffer[n];
        float outRadiate = 0.f;
        float outTube = 0.f;

        for (int i{ 0 }; i < NUM_MODES; i++)
        {
            float x = in * m_forcePosGain[i];

            float y = m_b0[i] * x + m_b2[i] * m_x2[i] - 
                        m_a1[i] * m_y1[i] - m_a2[i] * m_y2[i];

            m_x2[i] = m_x1[i];
            m_x1[i] = x;
            m_y2[i] = m_y1[i];
            m_y1[i] = y;

            outRadiate += y * m_outPosGain[i];
            outTube += y * m_tubePosGain[i];
        }

        outBuffer1[n] = outRadiate;
        outBuffer2[n] = outTube;
    }
}
//#endif

void ModalBank::setCoefs()
{
    for (int i{ 0 }; i < NUM_MODES; i++)
    {
        float freq = m_ratios[i] * m_f0;
        float alpha = m_globalDamping + m_overtoneDamping * 
                powf(m_ratios[i] - 1.f, 2.f) + BAR_NOTE_DAMPING * 
                                                        powf(m_f0, 2.f);

        float radius = expf(-alpha / m_sampleRate);
        float omega = 2.f * PI * freq;
        float k = omega * omega + alpha * alpha;
        m_a1[i] = -2.f * radius * cosf(omega / m_sampleRate);
        m_a2[i] = radius * radius;
        m_b0[i] = (1.f + m_a1[i] + m_a2[i]) / k;

        if (freq > 20000.f) 
            m_b0[i] = 0.f;
    }
}

void ModalBank::clear()
{
    std::fill(m_x1, m_x1 + NUM_MODES, 0.f);
    std::fill(m_x2, m_x2 + NUM_MODES, 0.f);
    std::fill(m_y1, m_y1 + NUM_MODES, 0.f);
    std::fill(m_y2, m_y2 + NUM_MODES, 0.f);
}
};
