#pragma once

class FloatingDamage : public Quad
{
public:
	FloatingDamage(wstring file) : Quad(file) { isActive = false; }
	~FloatingDamage() = default;

	void Spawn(Vector3 pos, int damage = 1);

	void Update();

private:
	float t = 0.0f;

	float startY = 0.0f, endY = 0.0f;

};

