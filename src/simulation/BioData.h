//Header/Include Guards;Saves your time,AND your eyes.DO NOT forget to use them.
#ifndef BIODATA_H
#define BIODATA_H
/*
I realized that rather than slowly growing the Particle
struct it makes more sense to just made a bio data struct
*/
struct BioData 
{
    int o2;
    int co2;
    int health;
    int radDamage;
    int maxHealth;
};
#endif /*BIODATA_H*/
