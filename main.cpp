// COMSC-210 | Lab 17 | Sarthak Pani
// Adapted from the instructor's linked list starter.
#include <cerrno>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
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
int countNodes(const Node *head);
bool readInteger(const string &prompt, int minimum, int maximum, int &value);
bool readValue(float &value);

int main() {
    Node *head = nullptr;

    // Create seven nodes with random values from 0 through 99.
    for (int i = 0; i < SIZE; i++) {
        addFront(head, rand() % 100);
    }
    output(head);

    bool running = true;
    while (running) {
        cout << "\n1. Add front\n"
             << "2. Add end\n"
             << "3. Delete node\n"
             << "4. Insert node (after an existing node)\n"
             << "5. Delete list\n"
             << "6. Print list\n"
             << "7. Exit\n";
        int choice;
        if (!readInteger("Choice --> ", 1, 7, choice)) {
            break;
        }

        float value;
        int position;
        switch (choice) {
            case 1:
            case 2:
                if (!readValue(value)) {
                    running = false;
                    break;
                }
                if (choice == 1) {
                    addFront(head, value);
                } else {
                    addEnd(head, value);
                }
                break;
            case 3:
            case 4:
                if (!head) {
                    cout << "Empty list. Add front or add end first.\n";
                    break;
                }
                output(head);
                if (choice == 4) {
                    cout << "Insert AFTER an existing numbered node; "
                         << "the last node is allowed.\n";
                }
                if (!readInteger("Node position (starting at 1) --> ",
                                 1, countNodes(head), position)) {
                    running = false;
                    break;
                }
                if (choice == 3) {
                    deleteNode(head, position);
                } else if (readValue(value)) {
                    insertAfter(head, position, value);
                } else {
                    running = false;
                }
                break;
            case 5:
                deleteList(head);
                cout << "List cleared.\n";
                break;
            case 6:
                output(head);
                break;
            case 7:
                running = false;
                break;
        }
    }

    deleteList(head);
    cout << "Goodbye.\n";
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

// Counting only reads nodes, so the caller's head does not need a reference.
int countNodes(const Node *head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

// Read a whole line so mixed text and fractional positions cannot slip through.
// Value is a reference to return the validated number; false signals EOF.
bool readInteger(const string &prompt, int minimum, int maximum, int &value) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) {
            return false;
        }
        istringstream input(line);
        int candidate;
        if (input >> candidate) {
            input >> ws;
            if (input.eof() && candidate >= minimum && candidate <= maximum) {
                value = candidate;
                return true;
            }
        }
        cout << "Enter a whole number from " << minimum << " to "
             << maximum << ".\n";
    }
}

// Value is a reference to return the user's number; false signals EOF.
// strtof reports range errors, including values too small for a float.
bool readValue(float &value) {
    string line;
    while (true) {
        cout << "Value --> ";
        if (!getline(cin, line)) {
            return false;
        }
        char *end;
        errno = 0;
        float candidate = strtof(line.c_str(), &end);
        bool hasNumber = end != line.c_str();
        while (isspace(static_cast<unsigned char>(*end))) {
            end++;
        }
        if (hasNumber && end == line.c_str() + line.size() &&
            errno != ERANGE && isfinite(candidate)) {
            value = candidate;
            return true;
        }
        cout << "Enter a finite number within the float range.\n";
    }
}
