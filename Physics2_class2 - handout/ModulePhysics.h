#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2DynArray.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class b2World;
class b2Body;
class b2Vec2;

// TODO 6: Create a small class that keeps a pointer to tghe b2Body
// and has a method to request the position
// then write the implementation in the .cpp
// Then make your circle creation function to return a pointer to that class
class PhysBody
{

public:

	PhysBody(b2Body* body):body_pointer(body){}
	~PhysBody() {}

	b2Vec2 GetPosition();
	void GetRotation();

private:
	
public:
	b2Body*			body_pointer;

};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* Create_Circle(float rad);
	void Create_Box(float box_w, float box_h);
	void Create_Chain();

	// TODO 4: Move body creation to 3 functions to create circles, rectangles and chains

private:

	bool debug;
	b2World* world;

public:
	p2DynArray<PhysBody*> physbody;
};


