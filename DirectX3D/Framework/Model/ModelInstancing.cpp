#include "framework.h"
#include "ModelInstancing.h"

ModelInstancing::ModelInstancing(string name)
    : Model(name)
{
    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
    SetShader(L"Model/ModelInstancing.hlsl");
}

ModelInstancing::~ModelInstancing()
{
    delete instanceBuffer;

    for (auto transform : transforms)
        delete transform;
}

void ModelInstancing::Update()
{
    drawCount = 0;
    for (UINT i = 0; i < transforms.size(); i++) {
        if (!transforms[i]->Active())
            continue;
        
        transforms[i]->UpdateWorld();
        if (CAM->ContainPoint(transforms[i]->GlobalPos())) {
            instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
            instanceDatas[drawCount].index = i;
            drawCount++;
        }
    }

    instanceBuffer->Update(instanceDatas, drawCount);
}

void ModelInstancing::Render()
{
    instanceBuffer->Set(1);

    for (auto mesh : meshes)
        mesh->RenderInstanced(drawCount);
}

void ModelInstancing::GUIRender()
{
    ImGui::Text("DrawCount : %d", drawCount);
    for (Transform* transform : transforms)
        transform->GUIRender();
}

Transform* ModelInstancing::Add()
{
    Transform* transform = new Transform;
    transform->SetTag(name + "_" + to_string(transforms.size()));
    transforms.push_back(transform);

    return transform;
}
