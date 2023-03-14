#pragma once

class Instancing230313 : public ModelAnimatorInstancing
{
public:
	Instancing230313(string name);
	~Instancing230313() = default;

	Transform* Add(Transform* transform);
	vector<Transform*>& GetTransforms() { return transforms; }

	void Update() override;
	void Render() override;
	void PostRender();

	bool Spawn(Vector3 pos);

	void SetEvent(UINT instanceID, int clip, Event event);
	void Hit(Collider* collider);
protected:
	virtual void UpdateTransforms();
private:
	int curTargetNum = 0;
};

