#pragma once

class EnemyController : public Controller
{
public:
	EnemyController() = default;
	~EnemyController() = default;

	void Control(void* pack = nullptr) override;

private:
	void CharacterAct(GridedTerrain* terrain, SRPGObject* target, int tile);

private:
	Character* holded = nullptr;
};

