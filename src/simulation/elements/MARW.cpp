#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_MARW()
{
	Identifier = "DEFAULT_PT_MARW";
	Name = "MARW";
	Colour = PIXPACK(0xc00044);
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

	DefaultProperties.bio.health = 100;
	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	DefaultProperties.bio.maxHealth = 100;
	DefaultProperties.tmp = MAX_O2;
	HeatConduct = 29;
	Description = "Bone marrow. Produces new cells.";

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
}

static int update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;
	int rad = parts[i].tmp;
	int max_health = parts[i].tmp2;

    rx =  RNG::Ref().between(-2, 2);
    ry =  RNG::Ref().between(-2, 2);

    // O2 use by marrow itself
    if (RNG::Ref().chance(1, 150)){

		if (parts[i].bio.o2 > 0){
        	parts[i].bio.o2 -= 1;
            parts[i].bio.co2 += 1;
		}
    }

    
    if (BOUNDS_CHECK && (rx || ry))
    {
        r = pmap[y+ry][x+rx];
		int er = sim->photons[y][x];
        if (r) {
			if (RNG::Ref().chance(1, 2)){
				// Diffuse among bio
				if (sim->elements[TYP(r)].Properties & TYPE_BIO && TYP(r) != PT_BLD){
					int ir = ID(r);

					if (parts[i].bio.o2 > parts[ir].bio.o2){
						parts[i].bio.o2--;
						parts[ir].bio.o2++;
					}
					if (parts[i].bio.co2 > parts[ir].bio.co2){
						parts[i].bio.co2--;
						parts[ir].bio.co2++;
					}
				}
			}
			//Radiation damage is back, maybe.
			if (sim->elements[TYP(r)].MenuSection == SC_NUCLEAR || sim->elements[TYP(er)].MenuSection == SC_NUCLEAR){
				parts[i].bio.radDamage++;
				if (RNG::Ref().chance(parts[i].bio.radDamage, 100000)){
					sim->part_change_type(i, x, y, PT_TUMOR);
					return 0;
				}
			}
        }
    }

	// Health management
	if (RNG::Ref().chance(1, 50)){
		// Temp check
		if (parts[i].temp > 323.15){
			int damage = (parts[i].temp - 315) / 5;
			parts[i].bio.health -= damage;
		}
		// Damage check
		if (parts[i].bio.co2 > MAX_CO2 || parts[i].bio.o2 < 1){
			parts[i].bio.health--;
		}
		// Otherwise heal
		else{
			if (parts[i].bio.health < parts[i].tmp){
				parts[i].bio.health++;
			}
		}
	}

	// Death check
	if (parts[i].bio.health < 1){
		sim->part_change_type(i, x, y, PT_DT);
	}

	// New cell creation
	if (
		parts[i].life > 300 &&
		parts[i].bio.o2 > 10 &&
		parts[i].bio.co2 < 10
	){
		rx =  RNG::Ref().between(-1, 1);
    		ry =  RNG::Ref().between(-1, 1);

		r = pmap[y+ry][x+rx];
		int t = TYP(r);
		int ir = ID(r);

		if (t == 0){
			sim->create_part(-1, x + rx, y + ry, PT_WBLD);
			parts[i].bio.o2 -= 10;
			parts[i].bio.co2 += 10;
		}
		else if (t == PT_BLD){
			sim->part_change_type(ir, x + rx, y + ry, PT_WBLD);
			parts[i].bio.o2 -= 10;
			parts[i].bio.co2 += 10;
		}
	}

	// Incrament time since last WBLD seen
	parts[i].life++;

	// Check for existing cells
	for (rx=-5; rx<=5; rx++)
		for (ry=-5; ry<=5; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if (TYP(r)==PT_WBLD)
				{
					parts[i].life = 0;
				}
			}	

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
    // Oxygen
    int o = cpart->bio.o2;

    // C02
    int c = cpart->bio.co2;

	*colr = (int)fmax(7 * o, 100);
	*colg = 0;
	*colb = (int)fmax(5 * o, 30);
	*pixel_mode |= PMODE_BLUR;

	// Life mix
	*colr = int(*colr * (cpart->bio.health) / 100.0f);
	*colg = int(*colg * (cpart->bio.health) / 100.0f);
	*colb = int(*colb * (cpart->bio.health) / 100.0f);

	return 0;
}
