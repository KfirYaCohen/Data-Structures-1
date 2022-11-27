#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>

#include "SearchTree.h"

template <class T, class Cmp>
class AVLTree : public SearchTree<T, Cmp> {
   private:
    bool checkIfRollNeeded(Node<T>* p) {
        int BF = getBalanceFactor(p);
        bool didRoll = 0;
        if (BF == 2) {  // 3.4 Not Balanced
            if (getBalanceFactor(p->getLeft()) == -1) {
                p = LR(p);
            } else {
                p = LL(p);
            }
            didRoll = 1;
        } else if (BF == -2) {  // 3.4 Not Balanced
            if (getBalanceFactor(p->getRight()) <= 0) {
                p = RR(p);
            } else {
                p = RL(p);
            }
            didRoll = 1;
        }
        if (didRoll == 1) {                       // 3.4
            Node<T>* nodeAbove = p->getFather();  // fixing the node before p
            if (nodeAbove != NULL) {
                if (this->cmp(p->getValue(), nodeAbove->getValue()) < 0) {
                    nodeAbove->setLeft(p);
                } else {
                    nodeAbove->setRight(p);
                }
                nodeAbove->setHeight(getMaxHeight(nodeAbove->getLeft(), nodeAbove->getRight()) + 1);
            } else {  // If Father of p is null, it is the root
                this->root = p;
            }
            return true;
        }
        return false;
    }

   public:
    AVLTree(Cmp cmp) : SearchTree<T, Cmp>(cmp) {}
    AVLTree(Cmp cmp, T* arr, int count) : SearchTree<T, Cmp>(cmp, count) { this->root = AVLTreeFromSortedArray(arr, 0, count - 1); };

    int getHeight() const { return this->root->getHeight(); }
    void insert(T value) override {                                        // Using the algorithm from Erez presentation
        SearchTree<T, Cmp>::insert(value);                                 // 1+2
        Node<T>* v = SearchTree<T, Cmp>::search(value);                    // 1
        while ((this->cmp)(v->getValue(), this->root->getValue()) != 0) {  // 3 comparing using elements's != operartor
            Node<T>* p = v->getFather();                                   // 3.1
            if (p->getHeight() >= v->getHeight() + 1) {                    // 3.2
                return;
            }
            p->setHeight(v->getHeight() + 1);  // 3.3
            if (checkIfRollNeeded(p) == true) {
                return;
            }
            v = p;  // 3.5
        }
    }

    void remove(T value) override {  // using algorithm from Rami's presentation
        Node<T>* v = SearchTree<T, Cmp>::search(value);
        if (v == NULL) {
            // throw DoesNotExists();
            return;
        }
        Node<T>* p;
        if (v->getLeft() && v->getRight()) {  // 2 sons
            p = getMinFrom(v->getRight())->getFather();
        } else {
            p = v->getFather();
        }
        SearchTree<T, Cmp>::remove(value);  // 1 removing v
        v = p;
        while (v != NULL && (this->cmp)(v->getValue(), this->root->getValue()) != 0) {
            p = v->getFather();
            int prevHeight = p->getHeight();                              // 3.1
            v->setHeight(getMaxHeight(v->getLeft(), v->getRight()) + 1);  // 3.2
            checkIfRollNeeded(v);                                         // 3.2

            if (prevHeight == p->getHeight()) {
                return;
            }
            v = p;
        }
    }
};

template <class T>
Node<T>* LR(Node<T>* A) {
    Node<T>* B = A->getLeft();
    Node<T>* C = B->getRight();
    Node<T>* w = C->getRight();
    Node<T>* y = C->getLeft();

    C->setLeft(B);
    C->setRight(A);
    B->setRight(y);
    A->setLeft(w);

    C->setFather(A->getFather());
    A->setFather(C);
    B->setFather(C);
    if (y != NULL) {
        y->setFather(B);
    }
    if (w != NULL) {
        w->setFather(A);
    }

    A->setHeight(getMaxHeight(A->getLeft(), A->getRight()) + 1);
    B->setHeight(getMaxHeight(B->getLeft(), B->getRight()) + 1);
    C->setHeight(getMaxHeight(C->getLeft(), C->getRight()) + 1);

    return C;
}

template <class T>
Node<T>* LL(Node<T>* A) {
    Node<T>* B = A->getLeft();
    Node<T>* y = B->getRight();

    B->setRight(A);
    A->setLeft(y);

    B->setFather(A->getFather());
    A->setFather(B);
    if (y != NULL) {
        y->setFather(A);
    }

    A->setHeight(getMaxHeight(A->getLeft(), A->getRight()) + 1);
    B->setHeight(getMaxHeight(B->getLeft(), B->getRight()) + 1);

    return B;
}

template <class T>
Node<T>* RL(Node<T>* A) {
    Node<T>* B = A->getRight();
    Node<T>* C = B->getLeft();
    Node<T>* w = C->getRight();
    Node<T>* y = C->getLeft();

    C->setLeft(A);
    C->setRight(B);
    A->setRight(y);
    B->setLeft(w);

    C->setFather(A->getFather());
    A->setFather(C);
    B->setFather(C);
    if (y != NULL) {
        y->setFather(A);
    }
    if (w != NULL) {
        w->setFather(B);
    }

    A->setHeight(getMaxHeight(A->getLeft(), A->getRight()) + 1);
    B->setHeight(getMaxHeight(B->getLeft(), B->getRight()) + 1);
    C->setHeight(getMaxHeight(C->getLeft(), C->getRight()) + 1);

    return C;
}

template <class T>
Node<T>* RR(Node<T>* A) {
    Node<T>* B = A->getRight();
    Node<T>* x = B->getLeft();

    B->setLeft(A);
    A->setRight(x);

    B->setFather(A->getFather());
    A->setFather(B);
    if (x != NULL) {
        x->setFather(A);
    }

    A->setHeight(getMaxHeight(A->getLeft(), A->getRight()) + 1);
    B->setHeight(getMaxHeight(B->getLeft(), B->getRight()) + 1);

    return B;
}

template <class T>
int getBalanceFactor(Node<T>* node) {
    if (node->getHeight() == 0) return 0;
    if (node->getLeft() != NULL && node->getRight() != NULL) {
        return node->getLeft()->getHeight() - node->getRight()->getHeight();
    } else if (node->getLeft() == NULL) {
        return (-1) - node->getRight()->getHeight();
    } else
        return node->getLeft()->getHeight() - (-1);
}

template <class T>
int getMaxHeight(Node<T>* left, Node<T>* right) {
    if (left == NULL && right == NULL) {
        return -1;
    } else if (left == NULL) {
        return right->getHeight();
    } else if (right == NULL) {
        return left->getHeight();
    } else {
        return right->getHeight() > left->getHeight() ? right->getHeight() : left->getHeight();
    }
}

template <class T>
Node<T>* AVLTreeFromSortedArray(T* arr, int start, int end) {
    if (start > end) return NULL;
    int mid = (start + end) / 2;
    Node<T>* root = new Node<T>(arr[mid]);
    root->setLeft(AVLTreeFromSortedArray(arr, start, mid - 1));
    root->setRight(AVLTreeFromSortedArray(arr, mid + 1, end));
    root->setHeight(getMaxHeight(root->getLeft(), root->getRight()) + 1);
    return root;
}

#endif  // AVL_TREE_H
