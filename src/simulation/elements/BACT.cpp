#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_BACT()
{
	Identifier = "DEFAULT_PT_BACT";
	Name = "BACT";
	Colour = PIXPACK(0xF433FF);
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.10f;
	HotAir = 0.000f  * CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 10;
	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 29;
	Description = "Bacteria. Infects living things (genes stored in tmp) (WIP).";

	Properties = TYPE_LIQUID|PROP_NEUTPENETRATE|TYPE_BIO|TYPE_DISEASE;

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
	DefaultProperties.bio.glucose = 200;
	DefaultProperties.bio.co2 = 0;
	DefaultProperties.bio.health = Max_Health;

}

static int update(UPDATE_FUNC_ARGS)
{
    // O2 use itself
    Biology::UseO2(300, UPDATE_FUNC_IN);
	// Bacteria attack all other biological components
	Biology::AttackBio(5, 2, 20, UPDATE_FUNC_IN);
	// Damage from extreme heat or cold
	Biology::DoHeatDamage(5, 323.15, 273, UPDATE_FUNC_IN);
	// Damage from lack of O2 or too much CO2
	Biology::DoRespirationDamage(100, UPDATE_FUNC_IN);
	// Radiation damage
	Biology::DoRadiationDamage(2, 2, UPDATE_FUNC_IN);
	// Heal naturally
	Biology::DoHealing(100, UPDATE_FUNC_IN);
	// Grow into air
	Biology::GrowInRange(100, 5, PT_NONE, UPDATE_FUNC_IN);
	// Grow onto dead tissue
	Biology::GrowInRange(100, 5, PT_DT, UPDATE_FUNC_IN);
	// Death check
	Biology::HandleDeath(UPDATE_FUNC_IN);

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
    // Oxygen
    int o = cpart->bio.o2;

    // C02
    int c = cpart->bio.co2;

	int q = cpart->bio.o2;

	int d = o - c;


	*colr = (int)fmin(d, 20) * 1;
	*colg = (int)fmin(d, 20) * 10;
	*colb = (int)fmin(d, 20) * 6;
	*pixel_mode |= PMODE_BLUR;

	//*colr = int(*colr * (cpart->bio.health) / 100.0f);
	//*colg = int(*colg * (cpart->bio.health) / 100.0f);
	//*colb = int(*colb * (cpart->bio.health) / 100.0f);

	return 0;
}

/*
int evaluateGenes(int gene, int genome)
{
	if (gene == 1)
	{
		geneValue = genome % 256;
		return geneValue;
	}
}

int modifyGenes(int gene, int newValue, int genome)
{
	
}
*/


