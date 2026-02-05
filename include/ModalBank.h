#pragma once

namespace ModalSynth
{
const int MODE_SHAPE_LUT_SIZE{ 64 };
const float MAX_R{ 0.99999f };
const float MAX_R_SQUARED{ powf(MAX_R, 2.0f) };

class ModalBank
{
private:
    float m_sampleRate{};
    float m_freq[8]{};
    float m_radius[8]{};
    float m_gain[8]{};
    float m_positionGain[8]{};
    float m_overtoneRatios[7]{};

    float m_f1{};

    float m_b0[8]{};
    float m_b2[8]{};
    float m_a1[8]{};
    float m_a2[8]{};

    float m_x1[8]{};
    float m_x2[8]{};
    float m_y1[8]{};
    float m_y2[8]{};

    static float s_modeShapes[8][MODE_SHAPE_LUT_SIZE];

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
