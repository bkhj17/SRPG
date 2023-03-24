#pragma once
class Reflection
{
public:
	Reflection(Transform* target);
	~Reflection();

	void Update();

	void SetReflection();	//PreRender
	void SetRender();		//Render
	void PostRender();

private:
	Transform* target;			//반사할 객체
	RenderTarget* renderTarget;	//비칠 화면
	DepthStencil* depthStencil;	//화면 깊이

	Camera* camera;				//반사 찍을 카메라

	class Quad* quad;			
};