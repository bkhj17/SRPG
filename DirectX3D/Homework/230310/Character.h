#pragma once

class Character : public Transform
{
public:
	Character();
	~Character();

	void Update();
	void Render();

private:
	Cube* cube;	//위치 테스트 용. 추후 모델 애니메이터로 변경
};

