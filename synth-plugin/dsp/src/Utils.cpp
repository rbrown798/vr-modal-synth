#include <immintrin.h>
#include <cassert>
#include "Utils.h"


namespace ModalSynth
{
// Source: https://graphics.stanford.edu/%7Eseander/bithacks.html#RoundUpPowerOf2
int nextPowerOf2(int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

float lerp(float a, float b, float t)
{
    assert((0 <= t) && (t <= 1));
    return a + (b - a) * t;
}

float cubicPoly(float a, float b, float c, float d, float x)
{
    return ((a * x + b) * x + c) * x + d;
}

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
        out[i] = in1[i] + in2[i];
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
        out[i] = in1[i] + in2[i];
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
};
