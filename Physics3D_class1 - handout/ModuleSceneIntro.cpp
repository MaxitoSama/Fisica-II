#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	// TODO 2: Place the camera one unit up in Y and one unit to the right
	// experiment with different camera placements, then use LookAt()
	// to make it look at the center
	App->camera->Move({ 0,1,0 });
	App->camera->LookAt({ 0,0,0 });

	vec3 center = { 0,0,0 };

	radius= sqrt(pow(App->camera->Position.x - center.x, 2) + pow(App->camera->Position.y - center.y, 2) + pow(App->camera->Position.z - center.z, 2));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// TODO 1: Create a Plane primitive. This uses the plane formula
	// so you have to express the normal of the plane to create 
	// a plane centered around 0,0. Make that it draw the axis for reference

	// TODO 6: Draw a sphere of 0.5f radius around the center
	// Draw somewhere else a cube and a cylinder in wireframe

	Color colore(255.0f, 0.0f, 100.0f, 0.0f);

	Plane first_plane(0,1,0,0);
	first_plane.axis=true;
	first_plane.Render();

	Sphere first_sphere(0.5f);
	first_sphere.SetPos(0, 0, 0);
	first_sphere.color = colore;
	first_sphere.Render();

	Sphere second_sphere(0.5f);
	second_sphere.SetPos(1.0f, 0, 0);
	second_sphere.color = colore;
	second_sphere.wire = true;
	second_sphere.Render();

	Sphere third_sphere(0.52f);
	third_sphere.SetPos(0.5f, 5, 0);
	third_sphere.color = colore;
	third_sphere.wire = true;
	third_sphere.Render();

	if(App->input->GetMouseButton(1)==KEY_REPEAT)
	{
		MouseRotation();
	}
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::MouseRotation()
{
	float x;
	float frac;
	float coseno;
	vec3 camera;

	x=App->input->GetMouseXMotion();
	if (x > 0 )
	{
		frac = sinf(1 / radius);
		coseno = cos(frac);
		camera = { frac,0,coseno };
	}
	else if (x < 0)
	{
		frac = sinf(1 / radius);
		coseno = cos(frac);
		camera = { -frac,0,-coseno };
	}
	else
	{
		camera = { 0,0,0 };
	}

	

	LOG("impulse: %f", x);
	App->camera->Move(camera);
	App->camera->LookAt({ 0,0,0 });


}