#pragma once

namespace ModalSynth
{
const float PI{ 3.14159265359f }; 

// Source: https://graphics.stanford.edu/%7Eseander/bithacks.html#RoundUpPowerOf2
int nextPowerOf2(int v);

float lerp(float a, float b, float t);
float cubicPoly(float a, float b, float c, float d, float x);

void gain_avx(float* in, float* out, unsigned int length, float g);
void gain_sse(float* in, float* out, unsigned int length, float g);
void gain_scalar(float* in, float* out, unsigned int length, float g);
void gain(float* in, float* out, unsigned int length, float g);

void mix_avx(float* out, float* in1, float* in2, unsigned int length);
void mix_sse(float* out, float* in1, float* in2, unsigned int length);
void mix_scalar(float* out, float* in1, float* in2, unsigned int length);
void mix(float* out, float* in1, float* in2, unsigned int length);
};

