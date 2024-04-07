#ifndef NODE_H
#define NODE_H

#include "order.h"
#include <memory> // For std::shared_ptr

class Node {
public:
    std::shared_ptr<Order> data;
    Node* left;
    Node* right;
    int height;

    // Constructor
    Node(std::shared_ptr<Order> d, Node* left = nullptr, Node* right = nullptr, int height = 1);

    // Copy constructor
    Node(const Node& other);

    // Assignment operator
    Node& operator=(const Node& other);

    // Equality operator
    bool operator==(const Node& other);
};

#endif // NODE_H

