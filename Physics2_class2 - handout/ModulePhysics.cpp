#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"

#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;

	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	b->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// On space bar press, create a circle on mouse position
	

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
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

PhysBody* ModulePhysics::Create_Circle(float rad) {
	
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(rad);
	
	b->CreateFixture(&shape,1.0f);
	LOG("circle");

	return new PhysBody(b);
}
PhysBody* ModulePhysics::Create_Box(float box_w, float box_h){

	// TODO 1: When pressing 2, create a box on the mouse position
	b2BodyDef body_box_def;
	body_box_def.type = b2_dynamicBody;
	body_box_def.position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	b2Body* box_body = world->CreateBody(&body_box_def);

	b2PolygonShape box_shape;
	box_shape.SetAsBox(PIXEL_TO_METERS(box_w), PIXEL_TO_METERS(box_h));
	//b2FixtureDef fixture;

	// TODO 2: To have the box behave normally, set fixture's density to 1.0f
	box_body->CreateFixture(&box_shape, 1.0f);
	LOG("box");

	return new PhysBody(box_body);
}
PhysBody* ModulePhysics::Create_Chain(){

	// TODO 3: Create a chain shape using those vertices
	// remember to convert them from pixels to meters!
	b2BodyDef body_def_chain;
	body_def_chain.type = b2_dynamicBody;
	body_def_chain.position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));

	b2Body* chain_body = world->CreateBody(&body_def_chain);

	b2ChainShape chain_shape;
	b2Vec2 vects[39];

	int points[78] = {
		46, 132,
		34, 137,
		39, 126,
		22, 124,
		30, 116,
		9, 102,
		28, 91,
		0, 76,
		28, 63,
		13, 35,
		41, 38,
		42, 18,
		40, 0,
		57, 12,
		69, 23,
		74, 32,
		82, 18,
		86, 5,
		92, 19,
		94, 40,
		111, 35,
		103, 57,
		105, 62,
		116, 65,
		109, 73,
		110, 87,
		105, 93,
		110, 100,
		103, 105,
		100, 117,
		105, 122,
		103, 126,
		98, 127,
		96, 138,
		87, 146,
		78, 149,
		64, 147,
		55, 143,
		49, 137
	};

	for (int i = 0; i < 39; i++) {
		vects[i].Set(PIXEL_TO_METERS(points[(i * 2)]), PIXEL_TO_METERS(points[(i * 2) + 1]));
	}
	chain_shape.CreateLoop(vects, 39);

	chain_body->CreateFixture(&chain_shape, 4.0f);

	return new PhysBody(chain_body);
}


void PhysBody::GetPosition(int& x, int&y) {

	b2Vec2 aux_vec;

	aux_vec = body_pointer->GetPosition();
	x = (METERS_TO_PIXELS(aux_vec.x));
	LOG("Circle pos x: %f", aux_vec.x);
	y = (METERS_TO_PIXELS(aux_vec.y));
	LOG("Circle pos y: %f", aux_vec.y);

}


void PhysBody::GetRotation(float& angl){

	angl = body_pointer->GetAngle()*57.2957f;
	LOG("Circle angle %f", angl);

}