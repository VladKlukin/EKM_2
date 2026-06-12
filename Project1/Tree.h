#ifndef TREE_H
#define TREE_H

#include "Vertex.h"

class Tree {
private:
    Vertex* head; // head of singly linked list of vertices
    int size;
    int totalApologies;

    // Вспомогательный DFS для проверки писем в поддереве
    bool dfs(int u, int parentId);
    Vertex* findVertex(int id);
    void appendVertex(int id);
    int maxDepth(int u, int parentId);
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
    // (traceRoute removed)
};

#endif