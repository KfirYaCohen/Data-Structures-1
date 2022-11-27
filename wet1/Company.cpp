#include "Company.h"

int Company::getId() const { return this->id; }
int Company::getValue() const { return this->value; }
void Company::increaseValue(int amount) { this->value += amount; }
void Company::setValue(int value) { this->value = value; }
shared_ptr<Employee> Company::getHighestEarner() const { return this->highestEarner; }
shared_ptr<Employee> Company::getEmployeeByID(int id) const {
    shared_ptr<Employee> toSearch = make_shared<Employee>(id);
    return ((this->employeesByID).search(toSearch))->getValue();
}
// shared_ptr<Employee> Company::getEmployeeBySalary(int id,int salary) const{
//     return this->employeesByID.search(Employee(id,salary));

// }
// shared_ptr<Employee> Company::getHighestEarner(int id) const{

// }
int Company::getSize() const { return employeesByID.getCount(); }
void Company::addEmployee(shared_ptr<Employee> emp) {
    this->employeesByID.insert(emp);
    this->employeesBySalary.insert(emp);
}
void Company::removeEmployee(shared_ptr<Employee> emp) {
    this->employeesByID.remove(emp);
    this->employeesBySalary.remove(emp);
}
void Company::removeEmployeeById(int id) {
    shared_ptr<Employee> emp = this->getEmployeeByID(id);
    this->removeEmployee(emp);
}
void Company::updateHighestEarner() {
    if (getSize() != 0) {
        highestEarner = employeesBySalary.getMaxValue();
    } else
        highestEarner = NULL;
}

void Company::getEmployeesInOrderBySalary(int amount, shared_ptr<Employee> *array) { employeesBySalary.getAmountInOrder(amount, array); }
void Company::getEmployeesInOrderById(int amount, shared_ptr<Employee> *array) { employeesByID.getAmountInOrder(amount, array); }
void Company::replaceTrees(shared_ptr<Employee> *mergedArrayByID, shared_ptr<Employee> *mergedArrayBySalary, int size) {
    employeesByID = AVLTree<shared_ptr<Employee>, EmployeeCmpById>(EmployeeCmpById(), mergedArrayByID, size);
    employeesBySalary = AVLTree<shared_ptr<Employee>, EmployeeCmpBySalary>(EmployeeCmpBySalary(), mergedArrayBySalary, size);
}

void Company::emptyComapny() {
    employeesByID.emptyTree();
    employeesBySalary.emptyTree();
}