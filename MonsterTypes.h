#pragma once
#include "Monster.h"


class Normal : public Monster {
public:
    Normal(string n, int hp, int atk, int def,
           int mercyGoal, vector<string> acts)
        : Monster(n, hp, Category::NORMAL, mercyGoal, atk, def)
    {
        for (auto& a : acts) addActionId(a);
    }

    string getLabel() const override {
        return "[NORMAL] ";
    }
};


class Miniboss : public Monster {
public:
    Miniboss(string n, int hp, int atk, int def,
             int mercyGoal, vector<string> acts)
        : Monster(n, hp, Category::MINIBOSS, mercyGoal, atk, def)
    {
        for (auto& a : acts) addActionId(a);
    }

    string getLabel() const override {
        return "*** [MINIBOSS] ";
    }
};


class Boss : public Monster {
public:
    Boss(string n, int hp, int atk, int def,
         int mercyGoal, vector<string> acts)
        : Monster(n, hp, Category::BOSS, mercyGoal, atk, def)
    {
        for (auto& a : acts) addActionId(a);
    }

    string getLabel() const override {
        return "!!! [BOSS] ";
    }
};
