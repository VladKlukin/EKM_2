#pragma once

#include "Tree.h"

class Menu {
private:
    Tree tree;

    bool inputFromKeyboard();
    bool inputFromFile();
    bool inputRandom();

public:
    void showMainMenu();
};
