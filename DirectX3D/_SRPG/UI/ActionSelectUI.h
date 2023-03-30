#pragma once
#include "UIWindow.h"
class ActionSelectUI : public UIWindow
{
public:
    ActionSelectUI(Vector3 pos = Vector3());

    void Update() override;
    void Render() override;

    void Close() override;
private:
    void ActiveFunc() override;

    float CursorX();
    float LineY(int lineNum);
private:
    vector<string> text = { "이동", "공격", "행동종료", "취소"};
};

