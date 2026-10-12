#pragma once
#include <string>
#include <vector>
using namespace std;

enum class Category { NORMAL, MINIBOSS, BOSS };

class Monster {
protected:  
    string name;
    int hp;
    int hpMax;
    int atk;
    int def;
    Category category;
    int mercy;
    int mercyGoal;
    vector<string> actionIds;

public:
    Monster();
    Monster(string n, int h, Category cat = Category::NORMAL,
            int mercyGoal_ = 100, int atk_ = 0, int def_ = 0);

    virtual ~Monster() = default; 

    
    virtual string getLabel() const = 0;

    string getName();
    int getHP();
    int getHPMax();
    int getHp();
    int getAtk();
    int getDef();
    bool isAlive();
    void takeDamage(int d);
    void modifyMercy(int delta);
    bool canBeSpared();
    int getMercy();
    int getMercyGoal();

    void addActionId(string id);
    vector<string>& getActionIds();

    Category getCategory();
};
