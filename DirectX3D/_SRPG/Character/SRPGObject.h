#pragma once

class SRPGObject : public Transform
{
public:
	friend class CharacterManager;

	enum Team {
		PLAYER, ENEMY, NONE
	};

	struct Status {
		string name = "";
		int teamNum = NONE;

		int maxHp = 10;
		int curHp = 10;

		int attack = 3;		//CalcAttack ������ ���� �̾� ���� �� ��
		int defence = 2;

		int move = 3;
		pair<int, int> attackRange = { 1, 1 };	//�ּ�, �ִ� ���ݻ�Ÿ�. ���߿� ���� ��������
	};
public:
	SRPGObject();
	~SRPGObject();

	virtual void Init();
	
	virtual void Update();
	virtual void Render() {};
	virtual void PostRender();

	virtual void TurnStart();
	virtual void ActEnd() { acted = true; originPos = Pos(); }
	virtual bool IsActed() { return acted; }

	void SetDir(Vector3 dir);

	bool IsMoved() { return moved; }
	bool IsMovable() { return !acted && !moved; }
	void CancelMove();

	virtual bool IsActing() { return Active() && IsMoving(); }

	const Status& GetStatus() { return status; }
protected:
	bool IsMoving();
	void Move();

	virtual void AttackHit();
	virtual void Damaged(int damage);
	 
	virtual void Die();
	 
	virtual void UpdateHPBar();
protected:
	ProgressBar* hpBar;

	Status status;

	bool acted = false;						//�ش� �� �ൿ ���� : ����, Ȥ�� �ൿ �Ϸ� ���� �� true�� ����. �� ���۽� false
	bool moved = false;						//�ش� �� �̵� ����
	Vector3 originPos = {};					//�ൿ ���� ���� ��ġ

	float lerpValue = 0.0f;					//��ġ �̵� ���� ������ ���� ���尪. ĭ �������� ���ľ� �ϱ⿡ ������ ���ڿ�������
	float moveSpeed = 0.2f;					//�̵� �ӵ�(������ ��ȭ) 
	Vector3 dir = { 0, 0, -1 };				//���� �ִ� ����
	vector<Vector3> movePath;
};
