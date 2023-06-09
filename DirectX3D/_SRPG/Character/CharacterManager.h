#pragma once

class Character;

class CharacterManager :
    public Singleton< CharacterManager>
{
private:
    friend class Singleton;
    CharacterManager();
    ~CharacterManager();

    UINT MAX_POOL = 10;

    struct Attack {
        int damage;

        SRPGObject* attacker;
        SRPGObject* hit;
    };
public:
    void Update();
    void Render(bool shadow = false);
    void PostRender();

    void CharacterHold(Character* character, int w, int h) { holded = character; holdedW = w; holdedH = h; }
    Character* HoldedCharacter() { return holded; }
    void CharacterUnhold();

    bool IsActing();

    //출현
    SRPGObject* Spawn(int teamNum);
    SRPGObject* Spawn(class GridedTerrain* terrain, int teamNum, int w, int h);
    SRPGObject* Spawn(string name, int teamNum, class GridedTerrain* terrain, int w, int h);

    bool IsBattle() { return isBattle; }
    void BattleStart(SRPGObject* offense, SRPGObject* defense);

    void TurnStart();
        
    //이동 취소
    void CancelMove();

    void SetTerrain(class GridedTerrain* terrain) { }

    Character* GetActableCharacter(Character::Team team);

    int NumActiveCharactersByTeam(Character::Team team);

    void SetCharacterShader(wstring shader);
    void SetObjectShader(wstring shader);
private:
    void BattleUpdate();
    void BattleEnd();

    void AttackHit(void* ptr);
    void AttackEnd(void* ptr);

    int CalcDamage(Character* attacker, SRPGObject* defender);
private:
    Character* holded = nullptr;    
    int holdedW = -1, holdedH = -1;
   
    //전투 중인 캐릭터들 : 배틀은 따로 뺄 수도 있을 것 같다만
    bool isBattle = false; 
    SRPGObject* curOffense = nullptr;
    SRPGObject* curDefense = nullptr;
    //공방 순서
    queue<Attack> attacks;

    map<int, vector<SRPGObject*>> objectPool;

    map<int, ModelAnimatorInstancing*> instances;
};
