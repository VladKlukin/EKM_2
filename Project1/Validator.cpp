#include "Validator.h"
#include "Tree.h"

int Validator::getValidatedInt(int minVal, int maxVal) {
    int value;
    while (true) {
        if (std::cin >> value) {
            if (value >= minVal && value <= maxVal) {
                return value;
            }
        }
        std::cin.clear();
        while (std::cin.get() != '\n'); // Очистка буфера
        std::cout << "Некорректный ввод. Введите число от " << minVal << " до " << maxVal << ": ";
    }
}

bool Validator::isValidM(int m) {
    return (m > 0 && m < 1000);
}

bool Validator::isValidNeighbor(int neighbor, int currentId, int maxM) {
    return (neighbor >= 1 && neighbor <= maxM && neighbor != currentId);
}

bool Validator::isValidNi(int ni, int maxM) {
    return (ni >= 0 && ni <= maxM);
}

bool Validator::isValidLi(int li) {
    return (li >= 0);
}

bool Validator::isTreeConnected(Tree &tree) {
    return tree.isConnectedPublic();
}
