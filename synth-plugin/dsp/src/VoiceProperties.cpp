#include "VoiceProperties.h"

namespace ModalSynth
{
VoiceProperties::setFromParameters(const Parameters& params)
{
    malletProperties.setFromParameters(params);
    barProperties.setFromParameters(params);
    tubeOn = params->tubeOn;
}
};

