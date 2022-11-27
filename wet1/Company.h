#ifndef COMPANY_H
#define COMPANY_H
#include <memory>

#include "AVLTree.h"
#include "Employee.h"

// class Employee;  // Forward declaration

using std::make_shared;
using std::shared_ptr;

class EmployeeCmpById {
   private:
   public:
    int operator()(shared_ptr<Employee> e1, shared_ptr<Employee> e2) { return (e1->getId() - e2->getId()); }
};
class EmployeeCmpBySalary {
   private:
   public:
    int operator()(shared_ptr<Employee> e1, shared_ptr<Employee> e2) {
        if (e1->getSalary() == e2->getSalary()) {
            return (e2->getId() - e1->getId());
        } else {
            return e1->getSalary() - e2->getSalary();
        }
    }
};

class Company {
   private:
    int id;
    int value;
    AVLTree<shared_ptr<Employee>, EmployeeCmpById> employeesByID;
    AVLTree<shared_ptr<Employee>, EmployeeCmpBySalary> employeesBySalary;
    shared_ptr<Employee> highestEarner;

   public:
    Company(int id, int val = 0)
        : id(id),
          value(val),
          employeesByID(AVLTree<shared_ptr<Employee>, EmployeeCmpById>(EmployeeCmpById())),
          employeesBySalary(AVLTree<shared_ptr<Employee>, EmployeeCmpBySalary>(EmployeeCmpBySalary())),
          highestEarner(NULL) {}
    int getId() const;
    int getValue() const;
    int getSize() const;
    void increaseValue(int amount);
    void setValue(int value);
    shared_ptr<Employee> getHighestEarner() const;
    shared_ptr<Employee> getEmployeeByID(int id) const;
    // shared_ptr<Employee> getEmployeeBySalary(int id) const;
    // shared_ptr<Employee> getHighestEarner(int id) const;
    void addEmployee(shared_ptr<Employee> emp);
    void removeEmployee(shared_ptr<Employee> emp);
    void removeEmployeeById(int id);
    void updateHighestEarner();
    void getEmployeesInOrderBySalary(int amount, shared_ptr<Employee> *array);
    void getEmployeesInOrderById(int amount, shared_ptr<Employee> *array);

    template <class Cond>
    int getEmployeesMathchingInRange(Cond cond) {
        return employeesByID.getAmountInRange(cond);
    }
    void replaceTrees(shared_ptr<Employee> *mergedArrayByID, shared_ptr<Employee> *mergedArrayBySalary, int size);
    void emptyComapny();
    void printByID() { employeesByID.print(); }
    void empty() {
        employeesByID.emptyTree();
        employeesBySalary.emptyTree();
        highestEarner = NULL;
    }
};

#endif  // COMPANY_H