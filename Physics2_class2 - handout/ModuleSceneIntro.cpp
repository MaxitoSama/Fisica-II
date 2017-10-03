#include "Globals.h"
#include "p2List.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");

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
	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
	
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->physics->physbody_circle.PushBack(App->physics->Create_Circle(25.0f));
		LOG("%d", App->physics->physbody_circle.Count());
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		App->physics->physbody_box.PushBack(App->physics->Create_Box(50.0f, 25.0f));

	}

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		App->physics->physbody_chain.PushBack(App->physics->Create_Chain());
	}
	
	// TODO 7: Draw all the circles using "circle" texture
	for (int i = 0; i < App->physics->physbody_circle.Count() ; i++) 
	{		
		int x, y;
		float ang;
		App->physics->physbody_circle[i]->GetPosition(x,y);
		App->physics->physbody_circle[i]->GetRotation(ang);
		App->renderer->Blit(circle, x-25, y-25,NULL,NULL,ang);
	}
	for (int i = 0; i < App->physics->physbody_box.Count(); i++)
	{
		int x, y;
		float ang;
		App->physics->physbody_box[i]->GetPosition(x, y);
		App->physics->physbody_box[i]->GetRotation(ang);
		App->renderer->Blit(box, x-50, y-25, NULL, NULL, ang);
	}
	for (int i = 0; i < App->physics->physbody_chain.Count(); i++)
	{
		int x, y;
		float ang;
		App->physics->physbody_chain[i]->GetPosition(x, y);
		App->physics->physbody_chain[i]->GetRotation(ang);
		App->renderer->Blit(rick, x, y, NULL, NULL, ang);
	}
	return UPDATE_CONTINUE;
}
