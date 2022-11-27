#ifndef _LIST_H
#define _LIST_H

#include <iostream>
#include <memory>

#include "Company.h"
#include "Employee.h"

using std::make_shared;
using std::shared_ptr;

class Link {
   private:
    shared_ptr<Employee> emp;
    Link *next;

   public:
    Link(shared_ptr<Employee> e, Link *next = NULL) : emp(e), next(next) {}
    ~Link() {
        emp.reset();
    }
    Link *getNext() { return next; }
    shared_ptr<Employee> getEmp() { return emp; }
    void setNext(Link *next) { this->next = next; }
    void empty(){
        emp.reset();
    }
};

class List {
   private:
    Link *head;

   public:
    List() : head(NULL) {}
    ~List() = default;
    void empty() {
        Link *node = head;
        while (node != NULL) {
            Link *nodeToDelete = node;
            node = node->getNext();
            nodeToDelete->empty();
            delete nodeToDelete;
        }
        head = NULL;
    }
    shared_ptr<Employee> find(int id) {
        Link *current = head;
        while (current != NULL) {
            if (current->getEmp()->getId() == id) {
                return current->getEmp();
            }
            current = current->getNext();
        }
        return NULL;  // not found
    };
    void insert(shared_ptr<Employee> emp) {
        Link *newLink = new Link(emp);
        newLink->setNext(head);
        head = newLink;
    }
    void remove(shared_ptr<Employee> emp) {
        Link *current = head;

        if (head->getEmp()->getId() == emp->getId()) {  // deleting head
            head = head->getNext();
            delete current;
        } else {
            while (current->getNext() != NULL && current->getNext()->getEmp()->getId() != emp->getId()) {
                current = current->getNext();
            }
            if (current->getNext() != NULL) {
                Link *toDelete = current->getNext();
                current->setNext(current->getNext()->getNext());
                delete toDelete;
            }
        }
    }
    Link *getHead() {
        return head;
    }
    shared_ptr<Employee> top() {
        if (head != NULL) {
            return head->getEmp();
        }
        return NULL;
    }
    shared_ptr<Employee> pop() {
        if (head != NULL) {
            shared_ptr<Employee> e = head->getEmp();
            remove(e);
            return e;
        }
        return NULL;
    }
    void print() {
        Link *current = head;
        // && current->getEmp() != NULL
        while (current != NULL && current->getEmp() != NULL) {
            std::cout << current->getEmp()->getId() << " ";
            // std::cout << current->getEmp()->getId() << " " <<"(" << current->getEmp()->getEmployer()->getID() << ") " ;
            std::cout << "-->"
                      << " ";
            current = current->getNext();
        }
        std::cout << " END";
    };
};

#endif