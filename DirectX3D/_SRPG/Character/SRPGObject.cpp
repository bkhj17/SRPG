#include "framework.h"

SRPGObject::SRPGObject()
{
	hpBar = new ProgressBar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_bar_BG.png");
}

SRPGObject::~SRPGObject()
{
	delete hpBar;
}

void SRPGObject::Init()
{
	SetActive(true);
	status.curHp = status.maxHp;
}

void SRPGObject::Update()
{
	if (!Active())
		return;
	Move();

	UpdateWorld();
	UpdateHPBar();
}

void SRPGObject::PostRender()
{
	if (!Active())
		return;

	if (!CAM->ContainSphere(GlobalPos(), 10.0f))
		return;

	hpBar->Render();
}

void SRPGObject::TurnStart()
{
	acted = moved = false;
	originPos = Pos();
}

void SRPGObject::SetDir(Vector3 dir)
{
	this->dir = dir.GetNormalized();
	Rot().y = atan2f(dir.x, dir.z) + XM_PI;
}

void SRPGObject::SetMovePath(vector<Vector3>& path)
{
	curPos = originPos;
	lerpValue = 0.0f;
	movePath.resize(path.size());
	copy(path.begin(), path.end(), movePath.begin());
}

void SRPGObject::CancelMove()
{
	if (acted || !moved)
		return;

	moved = false;
	Pos() = originPos;
	UpdateWorld();

	Observer::Get()->ExcuteParamEvent("FocusPos", &originPos);
}

bool SRPGObject::IsMoving()
{
	return !movePath.empty();
}

void SRPGObject::Move()
{
	if (movePath.empty())
		return;

	lerpValue += DELTA * moveSpeed;
	Pos() = Lerp(curPos, movePath.back(), lerpValue);

	Vector3 velocity = movePath.back() - Pos();

	if (velocity.Length() <= 0.1f) {
		curPos = Pos() = movePath.back();
		movePath.pop_back();
		lerpValue = 0.0f;

		//�̵� ����
		if (movePath.empty()) {
			moved = true;
			Observer::Get()->ExcuteParamEvent("CharacterMoveEnd", this);
		}
	}
	else {
		SetDir(velocity);
	}
}

void SRPGObject::AttackHit()
{
	Observer::Get()->ExcuteParamEvent("CharacterAttackHit", this);
}

void SRPGObject::Damaged(int damage)
{
	SRPGUIManager::Get()->SpawnDamage(Pos(), damage);
	status.curHp -= damage;

}

void SRPGObject::Die()
{
	SetActive(false);
}

void SRPGObject::UpdateHPBar()
{
	hpBar->SetAmount((float)status.curHp / status.maxHp);
	hpBar->Pos() = Environment::Get()->GetCurCamera()->WorldToScreen(GlobalPos());

	float dist = (GlobalPos() - Environment::Get()->GetCurCamera()->GlobalPos()).Length();
	hpBar->Scale() = { 70.0f / dist, 125.0f / dist, 1.0f };

	hpBar->UpdateWorld();
}
