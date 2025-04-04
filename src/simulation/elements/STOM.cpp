#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_STOM()
{
	Identifier = "DEFAULT_PT_STOM";
	Name = "STOM";
	Colour = 0x990055_rgb;
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.97f;
	Loss = 0.50f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 8;

	Weight = 150;

	DefaultProperties.bio.health = 100;
	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	DefaultProperties.bio.maxHealth = 100;
	HeatConduct = 29;
	Description = "Stomach. Generates SACID (stomach acid) to break down food and bio.";

	Properties = TYPE_SOLID|PROP_NEUTPENETRATE|TYPE_BIO;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;

	// Bio stuff

	Max_O2 = 100;
	Max_CO2 = 100;
	Max_Health = 200;

	DefaultProperties.bio.o2 = Max_O2;
	DefaultProperties.bio.co2 = 0;
	DefaultProperties.bio.health = Max_Health;
}

static int update(UPDATE_FUNC_ARGS)
{
	// O2 use by cells
	Biology::UseO2(100, UPDATE_FUNC_SUBCALL_ARGS);
	// Diffuse resources
	Biology::DiffuseResources(2, 2, UPDATE_FUNC_SUBCALL_ARGS);
	// Radiation damage
	Biology::DoRadiationDamage(2, 2, UPDATE_FUNC_SUBCALL_ARGS);
	// Damage from extreme heat or cold
	Biology::DoHeatDamage(5, 323.15, 273, UPDATE_FUNC_SUBCALL_ARGS);
	// Damage from lack of O2 or too much CO2
	Biology::DoRespirationDamage(100, UPDATE_FUNC_SUBCALL_ARGS);
	// Heal naturally
	Biology::DoHealing(100, UPDATE_FUNC_SUBCALL_ARGS);
	// Death check
	Biology::HandleDeath(UPDATE_FUNC_SUBCALL_ARGS);

    if (parts[i].bio.glucose > 2 && parts[i].bio.o2 > 5 && parts[i].bio.health > 75) {

        // Needs to be somewhat healthy to generate acid
        if (sim->rng.chance(1, 100)){
            // Generate stomach acid
            int rand_x =  sim->rng.between(-1, 1);
            int rand_y =  sim->rng.between(-1, 1);

            if (rand_x || rand_y){ 

                int pos = pmap[y + rand_y][x + rand_x];
                int t = TYP(pos);

                if (t == 0){
                    sim->create_part(-1, x + rand_x, y + rand_y, PT_SACID);
                }
            }
            // SQUEEZE the food
            sim->pv[y/CELL][x/CELL] = sim->rng.between(-5, 5);
        }
    }

	return 0;
}


static int graphics(GRAPHICS_FUNC_ARGS)
{
    // Oxygen
    int o = cpart->bio.o2;

	*colr = (int)fmin(170, fmax(3 * o, 100));
	*colg = 0;
	*colb = (int)fmin(80, fmax(1.5 * o, 30));
	*pixel_mode |= PMODE_BLUR;

	// Life mix
	*colr = int(*colr * (cpart->bio.health) / 140.0f);
	*colg = int(*colg * (cpart->bio.health) / 140.0f);
	*colb = int(*colb * (cpart->bio.health) / 140.0f);

	return 0;
}
