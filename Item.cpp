#include "Item.h"

Item::Item() : name(""), type(""), value(0), quantity(0) {}
Item::Item(string n, string t, int v, int q) : name(n), type(t), value(v), quantity(q) {}

string Item::getName() { return name; }
string Item::getType() { return type; }
int Item::getValue() { return value; }
int Item::getQuantity() { return quantity; }

void Item::setQuantity(int q) { quantity = q; }
