#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_TUMOR()
{
	Identifier = "DEFAULT_PT_TUMOR";
	Name = "TUMR";
	Colour = 0x554040_rgb;
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
	Hardness = 12;

	Weight = 150;

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 29;
	Description = "Tumor. Certain death/blobification for biology";

	Properties = TYPE_SOLID|PROP_NEUTPENETRATE|TYPE_BIO|TYPE_DISEASE;

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
	Max_CO2 =  100;

	DefaultProperties.bio.health = 250; // In real life, cancer survives better than regular tissue (citation: HeLa)
	DefaultProperties.bio.o2 = Max_O2;
	DefaultProperties.bio.co2 = 0;
}

static int update(UPDATE_FUNC_ARGS)
{
	// O2 use itself (Increased due to increased biological activity)
    Biology::UseO2(100, UPDATE_FUNC_SUBCALL_ARGS);
	// Steal resources from biological material
	Biology::StealResources(2, 2, UPDATE_FUNC_SUBCALL_ARGS);

	int r, rx, ry;

    rx =  sim->biorng.RBetween(2);
    ry =  sim->biorng.RBetween(2);
    
    if (sim->biorng.Chance(2000) && (rx || ry))
    {
        r = pmap[y+ry][x+rx];
		int target = ID(r);
		int target_type = parts[target].type;

        if (r) {
			// Metastasize
			if (target_type == PT_BLD){
				parts[target].ctype = PT_TUMOR;
			}	
        }
    }
	if (sim->biorng.Chance(100)){

		r = pmap[y+ry][x+rx];
		int target = ID(r);
		int target_type = parts[target].type;
		auto &sd = SimulationData::CRef();

		// convert biology to tumor (grow)
		if (sd.elements[target_type].Properties & TYPE_BIO && target_type != PT_TUMOR){
			
			sim->part_change_type(target, parts[target].x, parts[target].y, PT_TUMOR);
			parts[i].bio.o2--;
			parts[i].bio.co2++;
		}
	}

	// Health related things
	Biology::DoHeatDamage(50, 323.15, 273, UPDATE_FUNC_SUBCALL_ARGS);
	Biology::DoRespirationDamage(50, UPDATE_FUNC_SUBCALL_ARGS);
	Biology::DoHealing(50, UPDATE_FUNC_SUBCALL_ARGS);
	Biology::HandleDeath(UPDATE_FUNC_SUBCALL_ARGS);

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
    // Oxygen
    int o = cpart->bio.o2;

	*colr = (int)fmin(95, fmax(3 * o, 77));
	*colg = (int)fmin(69, fmax(3 * o, 62));
	*colb = (int)fmin(69, fmax(3 * o, 62));
	*pixel_mode |= PMODE_BLUR;

	// Life mix
	*colr = int(*colr * (cpart->bio.health) / 140.0f);
	*colg = int(*colg * (cpart->bio.health) / 140.0f);
	*colb = int(*colb * (cpart->bio.health) / 140.0f);

	return 0;
}
