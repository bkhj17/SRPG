#pragma once


class Character : public Transform
{
private:
	friend class CharacterManager;

	enum AnimState {
		IDLE, RUN, ATTACK, HIT, DIE
	};

	Character();
	~Character();
public:
	enum Team {
		PLAYER, ENEMY, NONE
	};

	struct Status {
		string name = "";
		int teamNum = NONE;

		int maxHp = 2;
		int curHp = 2;

		int attack = 3;
		int defence = 3;

		int move = 3;
		pair<int, int> attackRange = { 1, 1 };	//최소, 최대 공격사거리. 나중에 무기 스펙으로

	};
public:
	void Init();

	void Update();
	void Render();

	void TurnStart();
	
	void ActEnd() { acted = true; originPos = Pos(); }
	bool IsActed() { return acted; }

	void SetMovePath(vector<Vector3>& path);

	bool IsActing();
	int GetMaxMove() { return IsMovable() ? status.move : 0; }
	pair<int, int> GetAttackRange() { return acted ? make_pair(0, 0) : status.attackRange; }

	void SetDir(Vector3 dir);

	bool IsMoved() { return moved; }
	bool IsMovable() { return !acted && !moved; }
	void CancelMove();

	const Status& GetStatus() { return status; }
private:
	bool IsMoving();
	void Move();

	void SetAnimState(AnimState state);
	void AttackEnd();

	void AttackHit();
	void Damaged(int damage);
private:
	ModelAnimator* body;					//위치 테스트 용. 추후 모델 애니메이터로 변경
	AnimState animState = IDLE;				//현재 애니메이션

	Weapon* weapon;

	bool acted = false;						//해당 턴 행동 여부 : 공격, 혹은 행동 완료 선택 시 true로 변경. 턴 시작시 false
	bool moved = false;						//해당 턴 이동 여부
	Vector3 originPos = {};					//행동 선택 전의 위치

	Status status;

	float lerpValue = 0.0f;					//위치 이동 선형 보간을 위한 저장값. 칸 여러개를 거쳐야 하기에 비선형은 부자연스럽다
	float moveSpeed = 0.2f;					//이동 속도(보간값 변화) 
	Vector3 dir = { 0, 0, -1 };				//보고 있는 방향
	vector<Vector3> movePath;

	IntValueBuffer* valueBuffer;
};

