#include "node.h"

// Constructor implementation
Node::Node(std::shared_ptr<Order> d, Node* left, Node* right, int height)
    : data(d), left(left), right(right), height(height) {}

// Copy constructor implementation
Node::Node(const Node& other)
    : data(other.data), left(other.left), right(other.right), height(other.height) {}

// Assignment operator implementation
Node& Node::operator=(const Node& other) {
    if (this != &other) {
        data = other.data; // std::shared_ptr handles the reference count
        left = other.left;
        right = other.right;
        height = other.height;
    }
    return *this;
}

// Equality operator implementation
bool Node::operator==(const Node& other) {
    return data == other.data;
}
