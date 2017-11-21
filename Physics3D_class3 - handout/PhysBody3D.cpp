#include "Application.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// TODO 4: Fill in PhysBody methods. Remember to use 
// get/setOpenGLMatrix methods

class Application;

// ---------------------------------------------------------
PhysBody3D::PhysBody3D(btRigidBody* body)
{
	this->body = body;
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	body->getWorldTransform().getOpenGLMatrix(matrix);
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	body->getWorldTransform().setFromOpenGLMatrix(matrix);
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btMatrix3x3 matrix;
	GetTransform(matrix);

	btTransform trans(matrix, x, y, z);
	
	

	matrix

	SetTransform(&matrix);

}
