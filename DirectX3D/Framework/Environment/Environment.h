#pragma once

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:
    void Update();
    void GUIRender();

    void Set();
    void PostSet();
    /*
    void SetAlphaBlend();
    void SetAdditiveBlend();
    */
    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
    void SetPerspective();
    void SetOrthographic();

    LightBuffer::Light* AddLight();

    Camera* GetMainCamera() { return mainCamera; }

    Matrix GetProjection() { return perspective; }

    LightBuffer::Light* GetLight(UINT index) { return &lightBuffer->Get().lights[index]; }
private:
    void CreateProjection();
    void CreateState();

private:
    MatrixBuffer* orthoBuffer;
    MatrixBuffer* projectionBuffer;
    Camera* mainCamera;
    LightBuffer* lightBuffer;
    ViewBuffer* uiViewBuffer;

    SamplerState* samplerState;
    RasterizerState* raterizerState[2];
    BlendState* blendState[2];
    DepthStencilState* depthStencilState[2];


    D3D11_VIEWPORT viewport;

    bool isWireMode = false;

    Matrix perspective;
    Matrix orthographic;
};