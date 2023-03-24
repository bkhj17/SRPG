#pragma once

class WaterScene : public Scene
{
public:
    WaterScene();
    ~WaterScene();

    // Scene을(를) 통해 상속됨
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    SkyBox* skybox;
    Model* forest;
    Human* human;

    //Reflection* reflection;
    //Refraction* refraction;

    Water* water;
};

