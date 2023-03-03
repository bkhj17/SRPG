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
	CAM->SetTarget(fps, Vector3(0, 1, 0));
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
	Ray ray = CAM->ScreenPointToRay({ CENTER_X, CENTER_Y, 0.0f });
	check = nullptr;

	Contact contect;
	contect.distance = FLT_MAX;
	for (auto cube : cubes) {
		if (!cube->Active())
			continue;

		Contact c;
		if (cube->GetCollider()->IsRayCollision(ray, &c)) {
			if (!check || contect.distance > c.distance) {
				check = cube;	//탐지된 표적
				contect = c;
			}
		}
	}

	if (KEY_DOWN('Z')) {
		if (check) {	//탐지된 게 있다
			Ray cRay;	//위에 뭐가 있는지 검사
			cRay.pos = check->GlobalPos();
			cRay.dir = check->Up();

			bool isOn = false;
			for (auto cube : cubes) {
				if (!cube->Active() || cube == check)
					continue;

				//위에 뭐가 있는지 없는지 체크
				Contact cContact;
				if (cube->GetCollider()->IsRayCollision(cRay, &cContact)) {
					if (cContact.distance < (check->GlobalScale().y * 0.5f + 1.0f)) {
						isOn = true;
						break;
					}
				}
			}

			if (!isOn) { //위에 아무것도 없다면 cube 출현
				for (auto cube : cubes) {
					if (cube->Active())
						continue;

					cube->SetActive(true);
					cube->Pos() = check->GlobalPos() + check->Up();
					cube->Rot() = check->Rot();
					break;
				}
			}
		}
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
	for (auto cube : cubes) {
		fps->StandOn(cube->GetCollider());
	}
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
	


}

void Scene230303::GUIRender()
{
	if (ImGui::TreeNode("Cubes")) {
		for (auto cube : cubes)
			cube->RenderUI();

		ImGui::TreePop();
	}
}
