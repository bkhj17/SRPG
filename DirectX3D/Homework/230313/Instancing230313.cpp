#include "framework.h"
#include "Instancing230313.h"
#include "Enemy230313.h"

Instancing230313::Instancing230313(string name)
    : ModelAnimatorInstancing(name)
{
    ReadClip("Walking", 0);
    ReadClip("Hit", 0);
    ReadClip("Death", 0);


    for (int i = 0; i < 10; i++) {
        auto target = Add(new Enemy230313);
        target->SetActive(false);
        ((Enemy230313*)target)->GetIndex() = i;
    }


    //GetClip(1)->SetEvent(, 1.0f);
    //GetClip(2)->SetEvent(, 1.0f);
}

Transform* Instancing230313::Add(Transform* transform)
{
	transforms.push_back(transform);
	return transform;
}

void Instancing230313::Update()
{
    UpdateTransforms();
}

void Instancing230313::Render()
{
    for (UINT i = 0; i < transforms.size(); i++)
        ((Enemy230313*)transforms[i])->GetCollider()->Render();
    __super::Render();
}

void Instancing230313::PostRender()
{
    for (UINT i = 0; i < transforms.size(); i++)
        ((Enemy230313*)transforms[i])->PostRender();
}

void Instancing230313::UpdateTransforms()
{
    drawCount = 0;
    for (UINT i = 0; i < transforms.size(); i++) {
        if (transforms[i]->Active()) {
            if (!CAM->ContainPoint(transforms[i]->GlobalPos()))
                continue;

            UpdateFrame(i, frameInstancingBuffer->Get().motions[i]);

            auto enemy = (Enemy230313*)transforms[i];
            enemy->UpdateWorld();
            instanceDatas[drawCount].world = XMMatrixTranspose(enemy->GetModelWorld());
            instanceDatas[drawCount].index = i;
            drawCount++;
        }
    }

    instanceBuffer->Update(instanceDatas, drawCount);
}

bool Instancing230313::Spawn(Vector3 pos)
{
    if (curTargetNum < transforms.size()) {
        for (int i = 0; i < transforms.size(); i++) {
            if (transforms[i]->Active())
                continue;

            auto enemy = (Enemy230313*)transforms[i];
            PlayClip(i, 0);

            enemy->Spawn(pos);
            curTargetNum++;
            return true;
        }
    }
    return false;
}

void Instancing230313::SetEvent(UINT instanceID, int clip, Event event)
{
    
}

void Instancing230313::Hit(Collider* collider)
{
    for (int i = 0; i < transforms.size(); i++) {
        auto enemy = (Enemy230313*)transforms[i];

        if (collider->IsCollision(enemy->GetCollider())) {
            collider->SetActive(false);
            enemy->Hit();

            PlayClip(i, enemy->GetCurAction());
        }
    }
}
