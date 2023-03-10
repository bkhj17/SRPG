#include "framework.h"
#include "TestScene230310.h"
#include "GridedTerrain.h"

TestScene230310::TestScene230310()
{
	terrain = new GridedTerrain;
}

TestScene230310::~TestScene230310()
{
	delete terrain;
}

void TestScene230310::Update()
{
	terrain->UpdateWorld();
}

void TestScene230310::PreRender()
{
}

void TestScene230310::Render()
{
	terrain->Render();
}

void TestScene230310::PostRender()
{
}

void TestScene230310::GUIRender()
{
	terrain->GUIRender();
}
