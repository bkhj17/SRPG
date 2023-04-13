#include "framework.h"

Character::Character(ModelAnimatorInstancing* instancing)
{
	SetInstancing(instancing);

	actCylinder = new Cylinder(5.0f, 1.0f);
	actCylinder->GetMaterial()->SetShader(L"SRPG/ActCylinder.hlsl");
	actCylinder->Pos().y += 2.0f;
	actCylinder->SetParent(this);

	weaponOwner = new Transform;
}

Character::~Character()
{
	delete actCylinder;

	if (weapon) weapon->SetOwner(nullptr);
	weapon = nullptr;

	delete weaponOwner;

	bodyTransform = nullptr;
	instancing = nullptr;
	motion = nullptr;
}

void Character::Update()
{
	if (!Active())
		return;
	__super::Update();

	if (animState <= RUN)
		SetAnimState(IsMoving() ? RUN : IDLE);

	ExecuteEvent();

	actCylinder->UpdateWorld();

	if (weapon) {
		weaponOwner->SetWorld(instancing->GetTransformByNode(index, weaponBoneNum));
		weapon->Update();
	}
}

void Character::Render()
{
	if (!Active())
		return;

	if (status.teamNum == TurnManager::Get()->GetCurPlayer() && !IsActing() && !acted)
		actCylinder->Render();
}

bool Character::IsActing()
{
	if (!Active())
		return false;

	bool acting = false;
	acting |= IsMoving();
	acting |= (animState > RUN);
	return acting;
}


void Character::SetWeapon(Weapon* weapon, int boneNum)
{
	if (this->weapon) {
		this->weapon->SetActive(false);
		this->weapon->SetOwner(nullptr);
	}

	if (weapon == nullptr) {
		weaponBoneNum = -1;
		return;
	}

	weaponBoneNum = boneNum;
	this->weapon = weapon;
	this->weapon->SetOwner(weaponOwner);
	this->weapon->SetActive(true);
}

void Character::SetAttackAnim()
{
	if (!weapon) {
		SetAnimState(SWORD_ATTACK);
		return;
	}
	
	switch (weapon->GetType())
	{
	case Weapon::BOW:
		SetAnimState(BOW_ATTACK);
		break;
	case Weapon::SWORD:
	case Weapon::AXE:
	default:
		SetAnimState(SWORD_ATTACK);
		break;
	}
}

int Character::CalcAttack()
{
	int result = status.attack;
	if (weapon)
		result += weapon->GetPower();
	
	return result;
}

void Character::AttackStart(SRPGObject* defender)
{
	//공격 아직 시작 안 됨 -> 시작명령
	//공격 방향
	Vector3 dir = defender->GlobalPos() - GlobalPos();
	dir.y = 0.0f;
	SetDir(dir);
	//defender->SetDir(-dir); //기습 당하는 느낌인게 좋겠다 싶어 제외

	SetAttackAnim();
}

void Character::SetInstancing(ModelAnimatorInstancing* instancing)
{
	this->instancing = instancing;
	index = instancing->GetTransformNum();
	bodyTransform = instancing->Add();
	bodyTransform->Scale() *= 0.1f;
	bodyTransform->Pos().y = bodyTransform->Scale().y + 0.8f;
	bodyTransform->SetParent(this);

	motion = instancing->GetMotion(index);
	totalEvents.resize(instancing->GetClipSize());
	eventIters.resize(instancing->GetClipSize());

	SetEvent(SWORD_ATTACK, bind(&Character::AttackHit, this), 0.5f);
	SetEvent(SWORD_ATTACK, bind(&Character::AttackEnd, this), 0.8f);
	SetEvent(BOW_ATTACK, bind(&Character::AttackEnd, this), 0.8f);
	SetEvent(BOW_ATTACK, bind(&Character::AttackHit, this), 0.6f);
	SetEvent(HIT, bind(&Character::SetAnimState, this, IDLE), 0.9f);
	SetEvent(DIE, bind(&Character::Die, this), 0.9f);

	for (UINT i = 0; i < totalEvents.size(); i++)
		eventIters[animState] = totalEvents[animState].begin();

	instancing->PlayClip(index, IDLE);
}

void Character::SetAnimState(AnimState state)
{
	if (animState == state)
		return;

	animState = state;
	instancing->PlayClip(index, animState);
	eventIters[animState] = totalEvents[animState].begin();
}

void Character::AttackEnd()
{
	SetAnimState(IDLE);
	Observer::Get()->ExcuteParamEvent("CharacterAttackEnd", this);
}

void Character::Damaged(int damage)
{
	__super::Damaged(damage);
	SetAnimState(status.curHp <= 0 ? DIE : HIT);
}

void Character::Die()
{ 
	SetActive(false); 
	if (weapon) {
		//무기랑 연결 해제
		weapon->SetActive(false);
		weapon->SetOwner(nullptr); 
		weapon = nullptr;
	}
}

void Character::SetEvent(int clip, Event event, float timeRatio)
{
	if (totalEvents[clip].count(timeRatio) > 0)
		return;

	totalEvents[clip][timeRatio] = event;
}

void Character::ExecuteEvent()
{
	int index = animState;

	if (totalEvents[index].empty()) return;						//현재 클립에 실행할 게 없다
	if (eventIters[index] == totalEvents[index].end()) return;	//남은 이벤트가 없다

	float ratio = motion->runningTime / motion->duration;

	if (eventIters[index]->first > ratio) return;

	eventIters[index]->second();
	eventIters[index]++;
}
