#include "ElementCommon.h"

#define BIORNG_OVERUSED 120
#define BIORNG_SHIFT(val) val & 0xF

BioRNG::BioRNG() {
    this->PureRNG = RNG();
    RegenerateRNG();
}

// How about we spice things up?
void BioRNG::RegenerateRNG() {
    this->rngVal = PureRNG.gen();
    this->useCount = 0;
}

// Replaces rx & ry's between(). Melts brains.
// Please note that shift is used to shift bitwise.
int BioRNG::RBetween(int range) {
    int* val = (int*) &this->rngVal;
    int ret = *val % (range * 2);
    *val = *val >> BIORNG_SHIFT(*val);
    this->useCount++;
    OverusedCheck();
    return ret;
}

// RBetween's cousin
int BioRNG::Between(int lower, int upper) {
    int* val = (int*) this->rngVal;
    int num = *val;
    *val = *val >> BIORNG_SHIFT(*val);
    this->useCount++;
    OverusedCheck();
    return static_cast<int>(num % ((unsigned int)(upper) - (unsigned int)(lower) + 1U)) + lower;
}

// Let's go gambling!
bool BioRNG::Chance(unsigned int number) {
    int* val = (int*) &this->rngVal;
    bool ret = *val % number < 1;
    *val = *val >> BIORNG_SHIFT(*val);
    this->useCount++;
    OverusedCheck();
    return ret;
}

// Checks and regenerates the RNG if overused.
void BioRNG::OverusedCheck() {
    if (this->useCount > BIORNG_OVERUSED)
        RegenerateRNG();
}
