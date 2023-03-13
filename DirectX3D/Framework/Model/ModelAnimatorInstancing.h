#pragma once
#include "ModelAnimator.h"
class ModelAnimatorInstancing : public ModelAnimator
{
private:
    struct Motion {

        float takeTime = 0.2f;      //클립 전환 시간
        float tweenTime = 0.0f;
        float runningTime = 0.0f;
        float padding = 0.0f;

        Frame cur, next;

        Motion() {
            next.clip = -1;
        }
    };

    class FrameInstancingBuffer : public ConstBuffer
    {
    public:
        struct Data {
            Motion motions[MAX_INSTANCE];
        };
    public:
        FrameInstancingBuffer() : ConstBuffer(&data, sizeof(Data)) {}

        Data& Get() { return data; }
    private:
        Data data;
    };
public:
	ModelAnimatorInstancing(string name);
	~ModelAnimatorInstancing();

    virtual void Update();
    virtual void Render();
    void GUIRender();

    Transform* Add();

    void PlayClip(UINT instanceID, int clip, float scale = 1.0f, float takeTime = 0.1f);
protected:
    void UpdateFrame(UINT instanceID, Motion& motion);
    virtual void UpdateTransforms();
protected:
    vector<Transform*> transforms;
    InstanceData instanceDatas[MAX_INSTANCE];

    VertexBuffer* instanceBuffer;
    FrameInstancingBuffer* frameInstancingBuffer;

    UINT drawCount = 0;
};

