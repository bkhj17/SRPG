#include "framework.h"
#include "ModelAnimatorInstancing.h"

ModelAnimatorInstancing::ModelAnimatorInstancing(string name)
    : ModelAnimator(name)
{
    SetShader(L"Model/ModelAnimationInstance.hlsl");

    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
    frameInstancingBuffer =  new FrameInstancingBuffer();
}

ModelAnimatorInstancing::~ModelAnimatorInstancing()
{
    delete instanceBuffer;
    delete frameInstancingBuffer;
    for (auto transform : transforms)
        delete transform;
}

void ModelAnimatorInstancing::Update()
{
    UpdateTransforms();
}

void ModelAnimatorInstancing::Render()
{
    if (texture == nullptr)
        CreateTexture();

    instanceBuffer->Set(1);
    frameInstancingBuffer->SetVS(4);
    DC->VSSetShaderResources(0, 1, &srv);

    for (auto mesh : meshes)
        mesh->RenderInstanced(drawCount);
}

void ModelAnimatorInstancing::GUIRender()
{
    ImGui::Text("DrawCount : %d", drawCount);
    for (Transform* transform : transforms)
        transform->GUIRender();
}

Transform* ModelAnimatorInstancing::Add()
{
    Transform* transform = new Transform;
    transform->SetTag(name + "_" + to_string(transforms.size()));
    transforms.push_back(transform);

    return transform;
}

void ModelAnimatorInstancing::PlayClip(UINT instanceID, int clip, float scale, float takeTime)
{
    frameInstancingBuffer->Get().motions[instanceID].next.clip = clip;
    frameInstancingBuffer->Get().motions[instanceID].next.scale = scale;
    frameInstancingBuffer->Get().motions[instanceID].takeTime = takeTime;
    frameInstancingBuffer->Get().motions[instanceID].runningTime = 0.0f;
}

void ModelAnimatorInstancing::UpdateFrame(UINT instanceID, Motion& motion)
{
	{
		ModelClip* clip = clips[motion.cur.clip];

		motion.runningTime += motion.cur.scale * DELTA;
		motion.cur.time += clip->tickPerSecond * DELTA * motion.cur.scale;
		if (motion.cur.time >= 1.0f) {
			motion.cur.curFrame = (motion.cur.curFrame + 1) % (clip->frameCount - 1);
			motion.cur.time -= 1.0f;
		}

		clip->Execute();
	}
	{
		if (motion.next.clip < 0)
			return;

		ModelClip* clip = clips[motion.next.clip];

		motion.tweenTime += DELTA / motion.takeTime;
		if (motion.tweenTime >= 1.0f) {
			motion.cur = motion.next;
			motion.tweenTime = 0.0f;

			motion.next.clip = -1;
			motion.next.curFrame = 0;
			motion.next.time = 0.0f;
			return;
		}

		motion.next.time += clip->tickPerSecond * DELTA * motion.next.scale;
		if (motion.next.time >= 1.0f) {
			motion.next.curFrame = (motion.next.curFrame + 1) % (clip->frameCount - 1);
			motion.next.time -= 1.0f;
		}
	}
}

void ModelAnimatorInstancing::UpdateTransforms()
{
    drawCount = 0;
    for (UINT i = 0; i < transforms.size(); i++) {
        if (transforms[i]->Active()) {
            if (!CAM->ContainPoint(transforms[i]->GlobalPos()))
                continue;

            UpdateFrame(i, frameInstancingBuffer->Get().motions[i]);

            transforms[i]->UpdateWorld();
            instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
            instanceDatas[drawCount].index = i;
            drawCount++;
        }
    }

    instanceBuffer->Update(instanceDatas, drawCount);
}
