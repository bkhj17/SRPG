#include "framework.h"
#include "Character.h"
#include "../UI/SRPGUIManager.h"

Character::Character()
{
	body = new ModelAnimator("Soldier");
	
	body->ReadClip("SwordIdle");		//IDLE
	body->ReadClip("run");				//RUN
	body->ReadClip("SwordAttack");		//ATTACK
	body->ReadClip("Hit");
	body->ReadClip("Death");
	
	body->GetClip(ATTACK)->SetEvent(bind(&Character::AttackEnd, this), 0.8f);
	body->GetClip(ATTACK)->SetEvent(bind(&Character::AttackHit, this), 0.3f);
	body->GetClip(HIT)->SetEvent(bind(&Character::SetAnimState, this, IDLE), 0.9f);
	body->GetClip(DIE)->SetEvent(bind(&Character::Die, this), 0.9f);

	body->PlayClip(0);
	body->SetParent(this);

	body->Scale() *= 0.1f;
	body->Pos().y = body->Scale().y + 0.8f;

	body->SetShader(L"SRPG/Character.hlsl");

	actCylinder = new Cylinder(10.0f, 1.0f);
	actCylinder->Pos().y += 2.0f;
	actCylinder->SetParent(this);

	valueBuffer = new IntValueBuffer;
}

Character::~Character()
{
	delete body;
	delete actCylinder;
	delete valueBuffer;

	if (weapon) weapon->SetOwner(nullptr);
	weapon = nullptr;
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

	body->Update();
	actCylinder->UpdateWorld();

	if(weapon)
		weapon->Update();
}

void Character::Render()
{
	if (!Active())
		return;


	valueBuffer->Get()[0] = (int)acted;
	valueBuffer->SetPS(8);
	body->Render();

	if (status.teamNum == TurnManager::Get()->GetCurPlayer() && !IsActing() && !acted)
		actCylinder->Render();
}

bool Character::IsActing()
{
	if (!Active())
		return false;

	bool acting = false;
	acting |= IsMoving();
	acting |= (animState >= ATTACK);
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

void Character::SetWeapon(Weapon* weapon)
{
	if (this->weapon) {
		this->weapon->SetActive(false);
		this->weapon->SetOwner(nullptr);
	}

	this->weapon = weapon;
	this->weapon->SetOwner(body, 37);
	this->weapon->SetActive(true);
}

int Character::CalcAttack()
{
	int result = status.attack;
	if (weapon)
		result += weapon->GetPower();
	
	return result;
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
	body->PlayClip(state);
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
