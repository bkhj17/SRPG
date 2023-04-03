#pragma once

class MainSelectUI : public SelectUI
{
public:
	MainSelectUI(Vector3 pos = Vector3());
	~MainSelectUI() = default;

	void ActiveFunc() override;
};

