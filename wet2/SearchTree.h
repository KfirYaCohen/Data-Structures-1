#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <iostream>

#include "Node.h"
class Cmp {
   private:
   public:
    int operator()(shared_ptr<Employee> e1, shared_ptr<Employee> e2) {
        if (e1->getSalary() == e2->getSalary()) {
            return (e1->getId() - e2->getId());
        } else {
            return e1->getSalary() - e2->getSalary();
        }
    }
};
void destroyFrom(Node* node) {
    if (node == NULL) return;
    destroyFrom(node->getLeft());
    destroyFrom(node->getRight());
    node->getValue()->resetEmployer();
    delete node;
}

Node* searchIn(shared_ptr<Employee> value, Node* node, Cmp cmp) {
    if (node == NULL || cmp(node->getValue(), value) == 0) {
        return node;
    }
    if (cmp(value, node->getValue()) < 0) {
        return searchIn(value, node->getLeft(), cmp);
    } else {
        return searchIn(value, node->getRight(), cmp);
    }
}

Node* insertTo(shared_ptr<Employee> value, Node* node, Cmp& cmp) {
    if (node == NULL) {
        node = new Node(value);
    } else if (cmp(value, node->getValue()) < 0) {
        node->setLeft(insertTo(value, node->getLeft(), cmp));
        (node->getLeft())->setFather(node);
        node->increaseSumGrade(value->getGrade());
        node->increaseRank();

    } else {
        node->setRight(insertTo(value, node->getRight(), cmp));
        (node->getRight())->setFather(node);
        node->increaseSumGrade(value->getGrade());
        node->increaseRank();
    }

    // node->setHeight(getMaxHeight(node->getLeft(), node->getRight()) + 1);
    return node;
}

Node* getMinFrom(Node* node) {
    if (node->getLeft() == NULL) {
        return node;
    } else
        return getMinFrom(node->getLeft());
}

Node* removeFrom(shared_ptr<Employee> value, Node* node, Cmp cmp) {
    shared_ptr<Employee> nodeValue = node->getValue();
    Node* left = node->getLeft();
    Node* right = node->getRight();
    Node* father = node->getFather();

    if (cmp(value, nodeValue) < 0) {
        node->setLeft(removeFrom(value, left, cmp));
        if (node->getLeft() != NULL) {
            (node->getLeft())->setFather(node);
        }
        node->decreaseRank();
        node->decreaseSumGrade(value->getGrade());
    } else if (cmp(value, nodeValue) > 0) {
        node->setRight(removeFrom(value, right, cmp));
        if (node->getRight() != NULL) {
            (node->getRight())->setFather(node);
        }
        node->decreaseRank();
        node->decreaseSumGrade(value->getGrade());
    } else {                                  // need to remove node
        if (left != NULL && right != NULL) {  // node with 2 sons

            node->decreaseSumGrade(value->getGrade());
            node->decreaseRank();
            shared_ptr<Employee> minValAbove = getMinFrom(right)->getValue();
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

Node* getMaxFrom(Node* node) {
    if (node->getRight() == NULL) {
        return node;
    } else
        return getMaxFrom(node->getRight());
}

void printBT(const std::string& prefix, const Node* node, bool isLeft) {
    if (node != NULL) {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──");

        // print the value of the node
        std::cout << node->getValue()->getId() << "," << node->getValue()->getSalary() << "," << node->getValue()->getGrade() << std::endl;
        // std::cout << node->getValue()->getId() << std::endl;

        // enter the next tree level - left and right branch
        printBT(prefix + (isLeft ? "│   " : "    "), node->getRight(), true);
        printBT(prefix + (isLeft ? "│   " : "    "), node->getLeft(), false);
    }
}

void printBT(const Node* node) { printBT("", node, false); }

// returns the first "amount" elements into the "array" parameter while doing an in order traversal
void getAmountInOrderFrom(Node* node, shared_ptr<Employee>* array, int* c, int amount) {
    if (node == NULL || *c == amount) {
        return;
    }
    getAmountInOrderFrom(node->getLeft(), array, c, amount);
    if (*c < amount) {
        shared_ptr<Employee> val = node->getValue();
        array[*c] = val;
        (*c)++;

        getAmountInOrderFrom(node->getRight(), array, c, amount);
    }
}

Node* deepCopy(Node* other) {
    Node* newNode;
    if (other != NULL) {
        newNode = new Node(other->getValue());
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

class SearchTree {
   protected:
    Node* root;
    Cmp cmp;
    int count;

   public:
    SearchTree(int count = 0) : root(NULL), cmp(Cmp()), count(count){};
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
    Node* getRoot() { return this->root; }
    bool isEmpty() const {
        if (root == NULL) return true;
        return false;
    }

    Node* search(shared_ptr<Employee> value) const { return searchIn(value, root, cmp); }

    virtual void insert(shared_ptr<Employee> value) {
        if (value == NULL) {
            return;
        }
        if (search(value) != NULL) {
            throw AlreadyExists();
        }
        root = insertTo(value, root, cmp);
        count++;
    }
    virtual void remove(shared_ptr<Employee> value) {
        if (value == NULL) {
            return;
        }
        if (search(value) == NULL) {
            throw DoesNotExists();
        }
        root = removeFrom(value, root, cmp);
        count--;
    }
    void getAmountInOrder(int amount, shared_ptr<Employee>* array) {
        int c = 0;
        getAmountInOrderFrom(root, array, &c, amount);
    }

    void print() { printBT(root); }
    shared_ptr<Employee> getMaxValue() {
        if (root == NULL) return NULL;
        return getMaxFrom(root)->getValue();
    }
    shared_ptr<Employee> getMinValue() {
        if (root == NULL) return NULL;
        return getMinFrom(root)->getValue();
    }
};

#endif  // SEARCH_TREE_H