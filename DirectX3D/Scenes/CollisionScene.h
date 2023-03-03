#pragma once
#include "Scene.h"
class CollisionScene :
    public Scene
{
public:
    CollisionScene();
    ~CollisionScene();
    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    vector<Collider*> colliders;
};

