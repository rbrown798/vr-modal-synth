#pragma once

#include "MalletProperties.h"
#include "BarProperties.h"
#include "Parameters.h"

namespace ModalSynth
{
struct VoiceProperties
{
    MalletProperties    malletProperties;
    BarProperties       barProperties;
    bool                tubeOn;

    void setFromParameters(const Parameters& params);
};
};
