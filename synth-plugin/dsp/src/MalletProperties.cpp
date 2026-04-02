#include "MalletProperties.h"
#include "Utils.h"

namespace ModalSynth
{
void MalletProperties::setFromParameters(const Parameters& params)
{
    radius_m = lerp(MIN_MALLET_RADIUS, MAX_MALLET_RADIUS, params.malletSize);
    volume_m3 = (4.f / 3.f) * PI * radius_m * radius_m * radius_m;
    mass_kg = MALLET_DENSITY * volume_m3;

    contactModulus = lerp(MIN_CONTACT_MODULUS, MAX_CONTACT_MODULUS, 
            params.malletStiffness);
}
};
