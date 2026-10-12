#include "Game.h"
#include "Item.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <limits>
using namespace std;

Game::Action* Game::findActionById(const string& id) {
    for (auto& a : actionList) {
        if (a.id == id) return &a;
    }
    return nullptr;
}

Game::Game() : player(nullptr), victoryCount(0), sparedCount(0), killedCount(0) {
    actionList.emplace_back("greet", "You greet them awkwardly. They seem touched.", 20);
    actionList.emplace_back("insult", "You shout an insult. They look offended.", -10);
    actionList.emplace_back("bounce", "You compliment their bouncy personality. They wobble.", 15);
    actionList.emplace_back("stomp", "You stomp dramatically. It's... impressive.", -20);
    actionList.emplace_back("taunt", "You tell a silly joke about their hat. They giggle.", 25);
    actionList.emplace_back("firebreath", "You compliment their technique. They beam with pride.", 10);
    actionList.emplace_back("roar", "You roar back with enthusiasm. Oddly bonding.", 5);
    actionList.emplace_back("default_act", "You try something unpredictable. It has an effect.", 10);
}

void Game::start() {
    string name;
    cout << "Enter your name: ";
    cin >> name;
    player = make_unique<Player>(name, 100);

    loadItems("items.csv");
    loadMonsters("monsters.csv");

    cout << "\n=== Welcome, " << player->getName() << " ===\n";
    cout << "HP: " << player->getHP() << "/" << player->getHPMax() << "\n";

    while (true) {
        cout << "\n=== MENU PRINCIPAL ===\n";
        cout << "1. Start Combat\n2. Stats\n3. Inventory\n4. Bestiaire\n5. Quit\nChoice: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) startCombat();
        else if (choice == 2) showPlayerStats();
        else if (choice == 3) player->showInventory();
        else if (choice == 4) showBestiary();
        else if (choice == 5) break;
    }
}

void Game::loadItems(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Erreur: impossible d'ouvrir " << path << '\n';
        exit(1); 
    }
    string line; int lineNo = 0;
    while (getline(file, line)) {
        ++lineNo;
        if (line.empty() || line[0] == '#') continue;
        stringstream ss(line);
        string name, type, valueStr, qtyStr;
        if (!getline(ss, name, ';')) continue;
        if (!getline(ss, type, ';')) continue;
        if (!getline(ss, valueStr, ';')) continue;
        if (!getline(ss, qtyStr, ';')) continue;
        try {
            int value = stoi(valueStr);
            int qty = stoi(qtyStr);
            Item it(name, type, value, qty);
            player->addItem(it);
        }
        catch (...) {
            cerr << "Warning: ligne " << lineNo << " malformee dans " << path << '\n';
        }
    }
}

void Game::loadMonsters(string path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Erreur: impossible d'ouvrir " << path << '\n';
        exit(1); 
    }
    string line; int lineNo = 0;
    while (getline(file, line)) {
        ++lineNo;
        if (line.empty() || line[0] == '#') continue;
        stringstream ss(line);
        string name, catStr, hpStr, atkStr, defStr, mercyStr, actionsStr;
        if (!getline(ss, name, ';'))       continue;
        if (!getline(ss, catStr, ';'))     continue;
        if (!getline(ss, hpStr, ';'))      continue;
        if (!getline(ss, atkStr, ';'))     continue;
        if (!getline(ss, defStr, ';'))     continue;
        if (!getline(ss, mercyStr, ';'))   continue;
        getline(ss, actionsStr);

        try {
            int hp = stoi(hpStr);
            int atk = stoi(atkStr);
            int def = stoi(defStr);
            int mercyGoal = stoi(mercyStr);

            vector<string> acts;
            stringstream as(actionsStr);
            string aid;
            while (getline(as, aid, ','))
                if (!aid.empty()) acts.push_back(aid);

            
            if (catStr == "NORMAL")
                monstersList.push_back(make_unique<Normal>(name, hp, atk, def, mercyGoal, acts));
            else if (catStr == "MINIBOSS")
                monstersList.push_back(make_unique<Miniboss>(name, hp, atk, def, mercyGoal, acts));
            else if (catStr == "BOSS")
                monstersList.push_back(make_unique<Boss>(name, hp, atk, def, mercyGoal, acts));
            else
                cerr << "Warning: categorie inconnue ligne " << lineNo << '\n';

        }
        catch (...) {
            cerr << "Warning: ligne " << lineNo << " malformee dans " << path << '\n';
        }
    }
}

void Game::showBestiary() {
    cout << "\n=== BESTIAIRE (" << bestiary.size() << " monstres vaincus) ===\n";
    if (bestiary.empty()) { cout << "  Aucun monstre vaincu.\n"; return; }
    for (size_t i = 0; i < bestiary.size(); ++i) {
        auto& e = bestiary[i];
        cout << i + 1 << ". " << e.label << e.name
            << " -> " << (e.spared ? "EPARGNE" : "TUE") << '\n';
    }
}

void Game::showPlayerStats() {
    if (!player) return;
    cout << "\n=== STATISTIQUES ===\n";
    cout << "Nom      : " << player->getName() << '\n';
    cout << "HP       : " << player->getHP() << " / " << player->getHPMax() << '\n';
    cout << "Tues     : " << killedCount << '\n';
    cout << "Epargnes : " << sparedCount << '\n';
    cout << "Victoires: " << victoryCount << " / 10\n";
}

void Game::checkEndCondition() {
    if (victoryCount < 10) return;

    cout << "\n=== FIN DE PARTIE ===\n";
    cout << "Tues: " << killedCount << "  Epargnes: " << sparedCount << '\n';

    if (killedCount == 0)
        cout << "Fin Pacifiste : tu as epargne tous les monstres.\n";
    else if (sparedCount == 0)
        cout << "Fin Genocidaire : tu as tue tous les monstres.\n";
    else
        cout << "Fin Neutre\n";

    exit(0);
}

void Game::startCombat() {
    if (monstersList.empty()) { cout << "Aucun monstre charge.\n"; return; }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, (int)monstersList.size() - 1);

    int idx = dis(gen);
    Monster* monsterPtr = monstersList[idx].get();

    monsterPtr->takeDamage(0); 
    unique_ptr<Monster> fighter;
    vector<string> acts = monsterPtr->getActionIds();

    if (monsterPtr->getCategory() == Category::NORMAL)
        fighter = make_unique<Normal>(monsterPtr->getName(), monsterPtr->getHPMax(), monsterPtr->getAtk(), monsterPtr->getDef(), monsterPtr->getMercyGoal(), acts);
    else if (monsterPtr->getCategory() == Category::MINIBOSS)
        fighter = make_unique<Miniboss>(monsterPtr->getName(), monsterPtr->getHPMax(), monsterPtr->getAtk(), monsterPtr->getDef(), monsterPtr->getMercyGoal(), acts);
    else
        fighter = make_unique<Boss>(monsterPtr->getName(), monsterPtr->getHPMax(), monsterPtr->getAtk(), monsterPtr->getDef(), monsterPtr->getMercyGoal(), acts);

    Monster* monster = fighter.get();

    cout << "\n=== COMBAT: " << monster->getLabel() << monster->getName() << " ===\n";

    while (player->isAlive() && monster->isAlive()) {
        cout << "\nPlayer HP: " << player->getHP() << "/" << player->getHPMax() << '\n';
        cout << monster->getLabel() << monster->getName()
            << "  HP: " << monster->getHP() << "/" << monster->getHPMax()
            << "  Mercy: " << monster->getMercy() << "/" << monster->getMercyGoal() << '\n';

        cout << "\n1. FIGHT  2. ITEM  3. ACT  4. MERCY\nChoice: ";
        int choice;
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            uniform_int_distribution<> dmgDist(0, monster->getHPMax());
            int dmg = dmgDist(gen);
            if (dmg == 0) cout << "Coup rate!\n";
            else { cout << "You deal " << dmg << " damage!\n"; monster->takeDamage(dmg); }
        }
        else if (choice == 2) {
            player->showInventory();
            cout << "Index item: ";
            int i; cin >> i; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (!player->useItem((size_t)i)) cout << "Item invalide.\n";
        }
        else if (choice == 3) {
            auto& acts = monster->getActionIds();
            if (acts.empty()) { cout << "Aucune action disponible.\n"; }
            else {
                for (size_t i = 0; i < acts.size(); ++i) {
                    Action* a = findActionById(acts[i]);
                    if (a) cout << i << ". " << a->text << " (mercy " << a->mercyDelta << ")\n";
                    else   cout << i << ". " << acts[i] << '\n';
                }
                cout << "Choix: ";
                int ai; cin >> ai; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (ai >= 0 && ai < (int)acts.size()) {
                    Action* a = findActionById(acts[ai]);
                    if (a) { cout << a->text << '\n'; monster->modifyMercy(a->mercyDelta); }
                    else     monster->modifyMercy(10);
                    cout << "Mercy: " << monster->getMercy() << "/" << monster->getMercyGoal() << '\n';
                }
            }
        }
        else if (choice == 4) {
            if (monster->canBeSpared()) {
                cout << "You spared " << monster->getName() << "!\n";
                sparedCount++; victoryCount++;
                bestiary.push_back({ monster->getName(), monster->getLabel(), true });
                checkEndCondition();
                return;
            }
            else cout << "Mercy pas encore disponible.\n";
        }

        if (!monster->isAlive()) {
            cout << "You defeated " << monster->getLabel() << monster->getName() << "!\n";
            killedCount++; victoryCount++;
            bestiary.push_back({ monster->getName(), monster->getLabel(), false });
            checkEndCondition();
            return;
        }

        uniform_int_distribution<> mDmgDist(0, player->getHPMax());
        int mdmg = mDmgDist(gen);
        if (mdmg == 0) cout << monster->getName() << " rate son attaque!\n";
        else { cout << monster->getName() << " hits you for " << mdmg << "!\n"; player->takeDamage(mdmg); }

        if (!player->isAlive()) { cout << "\nGame Over.\n"; return; }
    }
}