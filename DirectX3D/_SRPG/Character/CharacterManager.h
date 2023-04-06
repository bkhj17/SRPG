#pragma once

class Character;

class CharacterManager :
    public Singleton< CharacterManager>
{
private:
    friend class Singleton;
    CharacterManager();
    ~CharacterManager();

    UINT MAX_POOL = 20;

    struct Attack {
        int damage;

        Character* attacker;
        Character* hit;
    };

public:
    void Update();
    void Render();
    void PostRender();

    //void CharacterHold(Character* character) { holded = character; }
    void CharacterHold(Character* character, int w, int h) { holded = character; holdedW = w; holdedH = h; }
    Character* HoldedCharacter() { return holded; }
    void CharacterUnhold();

    bool IsActing();

    //출현
    Character* Spawn(int teamNum);
    Character* Spawn(class GridedTerrain* terrain, int teamNum, int w, int h);
    Character* Spawn(string name, int teamNum, class GridedTerrain* terrain, int w, int h);

    bool IsBattle() { return isBattle; }
    void BattleStart(Character* offense, Character* defense);

    void TurnStart();
        
    //이동 취소
    void CancelMove();

    void SetTerrain(class GridedTerrain* terrain) { }

    Character* GetActableCharacter(Character::Team team);

    int NumActiveCharactersByTeam(Character::Team team);
private:
    void BattleUpdate();
    void BattleEnd();

    void AttackHit(void* ptr);
    void AttackEnd(void* ptr);

    int CalcDamage(Character* attacker, Character* defender);
private:
    Character* holded = nullptr;    
    int holdedW = -1, holdedH = -1;
   
    //전투 중인 캐릭터들 : 배틀은 따로 뺄 수도 있을 것 같다만
    bool isBattle = false; 
    Character* curOffense = nullptr;
    Character* curDefense = nullptr;
    //공방 순서
    queue<Attack> attacks;

    map<int, vector<Character*>> characterPool;
};
