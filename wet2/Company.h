#ifndef COMPANY_H
#define COMPANY_H

#include "Employee.h"
#include "EmployeesHT.h"
#include "EmployeesTree.h"

class Company {
   private:
    int id;
    EmployeesTree paidEmployees;
    EmployeesHT allEmployees;

   public:
    Company(int id) : id(id), paidEmployees(EmployeesTree()), allEmployees(EmployeesHT(10)){};
    ~Company() = default;
    int getID() { return id; }
    EmployeesTree *getPaidEmployees() { return &paidEmployees; }
    EmployeesHT *getAllEmployees() { return &allEmployees; }
    void removeEmployee(shared_ptr<Employee> e) {
        if (e->getSalary() > 0) {
            paidEmployees.remove(e);
        }
        if (e->getSalary() == 0) {
            allEmployees.updateInternSumGrades(-1 * e->getGrade());
        }
        allEmployees.remove(e);
    }
    void addEmployee(shared_ptr<Employee> e) {
        allEmployees.insert(e);
        if (e->getSalary() == 0) {
            allEmployees.updateInternSumGrades(+e->getGrade());
        }
    }
    void setPaidEmployees(shared_ptr<Employee> *array, int size) { paidEmployees = EmployeesTree(array, size); }
    void deleteComp(){
        paidEmployees.emptyTree();
        allEmployees.empty();
        
    }
};

void mergeArrays(shared_ptr<Employee> *array1, shared_ptr<Employee> *array2, int l1, int l2, shared_ptr<Employee> *mergedArray) {
    int i = 0, j = 0, k = 0;
    Cmp cmp = Cmp();
    while (i < l1 && j < l2) {
        if (cmp(array1[i], array2[j]) < 0)
            mergedArray[k++] = array1[i++];
        else
            mergedArray[k++] = array2[j++];
    }
    while (i < l1) {
        mergedArray[k] = array1[i];
        i++;
        k++;
    }

    while (j < l2)
        mergedArray[k++] = array2[j++];
}

void mergeEmployees(shared_ptr<Company> acquirer, shared_ptr<Company> target, double factor) {
    // merging trees------------------------------------------------------
    int a = acquirer->getPaidEmployees()->getCount();
    int t = target->getPaidEmployees()->getCount();
    if (t > 0) {
        // setting up arrays by ID
        shared_ptr<Employee> mergedArray[a + t];
        shared_ptr<Employee> acquirerArray[a];
        shared_ptr<Employee> targetArray[t];
        acquirer->getPaidEmployees()->getAmountInOrder(a, acquirerArray);  // inserts a elements to the array in order.
        target->getPaidEmployees()->getAmountInOrder(t, targetArray);      // inserts t elements to the array in order.

        // Updating the new employer
        for (int i = 0; i < t; i++) {
            targetArray[i]->setEmployer(acquirer);
        }

        // merging array
        mergeArrays(acquirerArray, targetArray, a, t, mergedArray);
        // building new EmployeesTree from sorted array and replacing old tree
        acquirer->setPaidEmployees(mergedArray, a + t);
        target->getPaidEmployees()->emptyTree();
        acquirer->getPaidEmployees()->updateRanks();
        acquirer->getPaidEmployees()->updateSumGrades();
    }
    // merging HT's------------------------------------------------------
    a = acquirer->getAllEmployees()->getCount();
    t = target->getAllEmployees()->getCount();
    if (t > 0) {
        acquirer->getAllEmployees()->addFromHT(target->getAllEmployees());
    }
    acquirer->getAllEmployees()->updateInternSumGrades(target->getAllEmployees()->getInternSumGrades());  // update InternSumGrades
}

#endif  // COMPANY_H