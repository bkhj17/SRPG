#include "framework.h"
#include "TestScene230310.h"
#include "GridedTerrain.h"
#include "MapCursor.h"
#include "Character.h"
#include "CharacterManager.h"

TestScene230310::TestScene230310()
{
	terrain = new GridedTerrain;

	pair<int, int> coord = { 5, 5 };
	
	Character* test = CharacterManager::Get()->Spawn();
	test->Pos() = terrain->CoordToPos(coord.first, coord.second);
	test->UpdateWorld();
	terrain->AddObject(test);

	coord = { 6, 8 };
	test = CharacterManager::Get()->Spawn();
	test->Pos() = terrain->CoordToPos(coord.first, coord.second);
	test->UpdateWorld();
	terrain->AddObject(test);

	//코드가 안 예뻐......

	cursor = new MapCursor;
	cursor->SetGridTerrain(terrain);
	cursor->SetPosCoord(5, 6, true);
	Observer::Get()->AddParamEvent("CharacterMoveEnd", bind(&TestScene230310::CharacterMoveEnd, this, placeholders::_1));
}

TestScene230310::~TestScene230310()
{
	delete terrain;
	delete cursor;

	CharacterManager::Delete();
}

void TestScene230310::Update()
{
	terrain->Update();
	CharacterManager::Get()->Update();

	if (CharacterManager::Get()->IsActing())
		return;
	
	Control();
}

void TestScene230310::PreRender()
{
}

void TestScene230310::Render()
{
	terrain->Render();
	CharacterManager::Get()->Render();

	if (!CharacterManager::Get()->IsActing())
		cursor->Render();
}

void TestScene230310::PostRender()
{
}

void TestScene230310::GUIRender()
{
	terrain->GUIRender();
}

void TestScene230310::Control()
{
	cursor->Update();
	if (KEY_DOWN(VK_SPACE))
		terrain->InputAction(cursor->GetW(), cursor->GetH());
}

void TestScene230310::CharacterMoveEnd(void* characterPtr)
{
	auto character = (Character*)characterPtr;
	if (!character) //wrong call
		return;

	pair<int, int> characterPosCoord = terrain->PosToCoord(character->GlobalPos());
	cursor->SetPosCoord(characterPosCoord.first, characterPosCoord.second, true);
	terrain->CheckMovableArea();
}
