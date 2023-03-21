#include "framework.h"
#include "TestScene230310.h"
#include "GridedTerrain.h"
#include "Character.h"

TestScene230310::TestScene230310()
{
	terrain = new GridedTerrain;
	testCharacter = new Character;
	pair<int, int> coord = { 5, 5 };
	testCharacter->Pos() = terrain->CoordToPos(coord.first, coord.second);
	terrain->AddObject(testCharacter);
}

TestScene230310::~TestScene230310()
{
	delete terrain;
	delete testCharacter;
}

void TestScene230310::Update()
{
	terrain->Update();

	testCharacter->Update();
}

void TestScene230310::PreRender()
{
}

void TestScene230310::Render()
{
	terrain->Render();
	testCharacter->Render();
}

void TestScene230310::PostRender()
{
}

void TestScene230310::GUIRender()
{
	terrain->GUIRender();
}
