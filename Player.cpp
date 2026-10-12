#include "Player.h"
#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;

Player::Player(string n, int maxHp) : name(n), hp(maxHp), hpMax(maxHp){}

void Player::addItem(Item& it) {
    for (auto& i : inventory) {
        if (i.getName() == it.getName()) {
            i.setQuantity(i.getQuantity() + it.getQuantity());
            return;
        }
    }
    inventory.push_back(it);
}

bool Player::useItem(size_t index) {
    if (index >= inventory.size()) return false;
    Item& it = inventory[index];
    if (it.getType() == "HEAL") {
        hp = min(hpMax, hp + it.getValue());
    }
    int newQty = it.getQuantity() - 1;
    it.setQuantity(newQty);
    if (newQty <= 0) inventory.erase(inventory.begin() + index);
    return true;
}

void Player::showInventory() {
    cout << "\n--- INVENTAIRE ---\n";
    if (inventory.empty()) {
        cout << "(vide)\n";
        cout << flush;
        return;
    }
    for (size_t i = 0; i < inventory.size(); ++i) {
        Item& it = inventory[i];
        cout << i << ". " << it.getName()
            << " | type: " << it.getType()
            << " | value: " << it.getValue()
            << " | qty: " << it.getQuantity() << '\n';
    }
    cout << flush;
}

void Player::showStats() {
    cout << "\n--- STATS ---\n";
    cout << "Nom: " << name << '\n';
    cout << "HP: " << hp << " / " << hpMax << '\n';
    cout << "Objets: " << inventory.size() << '\n';
    cout << flush;
}

int Player::getHP() { return hp; }
int Player::getHPMax() { return hpMax; }
string Player::getName() {
    return name;
}


void Player::takeDamage(int d) {
    hp -= d;
    if (hp < 0) hp = 0;
}

bool Player::isAlive() { return hp > 0; }

