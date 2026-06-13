#include "Tree.h"
#include <iostream>
#include <functional>

#include <string>

Tree::Tree() : head(nullptr), size(0), totalApologies(0) {}

Tree::~Tree() {
    clear();
}



void Tree::clear() {
    // Освобождает список вершин и сбрасывает состояние дерева
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
    // Создаёт и добавляет новую вершину в конец однонаправленного списка вершин
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
    // Ищет вершину по идентификатору в списке; возвращает nullptr если не найдена
    Vertex* temp = head;
    while (temp != nullptr) {
        if (temp->id == id) return temp;
        temp = temp->next;
    }
    return nullptr;
}

void Tree::allocate(int m) {
    // Инициализирует структуру дерева с m вершинами (идентификаторы 1..m)
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
    // Добавляет двунаправленное ребро между вершинами from и to, исключая дубликаты
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
    // Рекурсивно отмечает вершины как needed, если в их поддереве есть письма (>0)
    // Обход выполняется по дереву, исключая возврат к parentId
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
    // Основная логика: помечаем нужные вершины (dfs), сохраняем оригинальные значения
    // писем, выполняем симуляцию обхода, восстанавливаем данные и возвращаем ответ
    if (size == 0 || head == nullptr) return 0;
    if (!dfs(1, -1)) return 0;

    // Сохраняем оригинальное количество писем для последующего восстановления
    Vertex* t = head;
    while (t) { t->origLetters = t->letters; t = t->next; }

    int apologies = 0;
    // Первый вызов симуляции: помечаем, что текущий путь является финальным (onFinalPath=true)
    bool isFirst = false;
    simulateTraverse(1, -1, isFirst, apologies, true);

    // Восстанавливаем оригинальные значения писем
    t = head;
    while (t) { t->letters = t->origLetters; t = t->next; }

    return apologies;
}

int Tree::maxDepth(int u, int parentId) {
    // Вычисляет максимальную длину пути вниз от u по рёбрам, ведущим к нужным вершинам
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
    // Сложная рекурсивная процедура, моделирующая оптимальный обход минимального
    // поддерева нужных вершин (needed). Алгоритм старается выбрать одну "финальную"
    // ветвь, которую не нужно возвращать (onFinalPath=true) — это позволяет сократить
    // количество возвратов и, следовательно, лишних извинений.
    Vertex* v = findVertex(u);
    if (!v) return;

    // Вход в вершину: если это не первый посещённый узел и в нём нет писем,
    // тритон извиняется
    if (!isFirst) {
        if (v->letters == 0) apologies++; // извиняемся, если нет писем
    } else isFirst = false;

    // Если у вершины есть письма — отдать их (обнуляем)
    if (v->letters > 0) v->letters = 0;

    // Находим среди соседей те, которые помечены как needed и выбираем lastChild
    // — ту ветвь, которая ведёт к самой глубокой нужной вершине и оставляется
    // для прохода без возврата (финальная ветвь)
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

    // Рекурсивно обходим все нужные подветви, кроме lastChild. Для каждой такой
    // подветви тритон возвращается в текущую вершину v, и если у v нет писем,
    // приходится извиняться после возврата.
    for (int i = 0; i < v->neighbors.size(); i++) {
        int nb = v->neighbors.get(i);
        if (nb == parentId) continue;
        Vertex* vn = findVertex(nb);
        if (!vn || !vn->needed) continue;
        if (nb == lastChild) continue; // отложим на потом

        // этот вызов не считается частью финального пути
        simulateTraverse(nb, u, isFirst, apologies, false);
        // возвращаемся в v после этой ветви — извиняемся, если в v нет писем
        if (v->letters == 0) apologies++;
    }

    // Обрабатываем последнюю ветвь (lastChild): если текущая вершина на финальном
    // пути, то передаём onFinalPath параметр дальше, иначе после возвращения
    // придётся извиниться при отсутствии писем
    if (lastChild != -1) {
        simulateTraverse(lastChild, u, isFirst, apologies, onFinalPath);
        if (!onFinalPath) {
            if (v->letters == 0) apologies++;
        }
    }
}

void Tree::printTreeRecursive(int currentId, int parentId, int level) {
    // Рекурсивный печатный обход дерева для визуализации структуры
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
    // DFS для проверки достижимости всех вершин: помечаем visited=true
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
    // Проверяет связность графа: запускает DFS от вершины 1 и убеждается, что
    // все вершины были посещены
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
