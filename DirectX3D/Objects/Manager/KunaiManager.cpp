#include "framework.h"
#include "KunaiManager.h"

KunaiManager::KunaiManager()
{
	kunaiInstancing = new ModelInstancing("Kunai");
	kunaies.reserve(SIZE);
	for (UINT i = 0; i < SIZE; i++) {
		Transform* transform = kunaiInstancing->Add();
		transform->SetActive(false);
		Kunai* kunai = new Kunai(transform);
		kunaies.push_back(kunai);
	}
}

KunaiManager::~KunaiManager()
{
	delete kunaiInstancing;
	for (auto kunai : kunaies) {
		delete kunai;
	}
}

void KunaiManager::Update()
{
	kunaiInstancing->Update();
	for (auto kunai : kunaies)
		kunai->Update();
}

void KunaiManager::Render()
{
	kunaiInstancing->Render();
	for (auto kunai : kunaies)
		kunai->Render();
}

void KunaiManager::Throw(Vector3 pos, Vector3 dir)
{
	for (auto kunai : kunaies) {
		if (!kunai->GetTransform()->Active()) {
			kunai->Throw(pos, dir);
			return;
		}
	}

}
