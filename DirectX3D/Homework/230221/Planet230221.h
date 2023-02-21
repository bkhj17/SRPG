#pragma once
#include "Cube230221.h"
class Planet230221 : public Cube230221
{
public:
	Planet230221(wstring fileName, Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	~Planet230221() = default;

	virtual void Update();

	void SetRotSpeedY(float speed) { rotSpeedY = speed; }
	void SetRotSpeedA(float speed) { rotSpeedA = speed; }

	void SetAnchor(Transform* anchor) { 
		this->anchor = anchor; 
		Pos() = anchor->Pos();
	}
	void SetFarAchor(Vector3 vec) { farAnchor = vec; }

private:
	Transform* anchor = nullptr;
	Vector3 farAnchor;

	float rotSpeedY = 1.0f;
	float rotSpeedA = 1.0f;
};

