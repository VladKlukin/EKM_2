#pragma once

#include "Tree.h"

class Menu {
private:
    Tree tree;

    // Ввод структуры дерева с клавиатуры
    bool inputFromKeyboard();
    // Загрузка структуры дерева из файла
    bool inputFromFile();
    // Генерация случайного дерева
    bool inputRandom();

public:
    void showMainMenu();
};
