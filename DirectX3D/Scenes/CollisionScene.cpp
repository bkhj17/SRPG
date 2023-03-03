#include "framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	colliders.push_back(new BoxCollider);
	colliders[0]->SetTag("0");
	colliders.push_back(new BoxCollider);
	colliders[1]->SetTag("1");
}

CollisionScene::~CollisionScene()
{
	for (auto collider : colliders)
		delete collider;
}

void CollisionScene::Update()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
	//Vector3 contectPos;
	//if (colliders[0]->IsRayCollision(ray, contectPos)) {
	//	colliders[0]->SetColor(1, 0, 0);
	//	colliders[1].pos = contectPos;
	//} else
	//	colliders[0]->SetColor(0, 1, 0);
		
	if (colliders[0]->IsCollision(colliders[1])) {
		colliders[0]->SetColor(1, 0, 0);
		colliders[1]->SetColor(1, 0, 0);
	}
	else {
		colliders[0]->SetColor(0, 1, 0);
		colliders[1]->SetColor(0, 1, 0);
	}

	for (auto collider : colliders)
		collider->UpdateWorld();
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{

	for (auto collider : colliders)
		collider->Render();
}

void CollisionScene::PostRender()
{
}

void CollisionScene::GUIRender()
{
	for (auto collider : colliders)
		collider->RenderUI();
}
