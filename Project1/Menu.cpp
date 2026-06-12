#include "Menu.h"
#include "Validator.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

void Menu::showMainMenu() {
    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    while (true) {
        std::cout << "\n=== ЗАДАЧА 'ПОЧТАЛЬОН' ===\n";
        std::cout << "1. Ввод данных с клавиатуры\n";
        std::cout << "2. Загрузка данных из файла\n";
        std::cout << "3. Генерация случайного дерева (Рандом)\n";
        std::cout << "4. Выход\n";
        std::cout << "Выберите пункт меню: ";

        int choice = Validator::getValidatedInt(1, 4);

        if (choice == 4) break;

        bool ok = false;
        switch (choice) {
        case 1: ok = inputFromKeyboard(); break;
        case 2: ok = inputFromFile(); break;
        case 3: ok = inputRandom(); break;
        }

        // Если данные введены успешно, выводим ответ
        if (ok) {
            int res = tree.calculateApologies();
            std::cout << "\n>>> Минимальное количество извинений тритона: " << res << " <<<\n";
        }
    }
}

bool Menu::inputFromKeyboard() {
    std::cout << "Введите количество гнезд M (0 < M < 1000): ";
    int m = Validator::getValidatedInt(1, 999);

    tree.allocate(m);

    std::cout << "Для каждого из M гнезд введите данные:\n";
    std::cout << "Формат: [кол-во_соседей] [кол-во_писем] [номера_соседей...]\n";

    for (int i = 1; i <= m; i++) {
        std::cout << "Гнездо " << i << ": ";
        int ni = Validator::getValidatedInt(0, m);
        while (!Validator::isValidNi(ni, m)) {
            std::cout << "Некорректное кол-во соседей. Введите еще раз: ";
            ni = Validator::getValidatedInt(0, m);
        }

        int li = Validator::getValidatedInt(0, 10000);
        while (!Validator::isValidLi(li)) {
            std::cout << "Некорректное кол-во писем. Введите еще раз: ";
            li = Validator::getValidatedInt(0, 10000);
        }

        tree.setVertexData(i, li);

        for (int j = 0; j < ni; j++) {
            int neighbor = Validator::getValidatedInt(1, m);
            if (Validator::isValidNeighbor(neighbor, i, m)) {
                tree.addEdge(i, neighbor);
            }
        }
    }
    std::cout << "\nДанные успешно введены.";
    tree.printTree();
    return true;
}

bool Menu::inputFromFile() {
    char filename[256];
    std::cout << "Введите имя файла (например, input.txt): ";
    std::cin >> filename;

    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл!\n";
        return false;
    }

    int m;
    if (!(infile >> m) || !Validator::isValidM(m)) {
        std::cout << "Ошибка: Некорректный размер M в файле.\n";
        infile.close();
        return false;
    }

    tree.allocate(m);

    for (int i = 1; i <= m; i++) {
        int ni, li;
        if (!(infile >> ni >> li)) {
            std::cout << "Ошибка чтения данных гнезда " << i << "\n";
            infile.close();
            return false;
        }
        if (!Validator::isValidNi(ni, m)) {
            std::cout << "Ошибка: недопустимое количество соседей для гнезда " << i << "\n";
            infile.close();
            return false;
        }
        if (!Validator::isValidLi(li)) {
            std::cout << "Ошибка: количество писем не может быть отрицательным (гнездо " << i << ")\n";
            infile.close();
            return false;
        }
        tree.setVertexData(i, li);

        for (int j = 0; j < ni; j++) {
            int neighbor;
            if (!(infile >> neighbor)) {
                std::cout << "Ошибка: недостаточно соседей в файле для гнезда " << i << "\n";
                infile.close();
                return false;
            }
            if (!Validator::isValidNeighbor(neighbor, i, m)) {
                std::cout << "Ошибка: неверный номер соседа " << neighbor << " для гнезда " << i << "\n";
                infile.close();
                return false;
            }
            tree.addEdge(i, neighbor);
        }
    }
    infile.close();
    if (!Validator::isTreeConnected(tree)) {
        std::cout << "Ошибка: граф не связен, некорректное описание дерева в файле.\n";
        return false;
    }
    tree.printTree();
    std::cout << "Данные из файла успешно загружены.\n";
    return true;
}

bool Menu::inputRandom() {
    std::cout << "Введите желаемое количество гнезд M для генерации: ";
    int m = Validator::getValidatedInt(1, 999); // по условию можно до 999

    tree.allocate(m);

    std::cout << "\n--- Сгенерированная структура дуба ---\n";
    std::cout << "M = " << m << "\n";

    // Для генерации гарантированного дерева свяжем каждый узел i (начиная со 2) с одним из предыдущих
    for (int i = 1; i <= m; i++) {
        // Случайное число писем от 0 до 3
        int letters = std::rand() % 4 == 0 ? 0 : std::rand() % 3; // Смещение в сторону 0, чтобы были извинения
        tree.setVertexData(i, letters);
    }

    // Генерируем дерево: для каждого i>1 создаём ребро к случайному предку в [1..i-1]
    for (int i = 2; i <= m; i++) {
        int parent = (std::rand() % (i - 1)) + 1;
        tree.addEdge(i, parent);
    }

    std::cout << "Дерево сгенерировано и готово к расчету\n";
    // Всегда печатаем структуру дерева, даже для больших m
    tree.printTree();

    return true;
}