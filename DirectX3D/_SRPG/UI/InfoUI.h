#pragma once
#include "UIWindow.h"
class InfoUI : public UIWindow
{
	//캐릭터 정보 간략하게 출력
public:
	InfoUI(Vector3 pos);
	~InfoUI();

	void Render() override;
private:

};

