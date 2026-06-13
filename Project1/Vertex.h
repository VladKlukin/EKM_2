#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include "List.h"


class Vertex {
public:
    // Представляет вершину дерева: id, количество писем, список соседей и флаги состояния
    int id;
    int letters;
    List neighbors;
    bool visited; // Для обхода графа (так как связи двунаправленные)
    bool needed;  // Указывает, что вершина лежит на путях к адресатам
    Vertex* next; // Для односвязного списка вершин
    int origLetters; // для бэкапа количества писем при симуляции

    Vertex() : id(0), letters(0), visited(false), needed(false), next(nullptr) {}
};

#endif