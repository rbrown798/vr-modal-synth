#pragma once

#include <memory>
#include <vector>
#include <array>
#include "ImpactForce.h"
#include "BarResonator.h"
#include "MalletResonator.h"
#include "TubeResonator.h"
#include "Spatializer.h"
#include "Vector3.h"
#include "DipoleRadiation.h"
#include "MonopoleRadiation.h"


namespace ModalSynth
{
const float BAR_WIDTH = 0.025f;  
const float BAR_PAD = 0.006f;          
const int CENTER_NOTE = 53;

const float MALLET_GAIN{ 0.00015f };
const float BAR_GAIN{ 1e-8f };
const float TUBE_GAIN{ 4e-5f };

const float NOTE_XPOS_TABLE[] = { 0.f, 0.5f, 1.f, 1.5f, 2.f, 3.f, 3.5f, 4.f, 
                                    4.5f, 5.f, 5.5f, 6.f };

const float NOTE_XPOS_OFFSET = 7.f * static_cast<float>(CENTER_NOTE / 12) +
                                    NOTE_XPOS_TABLE[CENTER_NOTE % 12];

const float MAX_DAMPING = 0.5f;
const float PEDAL_DAMP_COEF = 2.5f;

constexpr size_t NUM_TEMP_BUFFERS{ 3 };


class Voice
{
private:
    ImpactForce             m_impactForce;
    BarResonator            m_barResonator;
    MalletResonator         m_malletResonator;
    TubeResonator           m_tubeResonator;
    bool                    m_isTubeOn{true};
    DipoleRadiation         m_malletRadiation;
    DipoleRadiation         m_barRadiation;
    MonopoleRadiation       m_tubeRadiation;
    Spatializer             m_spatializer;
    std::array<std::vector<float>, NUM_TEMP_BUFFERS>& m_tempBuffers;
    float                   m_barDamping{};
    float                   m_pedalDamping{};
    bool                    m_isActive{};
    int                     m_timestamp{};
    int                     m_note{};

    float getBarTotalDamping() const;
    void clear();

public:
    Voice(std::vector<float>& lfoBuffer, 
            std::array<std::vector<float>, NUM_TEMP_BUFFERS>& tempBuffers);

    void initialize(float sampleRate);
    void update();
    void noteOn(int note, float velocity, float position);
    void noteOff();
    void retrigger(float velocity, float position);
    void setBarTimbre(float barTimbre);
    void setBarDamping(float barDamping);
    void setBarMetallic(float value);
    void setMalletHeadRadius(float malletHeadRadius);
    void setMalletHeadMass(float malletHeadMass);
    void setContactModulus(float contactModulus);
    void renderBlock(float* outBuffer, unsigned int length, int outChannels);
    void setTubeOn(bool isTubeOn);
    void setPedalValue(float pedalValue);
    void setSourcePosition(const Vector3& sourcePosition, bool immediate=false);
    void setLeftEarPosition(const Vector3& leftEarPosition, 
            bool immediate=false);
    void setLeftEarDirection(const Vector3& leftEarDirection, 
            bool immediate=false);
    void setRightEarPosition(const Vector3& rightEarPosition, 
            bool immediate=false);
    void setRightEarDirection(const Vector3& rightEarDirection, 
            bool immediate=false);
    bool isVoiceActive() const;
    int getTimestamp() const;
    void incrementTimestamp();
    int getNote() const;
};
};
