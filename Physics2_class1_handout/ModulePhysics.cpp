#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "Box2D/Box2D/Box2D.h"
#include <cstdlib>
#include <time.h>

// TODO 1: Include Box 2 header and library
#ifdef _DEBUG
#pragma comment (lib,"Box2D/libx86/Debug/Box2D.lib")

#else
#pragma comment (lib,"Box2D/libx86/Release/Box2D.lib")

#endif // _DEBUG




ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");
	
	// TODO 2: Create a private variable for the world
	
	
	// - You need to send it a default gravity
	b2Vec2 gravity(0.0f, 9.81f);

	// -You need init the world in the constructor
	// - Remember to destroy the world after using it
	world = new b2World(gravity);
	

	// TODO 4: Create a a big static circle as "ground"
	
	//Definition of the body
	b2BodyDef body_def_circle;
	b2BodyDef body_def_box;

	body_def_circle.type = b2_staticBody;
	body_def_box.type = b2_staticBody;

	body_def_circle.position.Set(PIXEL_TO_METERS(SCREEN_WIDTH/2), PIXEL_TO_METERS(SCREEN_HEIGHT/2));
	body_def_box.position.Set(PIXEL_TO_METERS(0.0f), PIXEL_TO_METERS(1100));

	//We use the world deffined to create the body
	b2Body* body_circle;
	b2Body* body_box;

	body_circle = world->CreateBody(&body_def_circle);
	body_box = world->CreateBody(&body_def_box);

	//We define the shape fixture 
	b2CircleShape circle;
	b2PolygonShape box;

	circle.m_radius = PIXEL_TO_METERS(200);
	box.SetAsBox(PIXEL_TO_METERS(SCREEN_WIDTH), 10.0f);

	body_circle->CreateFixture(&circle, 0.0f);
	body_box->CreateFixture(&box, 0.0f);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	world->Step(1.0f/60.0f,8,3);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		CircleMouse();

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;
				case b2Shape::e_polygon:
				{
					b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					SDL_Rect rect = { METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y),METERS_TO_PIXELS(100.0f),METERS_TO_PIXELS(100.0f)};
					App->renderer->DrawQuad(rect,0, 255, 255, 255,true);
				}
				break;

				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void ModulePhysics::CircleMouse() {

	LOG("New Circle");

	srand(time(NULL));
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	int rand_rad = rand()%15+10;

	b2BodyDef new_body_def;
	new_body_def.type = b2_dynamicBody;
	new_body_def.position.Set(PIXEL_TO_METERS(mouse_x), PIXEL_TO_METERS(mouse_y));

	b2Body* new_body;
	new_body = world->CreateBody(&new_body_def);


	//We define the shape fixture 
	b2CircleShape new_circle;
	new_circle.m_radius = PIXEL_TO_METERS(rand_rad);

	new_body->CreateFixture(&new_circle, 0.0f);

}
