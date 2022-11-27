#ifndef EMPLOYEES_HT_H
#define EMPLOYEES_HT_H

#include <iostream>
#include <memory>

#include "Employee.h"
#include "List.h"
#include "exceptions.h"

using std::make_shared;
using std::shared_ptr;

class EmployeesHT {
   private:
    int size;
    int count;
    int internSumGrades;
    shared_ptr<List> *data;

   public:
    EmployeesHT(int size) : size(size), count(0),internSumGrades(0), data(new shared_ptr<List>[size]) {
        for (int i = 0; i < size; i++) {
            data[i] = NULL;
        }
    }
    ~EmployeesHT() {
        delete[] data;
    }
    void empty() {
        for (int i = 0; i < size; i++) {
            if (data[i] != NULL) {
                data[i]->empty();
                data[i].reset();
            }
        }
    }
    void insert(shared_ptr<Employee> e) {
        shared_ptr<List> list = data[hashFunction(e->getId())];
        if (list == NULL) {
            list = make_shared<List>();
            data[hashFunction(e->getId())] = list;
        }
        if (list->find(e->getId()) != NULL) {
            throw AlreadyExists();
        }
        list->insert(e);
        count++;
        reSize(2);
    }
    int getInternSumGrades() {
        return internSumGrades;
    }
    void updateInternSumGrades(int num) {
        internSumGrades += num;
    }
    int getCount() {
        return count;
    }
    void remove(shared_ptr<Employee> e) {
        shared_ptr<List> list = data[hashFunction(e->getId())];
        if (list == NULL || list->find(e->getId()) == NULL) {
            throw DoesNotExists();
        }
        list->remove(e);
        count--;
        reSize(2);
    }

    shared_ptr<Employee> find(int id) {
        if (data[hashFunction(id)] == NULL || data[hashFunction(id)]->find(id) == NULL) {
            throw DoesNotExists();
            return NULL;
        }
        return data[hashFunction(id)]->find(id);
    }

    void reSize(int factor) {
        if (count >= size) {
            size = factor * size;
            shared_ptr<List> *newData = new shared_ptr<List>[size];
            for (int i = 0; i < size; i++) {
                newData[i] = NULL;
            }
            copyEmployees(newData, data, size / factor);
            for (int i = 0; i < size / factor; i++) {
                if (data[i] != NULL) {
                    data[i].reset();
                }
            }
            delete[] data;
            data = newData;
        } else if (this->count <= (this->size / 4) && this->size > 10) {
            size = size / factor;
            shared_ptr<List> *newData = new shared_ptr<List>[size];
            for (int i = 0; i < size; i++) {
                newData[i] = NULL;
            }
            copyEmployees(newData, data, size * factor);
            for (int i = 0; i < size * factor; i++) {
                if (data[i] != NULL) {
                    data[i].reset();
                }
            }
            delete[] data;
            data = newData;
        }
    }

    void copyEmployees(shared_ptr<List> *newData, shared_ptr<List> *oldData, int oldSize) {
        for (int i = 0; i < oldSize; i++) {
            while (oldData[i] != NULL && oldData[i]->getHead() != NULL) {
                int id = oldData[i]->getHead()->getEmp()->getId();
                shared_ptr<List> list = newData[hashFunction(id)];
                if (list == NULL) {
                    list = make_shared<List>();
                    newData[hashFunction(id)] = list;
                }
                list->insert(oldData[i]->pop());
                if (oldData[i]->getHead() == NULL) {
                    oldData[i].reset();
                    oldData[i] = NULL;
                }
            }
        }
    }
    int hashFunction(int i) { return i % size; }

    void addFromHT(EmployeesHT *other) {
        count += other->getCount();
        int factor = 1;
        while (count > size * factor) {
            factor = factor * 2;
        }
        if (factor > 1) {
            reSize(factor);
        }

        copyEmployees(this->data, other->data, other->size);
    }

    void updateEmployer(shared_ptr<Company> cmp) {
        for (int i = 0; i < size; i++) {
            if (data[i] != NULL) {
                Link *current = data[i]->getHead();
                while (current != NULL) {
                    current->getEmp()->setEmployer(cmp);
                    current = current->getNext();
                }
            }
        }
    }

    void print() {
        int num = size - 1;
        while (num >= 0) {
            std::cout << "Cell: " << num << "   ";
            if (data[num] == NULL) {
                std::cout << "Null";
            } else {
                data[num]->print();
            };
            std::cout << std::endl;
            num--;
        }
    }
};

#endif  // EMPLOYEES_HT_H