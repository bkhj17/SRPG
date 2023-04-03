#pragma once

class ActionSelectUI : public SelectUI
{
public:
    ActionSelectUI(Vector3 pos = Vector3());

    void Close() override;
private:
    void ActiveFunc() override;
    void CancelFunc() override;
private:
};

