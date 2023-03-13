#pragma once
class ModelAnimator : public Model
{
protected:
    struct Frame {
        int clip;
        UINT curFrame;
        float time;
        float scale;
    };

    class FrameBuffer : public ConstBuffer
    {
    public:
        struct Data {
            float takeTime = 0.2f;      //클립 전환 시간
            float tweenTime = 0.0f;     
            float runningTime = 0.0f;   
            float padding;

            Frame cur, next;
        };
    public:
        FrameBuffer() : ConstBuffer(&data, sizeof(Data)) {
            data.next.clip = -1;
        }

        Data& Get() { return data; }
    private:
        Data data;
    };

public:
	ModelAnimator(string name);
	~ModelAnimator();

	void Update();
	void Render();
	void GUIRender();

	void ReadClip(string clipName, UINT clipNum = 0);
    void PlayClip(int clip, float scale = 1.0f, float takeTime = 0.2f);

    Matrix GetTransformByNode(int node);

    ModelClip* GetClip(UINT index) { return clips[index]; }
protected:
	void CreateTexture();
	void CreateClipTransform(UINT index);

	void UpdateFrame();

protected:
	bool isPlay = true;
	vector<ModelClip*> clips;

    FrameBuffer* frameBuffer;
    //배열
    ClipTransform* clipTransforms;
    ClipTransform* nodeTransforms;

    ID3D11Texture2D* texture = nullptr;
    ID3D11ShaderResourceView* srv;
};