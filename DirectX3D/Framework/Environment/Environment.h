#pragma once

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public:
    void Update();
    void RenderUI();

    void Set();
    void PostSet();
    /*
    void SetAlphaBlend();
    void SetAdditiveBlend();
    */
    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
    void SetPerspective();
    void SetOrthographic();

    Camera* GetMainCamera() { return mainCamera; }

    Matrix GetProjection() { return perspective; }
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
    
    BlendState* blendState[2];
    
    RasterizerState* raterizerState[2];


    D3D11_VIEWPORT viewport;

    bool isWireMode = false;

    Matrix perspective;
    Matrix orthographic;
};