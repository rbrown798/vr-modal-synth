#include "HighPass.h"
#include <immintrin.h>


namespace ModalSynth
{
//#ifdef __SSE__
//void HighPass::processBlock(float *in, float *out, unsigned int length)
//{
//    constexpr auto FLOATS_IN_SSE_REGISTER = 4u;
//    const audo vectorizableSamples = (length / FLOATS_IN_SSE_REGISTER) *
//        FLOATS_IN_SSE_REGISTER;
//
//    __m128 b0Reg = _mm_set1_ps(0.5f);
//    __m128 b1Reg = _mm_set1_ps(-0.5f);
//
//    unsigned int i{};
//    for (; i < vectorizable samples; i += FLOATS_IN_SSE_REGISTER)
//    {
//        __m128 inReg = _mm_loadu_ps(in + i);
//        __m128 outReg = _mm_mul_ps(b0Reg, inReg);
//        __m128 tempReg = _mm_mul_ps(b1Reg, x1Reg);
//
//        out[i] = 0.5f * in[i] - 0.5f * m_x1;
//        m_x1 = in[i];
//    }
//}
//#else
void HighPass::processBlock(float *in, float *out, unsigned int length)
{
    for (unsigned int i{ 0 }; i < length; i++)
    {
        out[i] = 0.5f * in[i] - 0.5f * m_x1;
        m_x1 = in[i];
    }
}
//#endif
};
