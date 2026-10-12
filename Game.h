#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Player.h"
#include "MonsterTypes.h"   
using namespace std;

class Game {
private:
    unique_ptr<Player> player;
    vector<unique_ptr<Monster>> monstersList; 
    int victoryCount;
    int sparedCount;
    int killedCount;

    struct Action {
        string id;
        string text;
        int mercyDelta;
        Action(string i, string t, int m) : id(i), text(t), mercyDelta(m) {}
    };

    vector<Action> actionList;
    void checkEndCondition();

    struct BestiaryEntry {
        string name;
        string label;   
        bool spared;
        BestiaryEntry(string n, string l, bool s) : name(n), label(l), spared(s) {}
    };

    vector<BestiaryEntry> bestiary;

public:
    Game();
    void start();
    void loadItems(string path = "items.csv");
    void loadMonsters(string path = "monsters.csv");
    void startCombat();
    void showPlayerStats();
    void showBestiary();
    Action* findActionById(const string& id);
};
