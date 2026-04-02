#pragma once

#include "Parameters.h"

namespace ModalSynth
{
const float MALLET_DENSITY{ 8000.f };
const float MIN_CONTACT_MODULUS{ 1.32e+07f };  // soft rubber + aluminum
const float MAX_CONTACT_MODULUS{ 2.51e+09f };  // ABS + aluminum

struct MalletProperties
{
    float radius_m;
    float mass_kg;
    float volume_m3;
    float contactModulus;

    void setFromParameters(const Parameters& params);
};
};
