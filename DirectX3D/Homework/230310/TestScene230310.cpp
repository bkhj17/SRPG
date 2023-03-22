#include "framework.h"
#include "TestScene230310.h"
#include "GridedTerrain.h"
#include "MapCursor.h"
#include "Character.h"

TestScene230310::TestScene230310()
{
	terrain = new GridedTerrain;


	testCharacter = new Character;
	pair<int, int> coord = { 5, 5 };
	testCharacter->Pos() = terrain->CoordToPos(coord.first, coord.second);
	testCharacter->UpdateWorld();

	terrain->AddObject(testCharacter);

	cursor = new MapCursor;
	cursor->SetGridTerrain(terrain);
	cursor->GetW() = 5;
	cursor->GetH() = 5;

	Observer::Get()->AddParamEvent("CharacterMoveEnd", bind(&TestScene230310::CharacterMoveEnd, this, placeholders::_1));
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

	cursor->Update();

	if (testCharacter->IsActing())
		return;
	
	if (KEY_DOWN(VK_SPACE))
		terrain->InputAction(cursor->GetW(), cursor->GetH());

}

void TestScene230310::PreRender()
{
}

void TestScene230310::Render()
{
	terrain->Render();
	cursor->Render();
	testCharacter->Render();
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
