#pragma once

class Character;
class CharacterManager :
    public Singleton< CharacterManager>
{
private:
    friend class Singleton;
    CharacterManager();
    ~CharacterManager();
public:
    void Update();
    void Render();

    void CharacterHold(Character* character) { holded = character; }
    Character* HoldedCharacter() { return holded; }
private:
    Character* holded = nullptr;    //

    vector<Character*> characterPool;
};
