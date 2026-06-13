#pragma once
#ifndef LIST_H
#define LIST_H

#include "Node.h"

class List {
private:
    Node* head;
    int currentSize;
public:
    // Односвязный список типа int, используется для хранения соседей вершины
    List();
    ~List();
    void clear();
    void push_back(int val);
    int get(int index) const;
    int size() const;
};

#endif