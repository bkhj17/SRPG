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
    /*
    void SetAlphaBlend();
    void SetAdditiveBlend();
    */
    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
    void SetProjection();
    void SetOrthoProjection();
    
    Camera* GetMainCamera() { return mainCamera; }

    Matrix GetProjection() { return projection; }
private:
    void CreateProjection();
    void CreateState();

    void CreateBlendState();
private:
    MatrixBuffer* orthoBuffer;
    MatrixBuffer* projectionBuffer;
    Camera* mainCamera;
    LightBuffer* lightBuffer;

    SamplerState* samplerState;
    
    ID3D11BlendState* alphaBlendState;
    ID3D11BlendState* additiveBlendState;
    
    RasterizerState* raterizerState[2];


    D3D11_VIEWPORT viewport;

    bool isWireMode = false;

    Matrix projection;
};