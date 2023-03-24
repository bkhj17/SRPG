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
	
	CharacterManager::Get();

	testCharacter = new Character;
	testCharacter->Pos() = terrain->CoordToPos(coord.first, coord.second);
	testCharacter->UpdateWorld();
	terrain->AddObject(testCharacter);

	coord = { 6, 8 };
	testCharacter2 = new Character;
	testCharacter2->Pos() = terrain->CoordToPos(coord.first, coord.second);
	testCharacter2->UpdateWorld();
	terrain->AddObject(testCharacter2);

	cursor = new MapCursor;
	cursor->SetGridTerrain(terrain);
	cursor->SetPosCoord(5, 5, true);

	Observer::Get()->AddParamEvent("CharacterMoveEnd", bind(&TestScene230310::CharacterMoveEnd, this, placeholders::_1));
}

TestScene230310::~TestScene230310()
{
	delete terrain;
	delete testCharacter;
	delete testCharacter2;
	delete cursor;

	CharacterManager::Delete();
}

void TestScene230310::Update()
{
	terrain->Update();

	testCharacter->Update();
	testCharacter2->Update();

	if (testCharacter->IsActing() || testCharacter2->IsActing())
		return;
	
	cursor->Update();
	if (KEY_DOWN(VK_SPACE))
		terrain->InputAction(cursor->GetW(), cursor->GetH());

}

void TestScene230310::PreRender()
{
}

void TestScene230310::Render()
{
	terrain->Render();
	testCharacter->Render();
	testCharacter2->Render();

	if (testCharacter->IsActing() || testCharacter2->IsActing())
		return;

	cursor->Render();
}

void TestScene230310::PostRender()
{
}

void TestScene230310::GUIRender()
{
	terrain->GUIRender();
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
