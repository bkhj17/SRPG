#include "framework.h"
#include "Character.h"

Character::Character()
{
	body = new ModelAnimator("Human");
	body->ReadClip("idle", 1);
	body->ReadClip("run", 1);
	body->PlayClip(0);
	body->SetParent(this);

	body->Scale() *= 3.0f;
	body->Pos().y = body->Scale().y;
}

Character::~Character()
{
	delete body;
}

void Character::Update()
{
	if (movePath.size() > 0) {
		lerpValue += DELTA * moveSpeed;
		Pos() = Lerp(Pos(), movePath.back(), lerpValue);

		Vector3 velocity = movePath.back() - Pos();
		
		if (velocity.Length() <= 0.1f) {
			Pos() = movePath.back();
			movePath.pop_back();
			lerpValue = 0.0f;

			if (movePath.empty())
				//이동 종료
				Observer::Get()->ExcuteParamEvent("CharacterMoveEnd", this);
		}
		else
			dir = velocity.GetNormalized();
	}

	UpdateWorld();

	Rot().y = atan2f(dir.x, dir.z) + XM_PI;

	SetAnimState(IsMoving() ? RUN : IDLE);
	body->Update();
}

void Character::Render()
{
	body->Render();
}

bool Character::IsActing()
{
	bool acting = false;
	acting |= IsMoving();
	return acting;
}

void Character::SetMovePath(vector<Vector3>& path)
{
	lerpValue = 0.0f;
	movePath.resize(path.size());
	copy(path.begin(), path.end(), movePath.begin());
}

bool Character::IsMoving() 
{
	return !movePath.empty(); 
}

void Character::SetAnimState(AnimState state)
{
	if (animState == state)
		return;

	animState = state;
	body->PlayClip(state);
}
