#include "framework.h"
#include "Scene230303.h"
#include "Cube230303.h"
#include "FPS230303.h"

Scene230303::Scene230303()
{
	int row = 10;
	int col = 10;
	const int MAX_CUBE = 300;
	cubes.resize(MAX_CUBE);

	int index = 0;
	for (auto& cube : cubes) {
		cube = new Cube230303;
		cube->SetActive(false);
		cube->SetTag(to_string(index++));
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cubes[(size_t)i * col + j]->SetActive(true);
			cubes[(size_t)i * col + j]->Pos() = { (float)i , -1.0f, (float)j};
		}
	}

	fps = new FPS230303;
	CAM->SetTarget(fps);

	Observer::Get()->AddEvent("CheckCollide", bind(&Scene230303::CheckCollide, this));
	Observer::Get()->AddEvent("CheckStand", bind(&Scene230303::CheckStand, this));

	quad = new Quad(L"Textures/UI/CrossHair.png");
	quad->Pos() = { CENTER_X, CENTER_Y, 0 };
	quad->Scale() *= 0.5f;
	quad->UpdateWorld();
}

Scene230303::~Scene230303()
{
	for (auto cube : cubes)
		delete cube;

	delete fps;
}

void Scene230303::Update()
{
	//표적이 탐지되었는지 확인
	CheckInRange();

	if (KEY_DOWN('Z')) {
		SetupCube();
	}

	if (KEY_DOWN('X')) {
		if (check)
			check->SetActive(false);
	}

	for (auto cube : cubes) {
		if (cube == check)
			cube->GetCollider()->SetColor(1, 0, 0);
		else
			cube->GetCollider()->SetColor(0, 1, 0);

		cube->UpdateWorld();
	}

	//바닥 콜리전 : 튐
	fps->Update();
}

void Scene230303::PreRender()
{
}

void Scene230303::Render()
{
	for (auto cube : cubes) 
		cube->Render();
	if (check)
		check->GetCollider()->Render();
}

void Scene230303::PostRender()
{
	quad->Render();

}

void Scene230303::GUIRender()
{
	if (ImGui::TreeNode("Cubes")) {
		for (auto cube : cubes)
			cube->RenderUI();

		ImGui::TreePop();
	}
}

void Scene230303::CheckInRange()
{
	rayCheck = CAM->ScreenPointToRay({ CENTER_X, CENTER_Y, 0.0f });
	check = nullptr;
	contectCheck.distance = FLT_MAX;
	for (auto cube : cubes) {
		if (!cube->Active())
			continue;

		Contact c;
		if (cube->GetCollider()->IsRayCollision(rayCheck, &c)) {
			if (!check || contectCheck.distance > c.distance) {
				check = cube;	//탐지된 표적
				contectCheck = c;
			}
		}
	}

}

void Scene230303::CheckCollide()
{
	for (auto cube : cubes) {
		if (fps->GetCollider()->IsBoxCollision(cube->GetCollider())) {
			Vector3 vec = fps->GlobalPos() - cube->GlobalPos();
			vec.Normalize();
			vec.y = 0.0f;

			if(vec.Length() > 0.7f)
				fps->Pos() += vec.GetNormalized() * DELTA * 3.0f;
			fps->UpdateWorld();
		}		
	}
}

void Scene230303::CheckStand()
{
	for (auto cube : cubes)
		fps->StandOn(cube->GetCollider());
}

void Scene230303::SetupCube()
{
	if (check) {	//탐지된 게 있다
		contectCheck.distance;

		Vector3 dir = contectCheck.hitPoint - check->GlobalPos();
		//셋 중 가장 큰 것만을 남긴다
		if (abs(dir.y) > abs(dir.x)) {
			if (abs(dir.y) > abs(dir.z))
				dir = { 0, (dir.y > 0) ? 1.0f : -1.0f, 0 };
			else
				dir = { 0, 0, (dir.z > 0) ? 1.0f : -1.0f };
		}
		else {
			if (abs(dir.x) > abs(dir.z))
				dir = { (dir.x > 0) ? 1.0f : -1.0f, 0, 0 };
			else
				dir = { 0, 0, (dir.z > 0) ? 1.0f : -1.0f };
		}

		Ray cRay;	//그 방향에 뭐가 있는지 검사
		cRay.pos = check->GlobalPos();
		cRay.dir = dir;

		bool isOn = false;
		for (auto cube : cubes) {
			if (!cube->Active() || cube == check)
				continue;

			//뭐가 있는지 없는지 체크
			Contact cContact;
			if (cube->GetCollider()->IsRayCollision(cRay, &cContact)) {
				if (cContact.distance < (check->GlobalScale().y * 0.5f + 1.0f)) {
					isOn = true;
					break;
				}
			}
		}

		if (!isOn) { //아무것도 없다면 cube 출현
			for (auto cube : cubes) {
				if (cube->Active())
					continue;

				cube->SetActive(true);
				cube->Pos() = check->GlobalPos() + dir;
				cube->Rot() = check->Rot();
				break;
			}
		}
	}
}
