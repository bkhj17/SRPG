#include "framework.h"
#include "Robot.h"

Robot::Robot(Transform* transform)
	: transform(transform)
{
	collider = new SphereCollider;
	collider->SetParent(transform);
}

Robot::~Robot()
{
	delete collider;
}

void Robot::Update()
{
}

void Robot::Render()
{
}
