#pragma once
class Controller
{
public:
	Controller() = default;
	~Controller() = default;

	virtual void Control() = 0;
};

