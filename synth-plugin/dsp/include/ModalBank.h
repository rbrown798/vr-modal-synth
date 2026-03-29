#pragma once

namespace ModalSynth
{
const int MODE_SHAPE_LUT_SIZE{ 64 };
const int NUM_MODES{ 8 };

const float MIN_BAR_GLOBAL_DAMPING{ 0.5f };
const float MAX_BAR_GLOBAL_DAMPING{ 10.f };
const float MIN_BAR_OVERTONE_DAMPING{ 0.006f };
const float MAX_BAR_OVERTONE_DAMPING{ 0.2f };
const float BAR_NOTE_DAMPING{ 1.5e-6f };

const float GLOCKEN_RATIOS[] = { 
    1.f, 2.576f, 4.505f, 8.933f, 13.34f, 18.63f, 24.81f, 31.86f 
};

const float XYLO_RATIOS[] = { 
    // first 7 ratios from euphonics.org/3-3-marimbas-and-xylophones
    1.f, 3.f, 6.16f, 10.29f, 14.01f, 19.66f, 24.02f, 32.f 
};

const float VIBES_RATIOS[] = { 
    // first 7 ratios from N.H. Fletcher "The Physics of Musical Instruments"
    1.f, 4.f, 9.7f, 18.3f, 23.5f, 35.4f, 46.3f, 64.f 
};

class ModalBank
{
private:
    float m_sampleRate{};
    float m_forcePosGain[NUM_MODES]{};
    float m_outPosGain[NUM_MODES]{};
    float m_tubePosGain[NUM_MODES]{};
    float m_ratios[NUM_MODES]{};

    float m_forceInGain{};

    float m_globalDamping{};
    float m_overtoneDamping{ 0.026f };

    float m_f0{};

    float m_b0[NUM_MODES]{};
    float m_b2[NUM_MODES]{};
    float m_a1[NUM_MODES]{};
    float m_a2[NUM_MODES]{};

    float m_x1[NUM_MODES]{};
    float m_x2[NUM_MODES]{};
    float m_y1[NUM_MODES]{};
    float m_y2[NUM_MODES]{};

    static float s_modeShapes[NUM_MODES][MODE_SHAPE_LUT_SIZE];

    static float getModeShape(float knL, float x);
    static float readModeShapeLerp(int modeNumber, float position);

    void setCoefs();
    float cubicPoly(float a, float b, float c, float d, float x);
    float lerp(float a, float b, float t);

public:
    ModalBank() = default;
    void initialize(float sampleRate);
    void setTimbre(float timbre);
    void setDamping(float damping);
    void setMetallic(float value);
    void setFreq(float f0);
    void setPosition(float position);
    void processBlock(float* inBuffer, float* outBuffer1, float* outBuffer2,
            unsigned int length);
    void clear();

    static void initModeShapes();
};
};
