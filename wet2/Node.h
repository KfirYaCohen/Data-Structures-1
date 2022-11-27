#ifndef NODE_H
#define NODE_H

#include "exceptions.h"
#include "Employee.h"

class Node {
   private:
    shared_ptr<Employee> value;
    Node *left;
    Node *right;
    Node *father;
    int height;
    int sumGrade;
    int rank;

   public:
    Node(shared_ptr<Employee> value) : value(value), left(NULL), right(NULL), father(NULL), height(0),sumGrade(value->getGrade()), rank(1){};
    ~Node() {
        value.reset();
    }
    Node *getLeft() const { return this->left; }
    Node *getRight() const { return this->right; }
    Node *getFather() const { return this->father; }
    int getHeight() const { return this->height; }
    int getSumGrade() const { return this->sumGrade; }
    int getRank() const { return this->rank; }
    shared_ptr<Employee> getValue() const { return this->value; }
    void setLeft(Node *left) { this->left = left; }
    void setRight(Node *right) { this->right = right; }
    void setFather(Node *father) { this->father = father; }
    void setHeight(int n) { this->height = n; }
    void setValue(shared_ptr<Employee> value) { this->value = value; }
    void setSumGrade(int sumGrade) { this->sumGrade = sumGrade; }
    void increaseSumGrade(int sumGrade) { this->sumGrade += sumGrade; }
    void decreaseSumGrade(int sumGrade) { this->sumGrade -= sumGrade; }
    void setRank(int rank) { this->rank = rank;}
    void increaseRank() { this->rank++;}
    void decreaseRank() { this->rank--;}
    
};

#endif  // NODE_H