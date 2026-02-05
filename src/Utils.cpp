#include "Utils.h"


namespace ModalSynth
{
// Source: https://graphics.stanford.edu/%7Eseander/bithacks.html#RoundUpPowerOf2
int nextPowerOf2(int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}
};
