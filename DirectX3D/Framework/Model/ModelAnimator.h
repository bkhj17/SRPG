#pragma once
class ModelAnimator : public Model
{
protected:
    class FrameBuffer : public ConstBuffer
    {
    private:
        struct Data {
            int clip = 0;
            UINT curFrame = 0;

            float padding[2];
        };
    public:
        FrameBuffer() : ConstBuffer(&data, sizeof(Data))
        {
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

protected:
	void CreateTexture();
	void CreateClipTransform(UINT index);

	void UpdateFrame();

protected:
	bool isPlay;
	vector<ModelClip*> clips;

    FrameBuffer* frameBuffer;
    //¹è¿­
    ClipTransform* clipTransforms;
    ClipTransform* nodeTransforms;

    ID3D11Texture2D* texture = nullptr;
    ID3D11ShaderResourceView* srv;
};