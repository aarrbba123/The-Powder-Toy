#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_SKINE()
{
	Identifier = "DEFAULT_PT_SKINE";
	Name = "SKINE";
	Colour = 0xE8C89B_rgb;
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 2;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 1;
	Description = "The Epidermis. The outermost layer of skin.";

	Properties = TYPE_SOLID|TYPE_BIO;

	// Bio stuff

	Max_O2 = 100;
	Max_CO2 = 100;

	// Outer skin is pretty tough
	Max_Health = 200;

	DefaultProperties.bio.o2 = Max_O2;
	DefaultProperties.bio.co2 = 0;
	DefaultProperties.bio.health = Max_Health;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 7.0f;
	HighPressureTransition = PT_NONE;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS)
{
    // O2 use itself
    Biology::UseO2(150, UPDATE_FUNC_SUBCALL_ARGS);
    // Diffuse resources
	Biology::DiffuseResources(2, 2, UPDATE_FUNC_SUBCALL_ARGS);
    // Radiation damage
	Biology::DoRadiationDamage(2, 2, UPDATE_FUNC_SUBCALL_ARGS);
	// Damage from extreme heat or cold
	// Skin is a bit more resilient against temperature
	Biology::DoHeatDamage(20, 323.15, 273, UPDATE_FUNC_SUBCALL_ARGS);
	// Damage from lack of O2 or too much CO2
	Biology::DoRespirationDamage(100, UPDATE_FUNC_SUBCALL_ARGS);
	// Heal naturally
	Biology::DoHealing(100, UPDATE_FUNC_SUBCALL_ARGS);
	// Death check
	Biology::HandleDeath(UPDATE_FUNC_SUBCALL_ARGS);

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
	// Life mix + base colour
	*colr = int(232 * (cpart->bio.health) / 200.0f);
	*colg = int(200 * (cpart->bio.health) / 200.0f);
	*colb = int(155 * (cpart->bio.health) / 200.0f);

	return 0;
}