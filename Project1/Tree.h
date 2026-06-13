#ifndef TREE_H
#define TREE_H

#include "Vertex.h"

class Tree {
private:
    Vertex* head; 
    int size;
    int totalApologies;

    // Вспомогательный DFS для проверки писем в поддереве
    bool dfs(int u, int parentId);
    // Поиск вершины по id в списке вершин
    Vertex* findVertex(int id);
    // Добавляет новую вершину в однонаправленный список вершин
    void appendVertex(int id);
    // Вычисляет максимальную глубину в поддереве среди нужных вершин
    int maxDepth(int u, int parentId);
    // Симулирует проход тритона по минимальному поддереву, считает извинения
    void simulateTraverse(int u, int parentId, bool &isFirst, int &apologies, bool onFinalPath);
    bool connectivityDfs(int u, int parentId);

    void printTreeRecursive(int currentId, int parentId, int level);

    // Проверка связности графа (все вершины достижимы из 1)
    bool isConnected();

public:
    Tree();
    ~Tree();

    void allocate(int m);
    void clear();

    void setVertexData(int id, int letters);
    void addEdge(int from, int to);

    void printTree();
    int calculateApologies();
    bool isConnectedPublic() { return isConnected(); }
   
};

#endif