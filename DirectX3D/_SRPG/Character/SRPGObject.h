#pragma once

class SRPGObject : public Transform
{
private:
	const float CURSOR_HEIGHT = 3.0f;
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

		int attack = 3;		//CalcAttack 있으니 직접 뽑아 쓰지 말 것
		int defence = 2;

		int move = 3;
		pair<int, int> attackRange = { 1, 1 };	//최소, 최대 공격사거리. 나중에 무기 스펙으로
	};
	SRPGObject();
	virtual ~SRPGObject();
public:
	virtual void Init();
	
	virtual void Update();
	virtual void Render() {};
	virtual void PostRender();

	virtual void TurnStart();
	virtual void ActEnd() { acted = true; originPos = Pos(); }
	virtual bool IsActed() { return acted; }

	void SetDir(Vector3 dir);

	void SetMovePath(vector<Vector3>& path);

	bool IsMoved() { return moved; }
	bool IsMovable() { return !acted && !moved; }
	void CancelMove();

	virtual bool IsActing() { return Active() && IsMoving(); }

	const Status& GetStatus() { return status; }

	virtual float GetCursorHeight() { return CURSOR_HEIGHT; }
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

	bool acted = false;						//해당 턴 행동 여부 : 공격, 혹은 행동 완료 선택 시 true로 변경. 턴 시작시 false
	bool moved = false;						//해당 턴 이동 여부
	Vector3 originPos = {};					//행동 선택 전의 위치

	float lerpValue = 0.0f;					//위치 이동 선형 보간을 위한 저장값. 칸 여러개를 거쳐야 하기에 비선형은 부자연스럽다
	Vector3 curPos = {};					//이전 위치
	float moveSpeed = 3.0f;					//이동 속도(보간값 변화) 
	Vector3 dir = { 0, 0, -1 };				//보고 있는 방향
	vector<Vector3> movePath;
};

