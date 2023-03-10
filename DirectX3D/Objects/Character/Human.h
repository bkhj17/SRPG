#pragma once
class Human : public ModelAnimator
{
public:
	Human();
	~Human();

	void Update();
	void Render();
	void GUIRender();
private:
	Crowbar* crowbar;
	Transform* rightHand;
};

