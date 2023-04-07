#include "framework.h"
#include "Character.h"
#include "../UI/SRPGUIManager.h"

Character::Character(ModelAnimatorInstancing* instancing)
{
	SetInstanging(instancing);

	hpBar = new ProgressBar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_bar_BG.png");

	actCylinder = new Cylinder(10.0f, 1.0f);
	actCylinder->GetMaterial()->SetShader(L"SRPG/ActCylinder.hlsl");
	actCylinder->Pos().y += 2.0f;
	actCylinder->SetParent(this);

	valueBuffer = new IntValueBuffer;

	weaponOwner = new Transform;
}

Character::~Character()
{
	delete actCylinder;
	delete hpBar;
	delete valueBuffer;

	if (weapon) weapon->SetOwner(nullptr);
	weapon = nullptr;

	delete weaponOwner;
}

void Character::Init()
{
	SetActive(true);
	status.curHp = status.maxHp;
}

void Character::Update()
{
	if (!Active())
		return;

	Move();
	UpdateWorld();

	if (animState <= RUN)
		SetAnimState(IsMoving() ? RUN : IDLE);

	ExecuteEvent();

	actCylinder->UpdateWorld();
	UpdateHPBar();

	if (weapon) {
		weaponOwner->SetWorld(instancing->GetTransformByNode(index, weaponBoneNum));
		weapon->Update();
	}
}

void Character::Render()
{
	if (!Active())
		return;

	valueBuffer->Get()[0] = (int)acted;
	valueBuffer->SetPS(8);

	if (status.teamNum == TurnManager::Get()->GetCurPlayer() && !IsActing() && !acted)
		actCylinder->Render();
}

void Character::PostRender()
{
	if (!Active())
		return;

	hpBar->Render();
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

void Character::TurnStart()
{
	acted = moved = false;
	originPos = Pos();
}

void Character::SetMovePath(vector<Vector3>& path)
{
	lerpValue = 0.0f;
	movePath.resize(path.size());
	copy(path.begin(), path.end(), movePath.begin());
}

void Character::SetDir(Vector3 dir)
{
	this->dir = dir.GetNormalized();
	Rot().y = atan2f(dir.x, dir.z) + XM_PI;
}

void Character::CancelMove()
{
	if (acted || !moved)
		return;

	moved = false;
	Pos() = originPos;
	UpdateWorld();

	Observer::Get()->ExcuteParamEvent("FocusPos", &originPos);
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

void Character::SetInstanging(ModelAnimatorInstancing* instancing)
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

bool Character::IsMoving()
{
	return !movePath.empty(); 
}

void Character::Move()
{
	if (movePath.empty())
		return;

	lerpValue += DELTA * moveSpeed;
	Pos() = Lerp(Pos(), movePath.back(), lerpValue);

	Vector3 velocity = movePath.back() - Pos();

	if (velocity.Length() <= 0.1f) {
		Pos() = movePath.back();
		movePath.pop_back();
		lerpValue = 0.0f;

		//이동 종료
		if (movePath.empty()) {
			moved = true;
			Observer::Get()->ExcuteParamEvent("CharacterMoveEnd", this);
		}
	}
	else {
		SetDir(velocity);
	}
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

void Character::AttackHit()
{
	Observer::Get()->ExcuteParamEvent("CharacterAttackHit", this);
}

void Character::Damaged(int damage)
{
	status.curHp -= damage;

	SetAnimState(status.curHp <= 0 ? DIE : HIT);

	SRPGUIManager::Get()->SpawnDamage(Pos(), damage);
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

void Character::UpdateHPBar()
{
	hpBar->SetAmount((float)status.curHp / status.maxHp);
	hpBar->Pos() = Environment::Get()->GetCurCamera()->WorldToScreen(GlobalPos());

	float dist = (GlobalPos() - Environment::Get()->GetCurCamera()->GlobalPos()).Length();
	hpBar->Scale() = { 70.0f / dist, 125.0f / dist, 1.0f };
	
	hpBar->UpdateWorld();
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
