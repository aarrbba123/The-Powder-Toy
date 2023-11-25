#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_PLAT()
{
	Identifier = "DEFAULT_PT_PLAT";
	Name = "PLAT";
	Colour = PIXPACK(0xaa8800);
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.20f;
	HotAir = 0.000f  * CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 10;
	DefaultProperties.bio.health = 500;
	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 29;
	Description = "Platelets. Form scar tissue when in contact with skin.";

	Properties = TYPE_LIQUID|PROP_NEUTPENETRATE|TYPE_BIO;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{

	int rand_x =  RNG::Ref().between(-1, 1);
	int rand_y =  RNG::Ref().between(-1, 1);

	if (BOUNDS_CHECK && (rand_x || rand_y)){ 

		int pos = pmap[y + rand_y][x + rand_x];
		int target = ID(pos);
		int target_type = parts[target].type;

		//Metastasis code
		if (target_type == PT_SKINS ||
            target_type == PT_SKIND ||
            target_type == PT_SKINE) {

			sim->part_change_type(i, x, y, PT_SCAR);
		}
	}

	return 0;
}