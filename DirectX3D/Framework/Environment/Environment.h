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

    Camera* GetMainCamera() { return mainCamera; }

private:
    void CreateProjection();
    void CreateState();

private:
    MatrixBuffer* projectionBuffer;

    SamplerState* samplerState;
    /*
    ID3D11BlendState* alphaBlendState;
    ID3D11BlendState* additiveBlendState;
    */
    RasterizerState* raterizerState[2];

    Camera* mainCamera;

    D3D11_VIEWPORT viewport;

    bool isWireMode = false;
};