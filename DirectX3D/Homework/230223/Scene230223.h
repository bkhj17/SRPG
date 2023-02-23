#pragma once

class Scene230223 :
    public Scene
{
public:
    Scene230223();
    ~Scene230223();
    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;
private:
    class Sphere230223* sphere;
    class Cube230223* cube;
};

