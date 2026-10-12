#pragma once
#include <string>

using namespace std;

class Item {
private:
    string name;
    string type; 
    int value;
    int quantity;

public:
    Item();
    Item(string n, string t, int v, int q);

    string getName();
    string getType();
    int getValue();
    int getQuantity();

    void setQuantity(int q);
};
