#include "framework.h"
#include "Character.h"
#include "FloatingDamage.h"
#include "SRPGUIManager.h"

Character::Character()
{
	body = new ModelAnimator("Naruto");
	
	body->ReadClip("idle");		//IDLE
	body->ReadClip("run");		//RUN
	body->ReadClip("Throw");	//ATTACK
	body->GetClip(ATTACK)->SetEvent(bind(&Character::AttackEnd, this), 0.8f);
	body->GetClip(ATTACK)->SetEvent(bind(&Character::AttackHit, this), 0.3f);
	
	body->PlayClip(0);
	body->SetParent(this);

	body->Scale() *= 0.05f;
	body->Pos().y = body->Scale().y;

	body->SetShader(L"SRPG/Character.hlsl");

	valueBuffer = new IntValueBuffer;
}

Character::~Character()
{
	delete body;

	delete valueBuffer;
}

void Character::Update()
{
	if (!Active())
		return;

	//Move
	if (IsMoving())
		Move();

	UpdateWorld();

	if (animState <= RUN)
		SetAnimState(IsMoving() ? RUN : IDLE);

	body->Update();
}

void Character::Render()
{
	if (!Active())
		return;

	valueBuffer->Get()[0] = (int)acted;
	valueBuffer->SetPS(8);
	body->Render();
}

bool Character::IsActing()
{
	bool acting = false;
	acting |= IsMoving();
	return acting;
}

void Character::TurnStart()
{
	acted = moved = false;
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

bool Character::IsMoving()
{
	return !movePath.empty(); 
}

void Character::Move()
{
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
