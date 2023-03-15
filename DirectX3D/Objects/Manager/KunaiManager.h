#pragma once
class KunaiManager : public Singleton<KunaiManager>
{
private:
	const UINT SIZE = 30;

	friend class Singleton;
	KunaiManager();
	~KunaiManager();
public:
	void Update();
	void Render();

	void Throw(Vector3 pos, Vector3 dir);

	bool IsCollision(Collider* collider);
private:
	ModelInstancing* kunaiInstancing;
	vector<Kunai*> kunaies;
};