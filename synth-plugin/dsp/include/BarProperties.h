#pragma once

namespace ModalSynth
{
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

struct BarProperties
{
    float modeRatios[NUM_MODES];
    float globalDamping;
    float overtoneDamping;
    float noteDamping{BAR_NOTE_DAMPING};

    void setFromParameters(const Parameters& params);
};
};
