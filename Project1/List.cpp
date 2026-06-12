#include "List.h"

List::List() : head(nullptr), currentSize(0) {}

List::~List() {
    clear();
}

void List::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    currentSize = 0;
}

void List::push_back(int val) {
    Node* newNode = new Node(val);
    if (head == nullptr) {
        head = newNode;
    }
    else {
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    currentSize++;
}

int List::get(int index) const {
    Node* temp = head;
    int count = 0;
    while (temp != nullptr) {
        if (count == index) return temp->data;
        count++;
        temp = temp->next;
    }
    return -1; // Ошибка индекса
}

int List::size() const {
    return currentSize;
}