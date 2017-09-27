#pragma once
#include "Module.h"
#include "Globals.h"

#define PIXELS_PER_METER 50.0
#define METER_PER_PIXEL 0.02

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER*m))
#define PIXEL_TO_METERS(p) ((float) METER_PER_PIXEL*p)

class b2World;
class b2BodyDef;

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();
	void CircleMouse();

private:

	b2World* world;


	bool debug;
};