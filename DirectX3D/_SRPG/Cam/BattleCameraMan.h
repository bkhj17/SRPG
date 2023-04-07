#pragma once
class BattleCameraMan : public Transform
{
public:
	BattleCameraMan();
	~BattleCameraMan();

	void Update();

	void SetView();
private:
	Camera* camera;
};

