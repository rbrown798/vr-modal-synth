#pragma once

namespace ModalSynth
{
const int MODE_SHAPE_LUT_SIZE{ 64 };
const float MAX_R{ 0.99999f };
const float MAX_R_SQUARED{ powf(MAX_R, 2.0f) };

const int NUM_MODES{ 8 };

class ModalBank
{
private:
    float m_sampleRate{};
    float m_freq[NUM_MODES]{};
    float m_radius[NUM_MODES]{};
    float m_gain[NUM_MODES]{};
    float m_positionGain[NUM_MODES]{};
    float m_outPosGain[NUM_MODES]{};
    float m_overtoneRatios[NUM_MODES-1]{};

    float m_f1{};

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
    void setFreq(float f1);
    void setPosition(float position);
    void processBlock(float* inBuffer, float* outBuffer, unsigned int length);
    void clear();

    static void initModeShapes();
};
};
