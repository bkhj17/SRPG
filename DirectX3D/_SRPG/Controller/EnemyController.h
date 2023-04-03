#pragma once

class EnemyController : public Controller
{
public:
	EnemyController() = default;
	~EnemyController() = default;

	void Control(void* pack = nullptr) override;
};

