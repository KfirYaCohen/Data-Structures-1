#ifndef COMPANIES_H
#define COMPANIES_H

#include <memory>

#include "Company.h"
#include "UFNode.h"

using std::make_shared;
using std::shared_ptr;

class Companies {
   private:
    int size;
    UFNode* array;

   public:
    Companies(int k) : size(k), array(new UFNode[k + 1]) {
        array[0] = UFNode();
    };
    ~Companies() {
        delete[] array;
    }

    void makeSet(int i) { array[i] = UFNode(make_shared<Company>(i)); };

    int getSize() {
        return size;
    }
    UFNode* find(int i) {
        UFNode* current = &array[i];
        UFNode* root = NULL;
        long double sum = 0;
        while (current->getFather() != NULL) {
            sum += current->getR();
            current = current->getFather();
        }

        root = current;
        current = &array[i];
        UFNode* temp = NULL;
        while (current->getFather() != NULL) {
            temp = current->getFather();
            current->setFather(root);
            long double tempR = current->getR();
            current->setR(sum);
            sum -= tempR;
            current = temp;
        }

        return root;
    };

    void acquire(UFNode* a, UFNode* t, double factor, long double tValue) {
        if ((a->getCompany())->getID() == (t->getCompany())->getID()) {
            return;
        }
        if (a->getSize() > t->getSize()) {  // normal case
            t->setFather(a);
            a->increaseSize(t->getSize());
            t->setMaster(NULL);  // optional
            t->setSize(-1);      // optional

            a->setR(a->getR() + factor * tValue);
            t->setR(t->getR() - a->getR());

        } else {  // not normal case
            a->setFather(t);
            t->setMaster(a->getMaster());
            t->increaseSize(t->getSize());
            a->setSize(-1);      // optional
            a->setMaster(NULL);  // optional

            a->setR(a->getR() + factor * tValue - t->getR());
        }
    }
    long double sumR(int companyID) {
        UFNode* current = &array[companyID];
        long double sum = current->getR();

        while (current->getFather() != NULL) {
            current = current->getFather();
            sum += current->getR();
        }
        return sum;
    }
    void empty() {
        for (int i = 1; i <= size; i++) {
            array[i].getCompany()->deleteComp();
            array[i].empty();
        }
    }
};

#endif  // COMPANIES_H
