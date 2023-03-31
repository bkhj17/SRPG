#pragma once

class PlayerController :
    public Controller
{
public:
    PlayerController();
    ~PlayerController();
    
    void Control() override;

};

