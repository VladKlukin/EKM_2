#pragma once
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>

class Validator {
public:
    // Проверка ввода целого числа с клавиатуры (защита от букв)
    static int getValidatedInt(int minVal, int maxVal);

    // Проверка корректности количества гнезд M
    static bool isValidM(int m);

    // Проверка корректности номеров соседей
    static bool isValidNeighbor(int neighbor, int currentId, int maxM);
    static bool isValidNi(int ni, int maxM);
    static bool isValidLi(int li);
    static bool isTreeConnected(class Tree &tree);
};

#endif