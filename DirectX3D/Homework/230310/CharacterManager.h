#pragma once

class Character;
class CharacterManager :
    public Singleton< CharacterManager>
{
private:
    friend class Singleton;
    CharacterManager();
    ~CharacterManager();

    UINT MAX_POOL = 30;

    struct Attack {
        int damage;

        Character* attacker;
        Character* hit;
    };

public:
    void Update();
    void Render();

    void CharacterHold(Character* character) { holded = character; }
    Character* HoldedCharacter() { return holded; }
    void CharacterUnhold();


    bool IsActing();

    //출현
    //출현 좌표 설정 : 1. 안에서 2. 밖에서
    Character* Spawn();

    bool IsBattle() { return isBattle; }
    void BattleStart(Character* offense, Character* defense);

private:
    void BattleUpdate();
    void BattleEnd();

    void AttackHit(void* ptr);
    void AttackEnd(void* ptr);
private:
    Character* holded = nullptr;    
   
    //전투 중인 캐릭터들 : 배틀은 따로 뺄 수도 있을 것 같다만
    bool isBattle = false; 
    Character* curOffense = nullptr;
    Character* curDefense = nullptr;
    //공방 순서
    queue<Attack> attacks;

    vector<Character*> characterPool;
};
