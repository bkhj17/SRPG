#pragma once

class InfoUI : public UIWindow
{
	//캐릭터 정보 간략하게 출력
public:
	InfoUI(Vector3 pos);
	~InfoUI() = default;

	void Render() override;

	void Close() override {}	//상시 띄울 창

	void SetObject(SRPGObject* object);
private:
	SRPGObject* object = nullptr;

	map<int, wstring> teamTexts;
};

