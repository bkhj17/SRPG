#pragma once

class SRPGScene : public Scene
{
private:
	enum SceneState {
		PLAYING, WIN, LOSE
	};

public:
	SRPGScene();
	~SRPGScene();

	virtual void Start() override;
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
private:
	void Control();

	void InputAction();
	void InputAttackAction();

	void CharacterMoveEnd(void* characterPtr);
private:
	class GridedTerrain* terrain;
	class MapCursor* mapCursor;

	int inputMode = 0;

	SceneState state = PLAYING;

	BattleCameraMan* battleCamera;
};

