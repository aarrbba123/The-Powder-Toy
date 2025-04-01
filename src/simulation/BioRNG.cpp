#include "ElementCommon.h"

#define BIORNG_OVERUSED 8192
#define BIORNG_MASK(val) val & 0b11111

BioRNG::BioRNG() {
    this->PureRNG = RNG();
    RegenerateRNG();
}

int BioRNG::Next() {
    this->rngVal = this->rngVal >> BIORNG_MASK(this->rngVal);
    this->useCount++;
    OverusedCheck();
    return this->rngVal;
}

// How about we spice things up?
void inline BioRNG::RegenerateRNG() {
    this->rngVal = PureRNG.gen();
    this->useCount = 0;
}

// Replaces rx & ry's between(). Melts brains.
int BioRNG::RBetween(unsigned int range) {
    return static_cast<int>(Next() % (range * 2 + 1U)) - range;
}

// RBetween's cousin
int BioRNG::Between(int lower, int upper) {
    return static_cast<int>(Next() % ((unsigned int)(upper) - (unsigned int)(lower) + 1U)) + lower;
}

// Let's go gambling!
bool BioRNG::Chance(unsigned int number) {
    return Next() % number < static_cast<unsigned int>(1);
}

// Gamblecore 2
bool BioRNG::Chance(int numerator, unsigned int denominator) {
    if (numerator < 0)
        return false;
    return Next() % denominator < static_cast<unsigned int>(numerator);
}

// Checks and regenerates the RNG if overused.
void BioRNG::OverusedCheck() {
    if (this->useCount > BIORNG_OVERUSED)
        RegenerateRNG();
}
