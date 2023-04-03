#pragma once

class PlayerController :
    public Controller
{
public:
    PlayerController() = default;
    ~PlayerController() = default;
    
    void Control(void *pack = nullptr) override;
};

