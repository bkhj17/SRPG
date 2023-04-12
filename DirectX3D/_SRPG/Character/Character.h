#pragma once


class Character : public SRPGObject
{
private:
	friend class CharacterManager;

	const float CURSOR_HEIGHT = 20.0f;

	enum AnimState {
		IDLE, RUN, HIT, DIE, SWORD_ATTACK, BOW_ATTACK
	};

	Character(ModelAnimatorInstancing* instancing);
	~Character();
public:
	void Update() override;
	void Render() override;

	void SetMovePath(vector<Vector3>& path);

	bool IsActing();
	int GetMaxMove() { return IsMovable() ? status.move : 0; }
	pair<int, int> GetAttackRange() { return acted ? make_pair(0, 0) : (weapon ? weapon->GetRange() : status.attackRange); }

	void SetWeapon(Weapon* weapon, int boneNum = 37);
	Weapon* GetWeapon() { return weapon; }
	
	void SetAttackAnim();

	int CalcAttack();
	void AttackStart(SRPGObject* defender);

	void SetInstancing(ModelAnimatorInstancing* instancing);

	virtual float GetCursorHeight() override { return CURSOR_HEIGHT; }
private:
	void SetAnimState(AnimState state);
	void AttackEnd();

	void Damaged(int damage) override;

	void Die() override;

	void SetEvent(int clip, Event event, float timeRatio);
	void ExecuteEvent();
private:
	AnimState animState = IDLE;				//현재 애니메이션
	//인스턴싱 관련
	int index = -1;
	Transform* bodyTransform = nullptr;
	ModelAnimatorInstancing* instancing = nullptr;
	ModelAnimatorInstancing::Motion* motion = nullptr;
	vector<map<float, Event>> totalEvents;
	vector<map<float, Event>::iterator> eventIters;

	//무기
	Transform* weaponOwner;
	int weaponBoneNum = -1;
	Weapon* weapon = nullptr;

	Cylinder* actCylinder;

};

