#include "framework.h"
#include "Instancing230313.h"
#include "Enemy230313.h"

Transform* Instancing230313::Add(Transform* transform)
{
	transforms.push_back(transform);
	return transform;
}

void Instancing230313::Update()
{
    for (UINT i = 0; i < transforms.size(); i++) {
        ((Enemy230313*)transforms[i])->Update();
    }

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
    for (UINT i = 0; i < transforms.size(); i++) {
        ((Enemy230313*)transforms[i])->PostRender();
    }
}

void Instancing230313::UpdateTransforms()
{
    drawCount = 0;
    for (UINT i = 0; i < transforms.size(); i++) {
        if (transforms[i]->Active()) {
            if (!CAM->ContainPoint(transforms[i]->GlobalPos()))
                continue;

            auto enemy = (Enemy230313*)transforms[i];

            UpdateFrame(i, frameInstancingBuffer->Get().motions[i]);

            transforms[i]->UpdateWorld();
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

            PlayClip(i, 0);
            auto enemy = (Enemy230313*)transforms[i];

            enemy->Spawn(pos);
            curTargetNum++;
            return true;
        }
    }
    return false;
}
