#pragma once
class Controller
{
public:
	Controller() = default;
	~Controller() = default;

	virtual void Control(void *pack = nullptr) = 0;
};

