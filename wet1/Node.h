#ifndef NODE_H
#define NODE_H

#include "exceptions.h"

template <class T>
class Node {
   private:
    T value;
    Node *left;
    Node *right;
    Node *father;
    int height;

   public:
    Node(T value) : value(value), left(NULL), right(NULL), father(NULL), height(0){};
    ~Node() = default;
    Node *getLeft() const { return this->left; }
    Node *getRight() const { return this->right; }
    Node *getFather() const { return this->father; }
    int getHeight() const { return this->height; }
    T getValue() const { return this->value; }
    void setLeft(Node *left) { this->left = left; }
    void setRight(Node *right) { this->right = right; }
    void setFather(Node *father) { this->father = father; }
    void setHeight(int n) { this->height = n; }
    void setValue(T value) { this->value = value; }
};

#endif  // NODE_H