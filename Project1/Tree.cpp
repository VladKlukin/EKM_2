#include "Tree.h"
#include <iostream>
#include <functional>

#include <string>

Tree::Tree() : head(nullptr), size(0), totalApologies(0) {}

Tree::~Tree() {
    clear();
}

// traceRoute removed

void Tree::clear() {
    Vertex* cur = head;
    while (cur != nullptr) {
        Vertex* next = cur->next;
        delete cur;
        cur = next;
    }
    head = nullptr;
    size = 0;
    totalApologies = 0;
}

void Tree::appendVertex(int id) {
    Vertex* node = new Vertex();
    node->id = id;
    node->letters = 0;
    node->visited = false;
    node->needed = false;
    node->next = nullptr;

    if (head == nullptr) {
        head = node;
    } else {
        Vertex* temp = head;
        while (temp->next != nullptr) temp = temp->next;
        temp->next = node;
    }
}

Vertex* Tree::findVertex(int id) {
    Vertex* temp = head;
    while (temp != nullptr) {
        if (temp->id == id) return temp;
        temp = temp->next;
    }
    return nullptr;
}

void Tree::allocate(int m) {
    clear();
    size = m;
    head = nullptr;
    for (int i = 1; i <= m; i++) appendVertex(i);
}

void Tree::setVertexData(int id, int letters) {
    Vertex* v = findVertex(id);
    if (v) v->letters = letters;
}

void Tree::addEdge(int from, int to) {
    Vertex* vf = findVertex(from);
    Vertex* vt = findVertex(to);
    if (vf == nullptr || vt == nullptr) return;

    bool exists = false;
    for (int i = 0; i < vf->neighbors.size(); i++) {
        if (vf->neighbors.get(i) == to) { exists = true; break; }
    }
    if (!exists) vf->neighbors.push_back(to);

    exists = false;
    for (int i = 0; i < vt->neighbors.size(); i++) {
        if (vt->neighbors.get(i) == from) { exists = true; break; }
    }
    if (!exists) vt->neighbors.push_back(from);
}

// Помечает вершины как needed, если в их поддереве (от u, не переходя в parentId)
// есть хотя бы одна вершина с letters > 0. Возвращает true, если такой адресат найден.
bool Tree::dfs(int u, int parentId) {
    Vertex* vu = findVertex(u);
    if (vu == nullptr) return false;
    bool has = (vu->letters > 0);
    for (int i = 0; i < vu->neighbors.size(); i++) {
        int v = vu->neighbors.get(i);
        if (v == parentId) continue;
        if (dfs(v, u)) has = true;
    }
    vu->needed = has;
    return has;
}

int Tree::calculateApologies() {
    if (size == 0 || head == nullptr) return 0;
    if (!dfs(1, -1)) return 0;

    // Backup original letters to restore later
    Vertex* t = head;
    while (t) { t->origLetters = t->letters; t = t->next; }

    int apologies = 0;
    // Считаем извинение даже при первоначальном положении в вершине 1, если в ней нет писем
    bool isFirst = false;
    // Пометим путь до самой глубокой нужной вершины как финальный путь
    // Для этого можно выбрать у корня lastChild по maxDepth, но проще — считать, что весь корень на финальном пути
    simulateTraverse(1, -1, isFirst, apologies, true);

    // Restore letters
    t = head;
    while (t) { t->letters = t->origLetters; t = t->next; }

    return apologies;
}

int Tree::maxDepth(int u, int parentId) {
    Vertex* v = findVertex(u);
    if (!v) return 0;
    int best = 0;
    for (int i = 0; i < v->neighbors.size(); i++) {
        int nb = v->neighbors.get(i);
        if (nb == parentId) continue;
        if (findVertex(nb)->needed) {
            int d = 1 + maxDepth(nb, u);
            if (d > best) best = d;
        }
    }
    return best;
}

// Симуляция прохода тритона по минимальному поддереву needed
void Tree::simulateTraverse(int u, int parentId, bool &isFirst, int &apologies, bool onFinalPath) {
    Vertex* v = findVertex(u);
    if (!v) return;

    // Вход в вершину
    if (!isFirst) {
        if (v->letters == 0) apologies++; // извиняемся, если нет писем
    } else isFirst = false;

    // Если есть письма — отдать (ставим 0)
    if (v->letters > 0) v->letters = 0;

    // Обход соседей, которые нужны
    // Соберём список нужных соседей
    int childCount = 0;
    int lastChild = -1;
    int bestDepth = -1;
    for (int i = 0; i < v->neighbors.size(); i++) {
        int nb = v->neighbors.get(i);
        if (nb == parentId) continue;
        Vertex* vn = findVertex(nb);
        if (!vn || !vn->needed) continue;
        int d = maxDepth(nb, u);
        if (d > bestDepth) { bestDepth = d; lastChild = nb; }
        childCount++;
    }

    // Проходим по всем детям, кроме выбранного как lastChild
    for (int i = 0; i < v->neighbors.size(); i++) {
        int nb = v->neighbors.get(i);
        if (nb == parentId) continue;
        Vertex* vn = findVertex(nb);
        if (!vn || !vn->needed) continue;
        if (nb == lastChild) continue; // отложим на потом

        // этот вызов не на финальном пути
        simulateTraverse(nb, u, isFirst, apologies, false);
        // возвращаемся в v после этой ветви — извиняемся, если в v нет писем
        if (v->letters == 0) apologies++;
    }

    // Теперь последний ребёнок (если есть)
    if (lastChild != -1) {
        // последний ребёнок: если текущая вершина onFinalPath == true, передаём true, чтобы последняя ветвь могла продолжить финальный маршрут
        simulateTraverse(lastChild, u, isFirst, apologies, onFinalPath);
        // если текущая вершина не на финальном пути, то после возвращения из последней ветви тритон вернётся в v и извинится
        if (!onFinalPath) {
            if (v->letters == 0) apologies++;
        }
    }
}

void Tree::printTreeRecursive(int currentId, int parentId, int level) {
    for (int i = 0; i < level; i++) std::cout << "    ";
    if (level > 0) std::cout << "|-- ";
    Vertex* v = findVertex(currentId);
    if (v) std::cout << "Гнездо " << currentId << " (Писем: " << v->letters << ")\n";

    if (!v) return;
    for (int i = 0; i < v->neighbors.size(); i++) {
        int neighborId = v->neighbors.get(i);
        if (neighborId != parentId) printTreeRecursive(neighborId, currentId, level + 1);
    }
}

void Tree::printTree() {
    if (size == 0 || head == nullptr) {
        std::cout << "Дерево пустое.\n";
        return;
    }
    std::cout << "\n--- СТРУКТУРА ДУБА (Корень в Гнезде 1) ---\n";
    printTreeRecursive(1, -1, 0);
    std::cout << "-----------------------------------------\n";
}

bool Tree::connectivityDfs(int u, int parentId) {
    Vertex* v = findVertex(u);
    if (v == nullptr) return false;
    v->visited = true;
    for (int i = 0; i < v->neighbors.size(); i++) {
        int nb = v->neighbors.get(i);
        if (nb == parentId) continue;
        Vertex* vn = findVertex(nb);
        if (vn && !vn->visited) connectivityDfs(nb, u);
    }
    return true;
}

bool Tree::isConnected() {
    if (head == nullptr) return true;
    Vertex* t = head;
    while (t) { t->visited = false; t = t->next; }
    connectivityDfs(1, -1);
    t = head;
    while (t) {
        if (!t->visited) return false;
        t = t->next;
    }
    return true;
}
