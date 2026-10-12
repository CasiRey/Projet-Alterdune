#pragma once
#include <string>
#include <vector>
#include "Item.h"

using namespace std;

class Player {
private:
    string name;
    int hp;
    int hpMax;
    vector<Item> inventory;

public:
    Player(string name, int hpMax);

    void takeDamage(int dmg);
    //void heal(int value);
    void addItem(Item& it);
    void showStats();
    string getName();

    int getHP();
    int getHPMax();
    bool isAlive();
    void showInventory();
    bool useItem(size_t index);
};
