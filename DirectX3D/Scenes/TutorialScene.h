#pragma once
#include "Scene.h"
class TutorialScene :
    public Scene
{
public:
    TutorialScene();
    ~TutorialScene();
    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    VertexShader* vertexShader;
    PixelShader* pixelShader;
    /*
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;

    MatrixBuffer* worldBuffer;

    vector<VertexColor> vertices;
    vector<UINT> indices;
    */
    class Cube230220* cube;
};

