// COMSC-210 | Lab 17 | Sarthak Pani
// Adapted from the instructor's linked list starter.
#include <cstdlib>
#include <iostream>
using namespace std;

const int SIZE = 7;

struct Node {
    float value;
    Node *next;
};

void addFront(Node *&head, float value);
void addEnd(Node *&head, float value);
bool deleteNode(Node *&head, int position);
bool insertAfter(Node *head, int position, float value);
void deleteList(Node *&head);
void output(const Node *head);

int main() {
    Node *head = nullptr;

    // Create seven nodes with random values from 0 through 99.
    for (int i = 0; i < SIZE; i++) {
        addFront(head, rand() % 100);
    }
    output(head);

    cout << "Which node to delete? Use its numbered position.\n";
    int entry;
    cout << "Choice --> ";
    if (!(cin >> entry)) {
        deleteList(head);
        return 0;
    }
    if (!deleteNode(head, entry)) {
        cout << "Invalid node position.\n";
    }
    output(head);

    cout << "After which node should 10000 be inserted?\n"
         << "Use an existing numbered position; the last node is allowed.\n";
    cout << "Choice --> ";
    if (!(cin >> entry)) {
        deleteList(head);
        return 0;
    }
    if (!insertAfter(head, entry, 10000)) {
        cout << "Invalid node position.\n";
    }
    output(head);

    deleteList(head);
    output(head);
    return 0;
}

// Head is a reference because adding at the front updates the caller's head.
void addFront(Node *&head, float value) {
    head = new Node{value, head};
}

// Head is a reference because adding to an empty list updates the caller's head.
void addEnd(Node *&head, float value) {
    if (!head) {
        addFront(head, value);
        return;
    }
    Node *current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = new Node{value, nullptr};
}

// Head is a reference because deleting the first node updates the caller's head.
// Positions start at 1; invalid positions leave the list unchanged.
bool deleteNode(Node *&head, int position) {
    if (position < 1) {
        return false;
    }
    Node *current = head;
    Node *previous = nullptr;
    for (int i = 1; current && i < position; i++) {
        previous = current;
        current = current->next;
    }
    if (!current) {
        return false;
    }
    if (!previous) {
        head = current->next;
    } else {
        previous->next = current->next;
    }
    delete current;
    return true;
}

// Head is passed by value because inserting after a node never replaces head.
// Position 1 means after the first node; the last node is also valid.
bool insertAfter(Node *head, int position, float value) {
    if (position < 1) {
        return false;
    }
    Node *current = head;
    for (int i = 1; current && i < position; i++) {
        current = current->next;
    }
    if (!current) {
        return false;
    }
    current->next = new Node{value, current->next};
    return true;
}

// Head is a reference because clearing the list must reset the caller's head.
void deleteList(Node *&head) {
    while (head) {
        Node *current = head;
        head = head->next;
        delete current;
    }
}

// A pointer to const nodes lets printing inspect the list without changing it.
void output(const Node *head) {
    if (!head) {
        cout << "Empty list.\n";
        return;
    }
    int count = 1;
    const Node *current = head;
    while (current) {
        cout << "[" << count++ << "] " << current->value << '\n';
        current = current->next;
    }
    cout << '\n';
}
