#pragma once
#include "MapSelectUI.h"
class AttackSelectUI :
    public MapSelectUI
{
public:
    AttackSelectUI();
    ~AttackSelectUI() = default;

    virtual void Init() override;
    virtual void Close() override;
};

