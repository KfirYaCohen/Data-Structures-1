#ifndef EMPLOYEES_TREE_H
#define EMPLOYEES_TREE_H

#include <iostream>

#include "Employee.h"
#include "SearchTree.h"

int getMaxHeight(Node* left, Node* right) {
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

void updateRank(Node* node) {
    Node* left = node->getLeft();
    Node* right = node->getRight();
    if (left == NULL && right == NULL) {
        node->setRank(1);
    } else if (left == NULL) {
        node->setRank(1 + right->getRank());
    } else if (right == NULL) {
        node->setRank(1 + left->getRank());
    } else {
        node->setRank(1 + left->getRank() + right->getRank());
    }
}
void updateSumGrade(Node* node) {
    Node* left = node->getLeft();
    Node* right = node->getRight();
    if (left == NULL && right == NULL) {
        node->setSumGrade(node->getValue()->getGrade());
    } else if (left == NULL) {
        node->setSumGrade(node->getValue()->getGrade() + right->getSumGrade());
    } else if (right == NULL) {
        node->setSumGrade(node->getValue()->getGrade() + left->getSumGrade());
    } else {
        node->setSumGrade(node->getValue()->getGrade() + left->getSumGrade() + right->getSumGrade());
    }
}
void updateRanksFrom(Node* n) {
    if (n != NULL) {
        updateRanksFrom(n->getLeft());
        updateRanksFrom(n->getRight());
        updateRank(n);
    }
}

void updateSumGradesFrom(Node* n) {
    if (n != NULL) {
        updateSumGradesFrom(n->getLeft());
        updateSumGradesFrom(n->getRight());
        updateSumGrade(n);
    }
}
Node* LR(Node* A) {
    Node* B = A->getLeft();
    Node* C = B->getRight();
    Node* w = C->getRight();
    Node* y = C->getLeft();

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

    // Heights
    A->setHeight(getMaxHeight(A->getLeft(), A->getRight()) + 1);
    B->setHeight(getMaxHeight(B->getLeft(), B->getRight()) + 1);
    C->setHeight(getMaxHeight(C->getLeft(), C->getRight()) + 1);

    // Ranks
    updateRank(A);
    updateRank(B);
    updateRank(C);

    // SumGrade
    updateSumGrade(A);
    updateSumGrade(B);
    updateSumGrade(C);

    return C;
}

Node* LL(Node* A) {
    Node* B = A->getLeft();
    Node* y = B->getRight();

    B->setRight(A);
    A->setLeft(y);

    B->setFather(A->getFather());
    A->setFather(B);
    if (y != NULL) {
        y->setFather(A);
    }
    // Height
    A->setHeight(getMaxHeight(A->getLeft(), A->getRight()) + 1);
    B->setHeight(getMaxHeight(B->getLeft(), B->getRight()) + 1);

    // Rank
    updateRank(A);
    updateRank(B);

    // SumGrade
    updateSumGrade(A);
    updateSumGrade(B);
    return B;
}

Node* RL(Node* A) {
    Node* B = A->getRight();
    Node* C = B->getLeft();
    Node* w = C->getRight();
    Node* y = C->getLeft();

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

    // Height
    A->setHeight(getMaxHeight(A->getLeft(), A->getRight()) + 1);
    B->setHeight(getMaxHeight(B->getLeft(), B->getRight()) + 1);
    C->setHeight(getMaxHeight(C->getLeft(), C->getRight()) + 1);

    // Rank
    updateRank(A);
    updateRank(B);
    updateRank(C);

    // SumGrade
    updateSumGrade(A);
    updateSumGrade(B);
    updateSumGrade(C);
    return C;
}

Node* RR(Node* A) {
    Node* B = A->getRight();
    Node* x = B->getLeft();

    B->setLeft(A);
    A->setRight(x);

    B->setFather(A->getFather());
    A->setFather(B);
    if (x != NULL) {
        x->setFather(A);
    }

    // Height
    A->setHeight(getMaxHeight(A->getLeft(), A->getRight()) + 1);
    B->setHeight(getMaxHeight(B->getLeft(), B->getRight()) + 1);

    // Rank
    updateRank(A);
    updateRank(B);

    // SumGrade
    updateSumGrade(A);
    updateSumGrade(B);
    return B;
}

int getBalanceFactor(Node* node) {
    if (node->getHeight() == 0) return 0;
    if (node->getLeft() != NULL && node->getRight() != NULL) {
        return node->getLeft()->getHeight() - node->getRight()->getHeight();
    } else if (node->getLeft() == NULL) {
        return (-1) - node->getRight()->getHeight();
    } else
        return node->getLeft()->getHeight() - (-1);
}

Node* EmployeesTreeFromSortedArray(shared_ptr<Employee>* arr, int start, int end) {
    if (start > end) return NULL;
    int mid = (start + end) / 2;
    Node* root = new Node(arr[mid]);
    root->setLeft(EmployeesTreeFromSortedArray(arr, start, mid - 1));
    root->setRight(EmployeesTreeFromSortedArray(arr, mid + 1, end));
    root->setHeight(getMaxHeight(root->getLeft(), root->getRight()) + 1);
    return root;
}

class EmployeesTree : public SearchTree {
   private:
    bool checkIfRollNeeded(Node* p) {
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
        if (didRoll == 1) {                    // 3.4
            Node* nodeAbove = p->getFather();  // fixing the node before p
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
    EmployeesTree() : SearchTree() {}
    EmployeesTree(shared_ptr<Employee>* arr, int count) : SearchTree(count) {
        this->root = EmployeesTreeFromSortedArray(arr, 0, count - 1);
    };

    int getHeight() const { return this->root->getHeight(); }
    void insert(shared_ptr<Employee> value) override {                     // Using the algorithm from Erez presentation
        SearchTree::insert(value);                                         // 1+2
        Node* v = SearchTree::search(value);                               // 1
        while ((this->cmp)(v->getValue(), this->root->getValue()) != 0) {  // 3 comparing using elements's != operartor
            Node* p = v->getFather();                                      // 3.1
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

    void remove(shared_ptr<Employee> value) override {  // using algorithm from Rami's presentation
        Node* v = SearchTree::search(value);
        if (v == NULL) {
            // throw DoesNotExists();
            return;
        }
        Node* p;
        if (v->getLeft() && v->getRight()) {  // 2 sons
            p = getMinFrom(v->getRight())->getFather();
        } else {
            p = v->getFather();
        }
        SearchTree::remove(value);  // 1 removing v
        v = p;
        while (v != NULL && (this->cmp)(v->getValue(), this->root->getValue()) != 0) {
            p = v->getFather();
            int prevHeight = p->getHeight();                              // 3.1
            v->setHeight(getMaxHeight(v->getLeft(), v->getRight()) + 1);  // 3.2
            updateRank(v);
            updateSumGrade(v);
            if (prevHeight == p->getHeight()) {
                return;
            }
            v = p;
        }
    }
    void updateRanks() { updateRanksFrom(root); }
    void updateSumGrades() { updateSumGradesFrom(root); }

    void getSumGradeByIndex(int i, long long int* sum) { return getSumGradeByIndexFrom(i, sum, root); }
    void getSumGradeByIndexFrom(int i, long long int* sum, Node* n) {
        int currentRank;
        if (n == NULL) {
            return;
        }
        if (n->getLeft() == NULL) {
            currentRank = 1;
        } else {
            currentRank = n->getLeft()->getRank() + 1;
        }
        if (currentRank == i) {  // found
            if (n->getRight() != NULL) {
                *sum += n->getRight()->getSumGrade();
            }
            return;
        }
        if (currentRank > i) {  //  down the left subtree
            if (n->getRight() == NULL) {
                *sum += n->getValue()->getGrade();
            } else {
                *sum += n->getRight()->getSumGrade() + n->getValue()->getGrade();
            }
            getSumGradeByIndexFrom(i, sum, n->getLeft());
        } else {  // down the right tree
            getSumGradeByIndexFrom(i - currentRank, sum, n->getRight());
        }
    }

    int getRankOfMinSalary(long long int salary) { return getRankOfMinSalaryFrom(salary, root); }
    int getRankOfMinSalaryFrom(long long int salary, Node* n) {
        if (n == NULL) {
            return 0;
        }
        if (n->getValue()->getSalary() >= salary) {  // go left
            return getRankOfMinSalaryFrom(salary, n->getLeft());

        } else {
            int nRank = 1;
            if (n->getLeft() != NULL) {
                nRank += n->getLeft()->getRank();
            }
            return nRank + getRankOfMinSalaryFrom(salary, n->getRight());
        };
    }
    int getRankOfMaxSalary(long long int salary) { return getRankOfMaxSalaryFrom(salary, root); }
    int getRankOfMaxSalaryFrom(long long int salary, Node* n) {
        if (n == NULL) {
            return 0;
        }
        if (n->getValue()->getSalary() <= salary) {  // go right
            int nRank = 1;
            if (n->getLeft() != NULL) {
                nRank += n->getLeft()->getRank();
            }
            return nRank + getRankOfMaxSalaryFrom(salary, n->getRight());
        } else {
            return getRankOfMaxSalaryFrom(salary, n->getLeft());
        };
    }
};
#endif  // EMPLOYEES_TREE_H