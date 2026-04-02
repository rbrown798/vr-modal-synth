#include "BarProperties.h"
#include "Utils.h"

namespace ModalSynth
{
BarProperties::setFromParameters(const Parameters& params)
{
    setModeRatios(params->barTimbre);

    globalDamping = lerp(MIN_BAR_GLOBAL_DAMPING, MAX_BAR_GLOBAL_DAMPING,
            params->barDamping);

    overtoneDamping = lerp(MIN_BAR_OVERTONE_DAMPING, MAX_BAR_OVERTONE_DAMPING,
            (1.f - params->barMetallic));
}

BarProperties::setModeRatios(float timbre)
{
    assert((0 <= timbre) && (timbre <= 1));

    for (int i{ 0 }; i < NUM_MODES; i++)
    {
        if (timbre < 0.5f)
        {
            m_ratios[i] = GLOCKEN_RATIOS[i] + 2.f * timbre * 
                                    (XYLO_RATIOS[i] - GLOCKEN_RATIOS[i]);
        }
        else
        {
            m_ratios[i] = XYLO_RATIOS[i] + 2.f * (timbre - 0.5f) * 
                                    (VIBES_RATIOS[i] - XYLO_RATIOS[i]);
        }
    }
}
};

