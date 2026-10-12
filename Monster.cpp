#include "Monster.h"
#include <algorithm>
using namespace std;

Monster::Monster() : name(""), hp(0), hpMax(0), atk(0), def(0), category(Category::NORMAL), mercy(0), mercyGoal(100) {}

Monster::Monster(string n, int h, Category cat, int mercyGoal_, int atk_, int def_)
    : name(n), hp(h), hpMax(h), atk(atk_), def(def_), category(cat), mercy(0), mercyGoal(mercyGoal_) {}

string Monster::getName() { return name; }
int Monster::getHP() { return hp; }
int Monster::getHPMax() { return hpMax; }
int Monster::getHp() {
    return hp;
}

int Monster::getAtk() {
    return atk;
}

int Monster::getDef() {
    return def;
}

bool Monster::isAlive() { return hp > 0; }
void Monster::takeDamage(int d) {
    hp -= d;
    if (hp < 0) hp = 0;
}

void Monster::modifyMercy(int delta) {
    mercy += delta;
    if (mercy < 0) mercy = 0;
    if (mercy > mercyGoal) mercy = mercyGoal;
}

bool Monster::canBeSpared() { return mercy >= mercyGoal; }
int Monster::getMercy() { return mercy; }
int Monster::getMercyGoal() { return mercyGoal; }

void Monster::addActionId(string id) {
    if (!id.empty()) actionIds.push_back(id);
}

vector<string>& Monster::getActionIds() { return actionIds; }

Category Monster::getCategory() { return category; }
