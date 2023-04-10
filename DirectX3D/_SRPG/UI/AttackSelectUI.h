#pragma once

class AttackSelectUI :
    public MapSelectUI
{
public:
    AttackSelectUI();
    ~AttackSelectUI() = default;

    virtual void Init() override;
    virtual void Close() override;
};

