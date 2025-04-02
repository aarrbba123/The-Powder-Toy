// It's much better to define the rng here.
// Saves on more RNG calls.
// I took some of the PRNG code from VIBR and repurposed it here.
// (And tpt-rand)

class BioRNG
{
    public:
        BioRNG();
        int RBetween(int range);
        int Between(int lower, int upper);
        bool Chance(unsigned int number);
        void RegenerateRNG();
        RNG PureRNG;
        unsigned int rngVal;

    protected:
        void OverusedCheck();
        int Next();
        unsigned short useCount;
};
