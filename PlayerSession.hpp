
typedef struct {
    int cur;
    int max;
} Stat;

// A class meant to store all information about a player's session for easy save/load
class PlayerSession
{
    private:
    int floor; // dungeon floor, increases as you descend
    int score;
    int level; // player level
    Stat xp;
    Stat hp, atk, def, spd;
    ItemId invExplore[3]; // exploration inventory
    ItemId invCombat[3]; // combat inventory
    //
    public:
    static const int BASE_HP = 10;
    static const int BASE_ATK = 1;
    static const int BASE_DEF = 0;
    static const int BASE_SPD = 3;

    PlayerSession()
    {
        floor = 1;
        score = 0;
        level = 1;
        xp.cur = 0; xp.max = 2; /// TODO instead calculate xp needed for levelling up
        hp.cur = BASE_HP; hp.max = BASE_HP;
        atk.cur = BASE_ATK; atk.max = BASE_ATK;
        def.cur = BASE_DEF; def.max = BASE_DEF;
        spd.cur = BASE_SPD; spd.max = BASE_SPD;
    }
};
