#include "framework.h"
#include "Robot.h"

Robot::Robot(Transform* transform, ModelAnimatorInstancing* instancing, UINT index)
	: transform(transform), instancing(instancing), index(index)
{
	root = new Transform;

	collider = new CapsuleCollider(30.0f, 100.0f);
	collider->SetParent(root);
	collider->Rot().z = XM_PIDIV2 - 0.2f;
	collider->Pos() = { -15.0f, 10.0f, 0.0f };
	collider->SetActive(false);

	motion = instancing->GetMotion(index);
	totalEvents.resize(instancing->GetClipSize());
	eventIters.resize(instancing->GetClipSize());

	SetEvent(STAND_UP, bind(&Robot::EndStandUp, this), 0.7f);
	SetEvent(HIT, bind(&Robot::SetState, this, WALK), 0.9f);
	SetEvent(DIE, bind(&Robot::EndDying, this), 0.9f);

	for (UINT i = 0; i < totalEvents.size(); i++)
		eventIters[curState] = totalEvents[curState].begin();

	hpBar = new ProgressBar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_bar_BG.png");
}

Robot::~Robot()
{
	delete collider;
	delete root;
	delete hpBar;
}

void Robot::Update()
{
	if (!transform->Active())
		return;

	velocity = target->GlobalPos() - transform->Pos();
	ExecuteEvent();
	Move();
	UpdateHPBar();

	root->SetWorld(instancing->GetTransformByNode(index, 3));
	collider->UpdateWorld();
}

void Robot::Render()
{
	if (!transform->Active())
		return;

	collider->Render();
}

void Robot::PostRender()
{
	if (!transform->Active())
		return;

	if (!CAM->ContainPoint(barPos)) return;

	hpBar->Render();
}

void Robot::SetEvent(int clip, Event event, float timeRatio)
{
	if (totalEvents[clip].count(timeRatio) > 0)
		return;

	totalEvents[clip][timeRatio] = event;
}

void Robot::ExecuteEvent()
{
	int index = curState;

	if (totalEvents[index].empty()) return;						//현재 클립에 실행할 게 없다
	if (eventIters[index] == totalEvents[index].end()) return;	//남은 이벤트가 없다

	float ratio = motion->runningTime / motion->duration;

	if (eventIters[index]->first > ratio) return;

	eventIters[index]->second();
	eventIters[index]++;
}

void Robot::EndStandUp()
{
	collider->SetActive(true);
	SetState(WALK);
}

void Robot::EndHit()
{
	SetState(WALK);
}

void Robot::EndDying()
{
	transform->SetActive(false);
	collider->SetActive(false);
}

void Robot::Hit()
{
	curHp -= 10.0f;
	hpBar->SetAmount(curHp / maxHp);

	if (curHp <= 0.0f) {
		SetState(DIE);
		return;
	}

	Audio::Get()->Play("hit", transform->Pos());

	curState = HIT;
	instancing->PlayClip(index, curState);
	eventIters[curState] = totalEvents[curState].begin();
}

void Robot::Spawn(Vector3 pos)
{
	transform->SetActive(true);
	collider->SetActive(true);
	SetState(WALK);

	curHp = maxHp;
	hpBar->SetAmount(curHp / maxHp);
	transform->Pos() = pos;

	Audio::Get()->Play("move", transform->Pos());
}

void Robot::SetState(State state)
{
	if (curState == state) return;

	curState = state;
	instancing->PlayClip(index, curState);
	eventIters[curState] = totalEvents[curState].begin();
}

void Robot::Move()
{
	if (curState != WALK && curState != RUN)
		return;

	if (velocity.Length() < 10.0f) {
		SetState(STAND_UP);
		return;
	}

	float speed = 50.0f;
	if (velocity.Length() < 500.0f)
	{
		speed = 100.0f;
		SetState(RUN);
	}
	else {
		speed = 50.0f;
		SetState(WALK);
	}

	transform->Pos() += velocity.GetNormalized() * speed * DELTA;
	transform->Rot().y = atan2f(velocity.x, velocity.z) + XM_PI;
}

void Robot::UpdateHPBar()
{
	barPos = transform->Pos() + Vector3(0, 180.0f, 0);
	hpBar->Pos() = CAM->WorldToScreen(barPos);

	float scale = 10.0f / sqrtf(velocity.Length());
	scale = Clamp(0.001f, 1.0f, scale);
	hpBar->Scale() = { scale, scale, scale };
	hpBar->UpdateWorld();
}
