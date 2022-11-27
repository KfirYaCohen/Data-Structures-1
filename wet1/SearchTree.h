#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <iostream>

#include "Node.h"

template <class T, class Cmp>
class SearchTree {
   protected:
    Node<T>* root;
    Cmp cmp;
    int count;

   public:
    SearchTree(Cmp cmp, int count = 0) : root(NULL), cmp(cmp), count(count){};
    ~SearchTree() { destroyFrom(root); }
    void emptyTree() {
        destroyFrom(root);
        root = NULL;
        count = 0;
    }

    SearchTree& operator=(const SearchTree& other) {
        if (this == &other) {
            return *this;
        }
        destroyFrom(root);
        root = deepCopy(other.root);
        count = other.count;
        cmp = other.cmp;
        return *this;
    }

    int getCount() const { return this->count; }
    Node<T>* getRoot() { return this->root; }
    bool isEmpty() const {
        if (root == NULL) return true;
        return false;
    }

    Node<T>* search(T value) const { return searchIn(value, root, cmp); }

    virtual void insert(T value) {
        if (value == NULL) {
            return;
        }
        if (search(value) != NULL) {
            throw AlreadyExists();
        }
        root = insertTo(value, root, cmp);
        count++;
    }
    virtual void remove(T value) {
        if (value == NULL) {
            return;
        }
        if (search(value) == NULL) {
            throw DoesNotExists();
        }
        root = removeFrom(value, root, cmp);
        count--;
    }
    void printInOrder() { printInOrderFrom(root); }
    void getAmountInOrder(int amount, T* array) {
        int c = 0;
        getAmountInOrderFrom(root, array, &c, amount);
    }
    template <class Cond>
    int getAmountInRange(Cond cond) {
        return getAmountInRangeFrom(root, cond);
    }
    void print() { printBT(root); }
    T getMaxValue() {
        if (root == NULL) return NULL;
        return getMaxFrom(root)->getValue();
    }
    T getMinValue() {
        if (root == NULL) return NULL;
        return getMinFrom(root)->getValue();
    }
};

template <class T, class Cmp>
Node<T>* searchIn(T value, Node<T>* node, Cmp cmp) {
    if (node == NULL || cmp(node->getValue(), value) == 0) {
        return node;
    }
    if (cmp(value, node->getValue()) < 0) {
        return searchIn(value, node->getLeft(), cmp);
    } else {
        return searchIn(value, node->getRight(), cmp);
    }
}

template <class T, class Cmp>
Node<T>* insertTo(T value, Node<T>* node, Cmp& cmp) {
    if (node == NULL) {
        node = new Node<T>(value);
    } else if (cmp(value, node->getValue()) < 0) {
        node->setLeft(insertTo(value, node->getLeft(), cmp));
        (node->getLeft())->setFather(node);
    } else {
        node->setRight(insertTo(value, node->getRight(), cmp));
        (node->getRight())->setFather(node);
    }
    // node->setHeight(getMaxHeight(node->getLeft(), node->getRight()) + 1);
    return node;
}

template <class T, class Cmp>
Node<T>* removeFrom(T value, Node<T>* node, Cmp cmp) {
    T nodeValue = node->getValue();
    Node<T>* left = node->getLeft();
    Node<T>* right = node->getRight();
    Node<T>* father = node->getFather();

    if (cmp(value, nodeValue) < 0) {
        node->setLeft(removeFrom(value, left, cmp));
        if (node->getLeft() != NULL) {
            (node->getLeft())->setFather(node);
        }
    } else if (cmp(value, nodeValue) > 0) {
        node->setRight(removeFrom(value, right, cmp));
        if (node->getRight() != NULL) {
            (node->getRight())->setFather(node);
        }
    } else {                                  // need to remove node
        if (left != NULL && right != NULL) {  // node with 2 sons
            T minValAbove = getMinFrom(right)->getValue();
            node->setValue(minValAbove);
            node->setRight(removeFrom(minValAbove, right, cmp));
            if (node->getRight() != NULL) {
                (node->getRight())->setFather(node);
            }
        } else {  // 1 or 0 sons
            delete node;
            if (left != NULL) {
                left->setFather(father);
                return left;
            } else if (right != NULL) {
                right->setFather(father);
                return right;
            } else {
                return NULL;
            }
        }
    }
    // node->setHeight(getMaxHeight(node->getLeft(), node->getRight()) + 1);
    return node;
}

template <class T>
Node<T>* getMinFrom(Node<T>* node) {
    if (node->getLeft() == NULL) {
        return node;
    } else
        return getMinFrom(node->getLeft());
}

template <class T>
Node<T>* getMaxFrom(Node<T>* node) {
    if (node->getRight() == NULL) {
        return node;
    } else
        return getMaxFrom(node->getRight());
}

template <class T>
void printInOrderFrom(Node<T>* node) {
    if (node == NULL) return;
    printInOrderFrom(node->getLeft());
    std::cout << *(node->getValue()) << " ";
    printInOrderFrom(node->getRight());
}

template <class T>
void destroyFrom(Node<T>* node) {
    if (node == NULL) return;
    destroyFrom(node->getLeft());
    destroyFrom(node->getRight());
    delete node;
}

template <class T>
void printBT(const std::string& prefix, const Node<T>* node, bool isLeft) {
    if (node != NULL) {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──");

        // print the value of the node
        std::cout << node->getHeight() << std::endl;

        // enter the next tree level - left and right branch
        printBT(prefix + (isLeft ? "│   " : "    "), node->getRight(), true);
        printBT(prefix + (isLeft ? "│   " : "    "), node->getLeft(), false);
    }
}

template <class T>
void printBT(const Node<T>* node) {
    printBT("", node, false);
}

// returns the first "amount" elements into the "array" parameter while doing an in order traversal
template <class T>
void getAmountInOrderFrom(Node<T>* node, T* array, int* c, int amount) {
    if (node == NULL || *c == amount) {
        return;
    }
    getAmountInOrderFrom(node->getLeft(), array, c, amount);
    if (*c < amount) {
        T val = node->getValue();
        array[*c] = val;
        (*c)++;

        getAmountInOrderFrom(node->getRight(), array, c, amount);
    }
}

template <class T, class Cond>
int getAmountInRangeFrom(Node<T>* node, Cond cond) {
    if (node == NULL) {
        return 0;
    }

    if (cond(node->getValue()) == 0) {
        return 1 + getAmountInRangeFrom(node->getLeft(), cond) + getAmountInRangeFrom(node->getRight(), cond);
    } else if (cond(node->getValue()) == -1) {
        return getAmountInRangeFrom(node->getRight(), cond);
    } else if (cond(node->getValue()) == 2) {
        return getAmountInRangeFrom(node->getLeft(), cond) + getAmountInRangeFrom(node->getRight(), cond);
    } else {
        return getAmountInRangeFrom(node->getLeft(), cond);
    }
}

template <class T>
Node<T>* deepCopy(Node<T>* other) {
    Node<T>* newNode;
    if (other != NULL) {
        newNode = new Node<T>(other->getValue());
        newNode->setHeight(other->getHeight());
        newNode->setLeft(deepCopy(other->getLeft()));
        if (newNode->getLeft() != NULL) {
            newNode->getLeft()->setFather(newNode);
        }
        newNode->setRight(deepCopy(other->getRight()));
        if (newNode->getRight() != NULL) {
            newNode->getRight()->setFather(newNode);
        }
    } else {
        return NULL;
    }
    return newNode;
}

#endif  // SEARCH_TREE_H