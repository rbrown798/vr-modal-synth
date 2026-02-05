#include <cmath>
#include <cassert>
#include <immintrin.h>
#include "ModalBank.h"
#include "Utils.h"


namespace ModalSynth
{
float ModalBank::s_modeShapes[8][MODE_SHAPE_LUT_SIZE]{};

float ModalBank::getModeShape(float knL, float x)
{
    return sinhf(knL * x) + sin(knL * x) + (sin(knL) - sinh(knL))
        / (cosh(knL) - cos(knL)) * (cosh(knL * x) + cos(knL * x));
}

void ModalBank::initModeShapes()
{
    float knLValues[8] = { 4.73004f, 7.8532f, 10.99561f, 14.13717f, 17.27876f,
                           20.42035f, 23.56194f, 26.70354f };

    for (int i = 0; i < 8; i++)
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
}

void ModalBank::setTimbre(float timbre)
{
    assert((0 <= timbre) && (timbre <= 1));

    m_overtoneRatios[0] = cubicPoly(0.693042f, 0.472437, 0.078521, 2.756, 
                            timbre);

    m_overtoneRatios[1] = cubicPoly(2.12847f, 1.45529f, 0.252235f, 5.404f, 
                            timbre);

    m_overtoneRatios[2] = cubicPoly(4.28341f, 2.82088f, 0.232706f, 8.933f, 
                            timbre);

    m_overtoneRatios[3] = lerp(13.34f, 24.22f, timbre);
    m_overtoneRatios[4] = lerp(18.63f, 33.54f, timbre);
    m_overtoneRatios[5] = lerp(24.81f, 42.97f, timbre);
    m_overtoneRatios[6] = lerp(31.86f, 52.4f, timbre); // 52.4 is a prediction

    // find a better place for this... (maybe at the top of setCoefs)
    for (int i = 0; i < 7; i++)
        m_freq[i + 1] = m_overtoneRatios[i] * m_f1;

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
    for (int i = 0; i < 8; i++)
    {
        m_radius[i] = MAX_R * expf(-0.0005f * damping *
                                            static_cast<float>(i + 1));
    }

    setCoefs();
}

//void ModalBank::setFreq(float f1)
//{
//    for (int i = 0; i < 8; i++)
//    {
//        m_freq[i] = 0.441f * powf((i + 1.5f), 2.0f) * f1;
//    }
//
//    setCoefs();
//}

void ModalBank::setFreq(float f1)
{
    m_f1 = f1;

    m_freq[0] = f1;

    for (int i = 0; i < 7; i++)
        m_freq[i + 1] = m_overtoneRatios[i] * f1; 
    
    setCoefs();
}

void ModalBank::setPosition(float position)
{
    for (int i = 0; i < 8; i++)
    {
        m_positionGain[i] = readModeShapeLerp(i, position);
    }
}

#ifdef __SSE__
void ModalBank::processBlock(float* inBuffer, float* outBuffer, 
                             unsigned int length)
{
    const unsigned int FLOATS_IN_SSE_REGISTER = 4u;
    const unsigned int NUM_MODES = 8u;
    const unsigned int vectorizableModes = (NUM_MODES / 
            FLOATS_IN_SSE_REGISTER) * FLOATS_IN_SSE_REGISTER;

    for (unsigned int n{ 0 }; n < length; n++)
    {
        __m128 sumReg = _mm_setzero_ps();

        unsigned int i{};
        for (; i < vectorizableModes; i += FLOATS_IN_SSE_REGISTER)
        {
            __m128 positionGainReg = _mm_loadu_ps(m_positionGain + i);

            __m128 b0Reg = _mm_loadu_ps(m_b0 + i);
            __m128 b2Reg = _mm_loadu_ps(m_b2 + i);
            __m128 a1Reg = _mm_loadu_ps(m_a1 + i);
            __m128 a2Reg = _mm_loadu_ps(m_a2 + i);

            __m128 x1Reg = _mm_loadu_ps(m_x1 + i);
            __m128 x2Reg = _mm_loadu_ps(m_x2 + i);
            __m128 y1Reg = _mm_loadu_ps(m_y1 + i);
            __m128 y2Reg = _mm_loadu_ps(m_y2 + i);
    
            __m128 xReg = _mm_set1_ps(inBuffer[n]);
            xReg = _mm_mul_ps(xReg, positionGainReg);

            // y[n] = b0 * x[n]
            __m128 yReg = _mm_mul_ps(b0Reg, xReg);

            // y[n] += b2 * x[n - 2]
            __m128 tempReg = _mm_mul_ps(b2Reg, x2Reg);
            yReg = _mm_add_ps(yReg, tempReg);

            // y[n] += a1 * y[n - 1]
            tempReg = _mm_mul_ps(a1Reg, y1Reg);
            yReg = _mm_sub_ps(yReg, tempReg);

            // y[n] += a2 * y[n - 2[
            tempReg = _mm_mul_ps(a2Reg, y2Reg);
            yReg = _mm_sub_ps(yReg, tempReg);


            _mm_storeu_ps(m_x2 + i, x1Reg);
            _mm_storeu_ps(m_x1 + i, xReg);
            _mm_storeu_ps(m_y2 + i, y1Reg);
            _mm_storeu_ps(m_y1 + i, yReg);

            sumReg = _mm_add_ps(sumReg, yReg);
        }
        // horizontal sum
        sumReg = _mm_hadd_ps(sumReg, sumReg);
        sumReg = _mm_hadd_ps(sumReg, sumReg);
        float out = _mm_cvtss_f32(sumReg);

        for (; i < NUM_MODES; i++)
        {
            float x = inBuffer[n] * m_positionGain[i];

            float y = m_b0[i] * x + m_b2[i] * m_x2[i] - 
                        m_a1[i] * m_y1[i] - m_a2[i] * m_y2[i];

            m_x2[i] = m_x1[i];
            m_x1[i] = x;
            m_y2[i] = m_y1[i];
            m_y1[i] = y;

            out += y;
        }

        outBuffer[n] = out; 
    }
}
#else
void ModalBank::processBlock(float* inBuffer, float* outBuffer, 
                             unsigned int length)
{
    float out;
    for (unsigned int n = 0; n < length; n++)
    {
        out = 0.0f;
        for (int i{ 0 }; i < 8; i++)
        {
            float x = inBuffer[n] * m_positionGain[i];

            float y = m_b0[i] * x + m_b2[i] * m_x2[i] - 
                        m_a1[i] * m_y1[i] - m_a2[i] * m_y2[i];

            m_x2[i] = m_x1[i];
            m_x1[i] = x;
            m_y2[i] = m_y1[i];
            m_y1[i] = y;

            out += y;
        }

        outBuffer[n] = out; //+= out;
    }
}
#endif

void ModalBank::setCoefs()
{
    for (int i = 0; i < 8; i++)
    {
        if (m_freq[i] > 20000.0f)
        {
            m_gain[i] = 0.0f;
        }

        else
        {
            float upperHarmonics = 2.0f;
            // Still figuring this out
            m_gain[i] = 400.0f * expf(-0.25f / upperHarmonics * i);
        }
    }

    //float MAX_R_SQUARED = powf(0.99999f, 2.0f);

    float T = 1.0f / m_sampleRate;
    float theta;
    for (int i = 0; i < 8; i++)
    {
        theta = 2.0f * PI * m_freq[i] * T;
        m_a1[i] = -2.0f * m_radius[i] * cosf(theta);
        m_a2[i] = m_radius[i] * m_radius[i];
        m_b0[i] = m_gain[i] * 0.5f * (1.f - MAX_R_SQUARED);
        m_b2[i] = -m_b0[i];
    }
}

void ModalBank::clear()
{
    std::fill(m_x1, m_x1 + 8, 0.f);
    std::fill(m_x2, m_x2 + 8, 0.f);
    std::fill(m_y1, m_y1 + 8, 0.f);
    std::fill(m_y2, m_y2 + 8, 0.f);
}
};
