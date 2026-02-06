#include "Voice.h"
#include <immintrin.h>
#include <cstdlib>
#include <iostream>


namespace ModalSynth
{
#ifdef __AVX__
void gain_avx(float* in, float* out, unsigned int length, float g)
{
    constexpr auto FLOATS_IN_AVX_REGISTER = 8u;
    const auto vectorizableSamples = (length / FLOATS_IN_AVX_REGISTER) *
        FLOATS_IN_AVX_REGISTER;

    __m256 gRegister = _mm256_set1_ps(g);
    
    unsigned int i{};
    for (; i < vectorizableSamples; i += FLOATS_IN_AVX_REGISTER)
    {
        __m256 inRegister = _mm256_loadu_ps(in + i);
        __m256 outRegister = _mm256_mul_ps(inRegister, gRegister);
        _mm256_storeu_ps(out + i, outRegister);
    }

    for (; i < length; i++)
    {
        out[i] = g * in[i];
    }
}
#endif

#ifdef __SSE__
void gain_sse(float* in, float* out, unsigned int length, float g)
{
    constexpr auto FLOATS_IN_SSE_REGISTER = 4u;
    const auto vectorizableSamples = (length / FLOATS_IN_SSE_REGISTER) *
        FLOATS_IN_SSE_REGISTER;

    __m128 gRegister = _mm_set1_ps(g);
    
    unsigned int i{};
    for (; i < vectorizableSamples; i += FLOATS_IN_SSE_REGISTER)
    {
        __m128 inRegister = _mm_loadu_ps(in + i);
        __m128 outRegister = _mm_mul_ps(inRegister, gRegister);
        _mm_storeu_ps(out + i, outRegister);
    }

    for (; i < length; i++)
    {
        out[i] = g * in[i];
    }
}
#endif

void gain_scalar(float* in, float* out, unsigned int length, float g)
{
    for (unsigned int i{ 0 }; i < length; i++)
        out[i] = g * in[i];
}

void gain(float* in, float* out, unsigned int length, float g)
{
#ifdef __AVX__
    gain_avx(in, out, length, g);
#elif defined(__SSE__)
    gain_sse(in, out, length, g);
#else
    gain_scalar(in, out, length, g);
#endif
}

#ifdef __AVX__
void mix_avx(float* out, float* in1, float* in2, unsigned int length)
{
    constexpr auto FLOATS_IN_AVX_REGISTER = 8u;
    const auto vectorizableSamples = (length / FLOATS_IN_AVX_REGISTER) *
        FLOATS_IN_AVX_REGISTER;

    unsigned int i{};
    for (; i < vectorizableSamples; i += FLOATS_IN_AVX_REGISTER)
    {
        __m256 in1Register = _mm256_loadu_ps(in1 + i);
        __m256 in2Register = _mm256_loadu_ps(in2 + i);
        __m256 outRegister = _mm256_add_ps(in1Register, in2Register);
        _mm256_storeu_ps(out + i, outRegister);
    }

    for (; i < length; i++)
    {
        out[i] = in1[i] * in2[i];
    }
}
#endif

#ifdef __SSE__
void mix_sse(float* out, float* in1, float* in2, unsigned int length)
{
    constexpr auto FLOATS_IN_AVX_REGISTER = 4u;
    const auto vectorizableSamples = (length / FLOATS_IN_AVX_REGISTER) *
        FLOATS_IN_AVX_REGISTER;

    unsigned int i{};
    for (; i < vectorizableSamples; i += FLOATS_IN_AVX_REGISTER)
    {
        __m128 in1Register = _mm_loadu_ps(in1 + i);
        __m128 in2Register = _mm_loadu_ps(in2 + i);
        __m128 outRegister = _mm_add_ps(in1Register, in2Register);
        _mm_storeu_ps(out + i, outRegister);
    }

    for (; i < length; i++)
    {
        out[i] = in1[i] * in2[i];
    }
}
#endif

void mix_scalar(float* out, float* in1, float* in2, unsigned int length)
{
    for (unsigned int i{ 0 }; i < length; i++)
        out[i] = in1[i] + in2[i];
}


void mix(float* out, float* in1, float* in2, unsigned int length)
{
#ifdef __AVX__
    mix_avx(out, in1, in2, length);
#elif defined(__SSE__)
    mix_sse(out, in1, in2, length);
#else
    mix_scalar(out, in1, in2, length);
#endif
}

Voice::Voice(std::vector<float>& lfoBuffer, std::vector<float>& tempBuffer1, 
        std::vector<float>& tempBuffer2, std::vector<float>& tempBuffer3) : 
    m_tube{lfoBuffer}, m_tempBuffer1{tempBuffer1}, m_tempBuffer2{tempBuffer2}, 
    m_tempBuffer3{tempBuffer3}
{
}

void Voice::initialize(float sampleRate)
{
    m_impactForce.initialize(sampleRate);
    m_modalBank.initialize(sampleRate);
    m_malletSpring.initialize(sampleRate);
    m_tube.initialize(sampleRate);
}

void Voice::noteOn(int note, float velocity, float position)
{
    float freq = 440.f * powf(2.f, static_cast<float>(note - 69) / 12.f);

    m_modalBank.clear();
    m_modalBank.setFreq(freq);
    m_modalBank.setPosition(position);
    m_modalBank.setDamping(m_barDamping);

    m_tube.clear();
    m_tube.setFreq(freq);

    m_impactForce.play(velocity);
}

void Voice::noteOff()
{
    // temporary solution
    //m_modalBank.clear();
    m_modalBank.setDamping(std::max(0.4f, m_barDamping));
}

void Voice::renderBlock(float* outBuffer, unsigned int length, int outChannels)
{ 
    std::fill(m_tempBuffer1.begin(), m_tempBuffer1.end(), 0.0f);
    std::fill(m_tempBuffer2.begin(), m_tempBuffer2.end(), 0.0f);
    std::fill(m_tempBuffer3.begin(), m_tempBuffer3.end(), 0.0f);

    float* temp1 = m_tempBuffer1.data();
    float* temp2 = m_tempBuffer2.data();
    float* temp3 = m_tempBuffer3.data();

    m_impactForce.renderBlock(temp1, length);

    m_modalBank.processBlock(temp1, temp2, length);
 
    if (m_isTubeOn)
    {
        m_tube.processBlock(temp2, temp3, length);
    }
    gain(temp3, temp3, length, 0.07f);  // Resonator gain
    gain(temp2, temp2, length, 0.8f);   // Bar gain
    mix(temp2, temp3, temp2, length);
 
    m_malletSpring.processBlock(temp1, temp1, length);
    //gain(temp1, temp1, length, 0.005f); // Mallet gain
    gain(temp1, temp1, length, 5000000.f); // Mallet gain (changed b0)
    mix(temp2, temp1, temp2, length);

    m_highPass.processBlock(temp2, temp2, length);
    gain(temp2, temp2, length, 0.0005);

    // TODO: Need to handle spatialization

    for (unsigned int n{ 0 }; n < length; n++)
    {
        for (int ch{ 0 }; ch < outChannels; ch++)
        {
            outBuffer[n * outChannels + ch] += temp2[n];
        }
    }
}

void Voice::setBarTimbre(float barTimbre)
{
    m_modalBank.setTimbre(barTimbre);
}

void Voice::setBarDamping(float barDamping)
{
    // TODO: Fix this, probably will cause problems for note off
    m_barDamping = barDamping;
    m_modalBank.setDamping(barDamping);
}

void Voice::setMalletHeadRadius(float malletHeadRadius)
{
    m_impactForce.setMalletHeadRadius(malletHeadRadius);
}

void Voice::setMalletHeadMass(float malletHeadMass)
{
    m_impactForce.setMalletHeadMass(malletHeadMass);
    m_malletSpring.setMass(malletHeadMass);
}

void Voice::setContactModulus(float contactModulus)
{
    m_impactForce.setContactModulus(contactModulus);
}

void Voice::setTubeOn(bool isTubeOn)
{
    m_isTubeOn = isTubeOn;
}
};
