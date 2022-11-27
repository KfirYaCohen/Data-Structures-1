#include "Employee.h"

int Employee::getId() const { return this->id; }
long long int Employee::getSalary() const { return this->salary; }
long long int Employee::getGrade() const { return this->grade; }
void Employee::increaseSalary(int amount) { this->salary += amount; }
shared_ptr<Company> Employee::getEmployer() const { return this->employer; }
void Employee::setSalary(long long int s) { salary = s; }
void Employee::setGrade(long long int s) { grade = s; }
void Employee::setEmployer(shared_ptr<Company> comp) { this->employer = comp; }