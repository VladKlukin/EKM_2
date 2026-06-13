#include "Menu.h"
#include <locale>
#include <windows.h>
int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    Menu app;
    app.showMainMenu();

    return 0;
}