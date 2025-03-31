#include "ElementCommon.h"

#define BIORNG_OVERUSED 8192
#define BIORNG_MASK(val) val & 0b1111

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
int BioRNG::RBetween(unsigned int range) {
    int* val = (int*) &this->rngVal;
    int ret = static_cast<int>(*val % (range * 2 + 1U)) - range;
    *val = *val >> BIORNG_MASK(*val);
    this->useCount++;
    OverusedCheck();
    return ret;
}

// RBetween's cousin
int BioRNG::Between(int lower, int upper) {
    int* val = (int*) &this->rngVal;
    int num = *val;
    *val = *val >> BIORNG_MASK(*val);
    this->useCount++;
    OverusedCheck();
    return static_cast<int>(num % ((unsigned int)(upper) - (unsigned int)(lower) + 1U)) + lower;
}

// Let's go gambling!
bool BioRNG::Chance(unsigned int number) {
    int* val = (int*) &this->rngVal;
    bool ret = *val % number < 1;
    *val = *val >> BIORNG_MASK(*val);
    this->useCount++;
    OverusedCheck();
    return ret;
}

// Gamblecore 2
bool BioRNG::Chance(int numerator, unsigned int denominator) {
    if (numerator < 0)
        return false;
    int* val = (int*) &this->rngVal;
    bool ret = *val % denominator < static_cast<unsigned int>(numerator);
    this->useCount++;
    OverusedCheck();
    return ret;
}

// Checks and regenerates the RNG if overused.
void BioRNG::OverusedCheck() {
    if (this->useCount > BIORNG_OVERUSED)
        RegenerateRNG();
}
