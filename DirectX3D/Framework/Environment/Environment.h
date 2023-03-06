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

    void CreateBlendState();
private:
    MatrixBuffer* orthoBuffer;
    MatrixBuffer* projectionBuffer;
    Camera* mainCamera;
    LightBuffer* lightBuffer;
    ViewBuffer* uiViewBuffer;

    SamplerState* samplerState;
    
    ID3D11BlendState* alphaBlendState;
    ID3D11BlendState* additiveBlendState;
    
    RasterizerState* raterizerState[2];


    D3D11_VIEWPORT viewport;

    bool isWireMode = false;

    Matrix perspective;
    Matrix orthographic;
};