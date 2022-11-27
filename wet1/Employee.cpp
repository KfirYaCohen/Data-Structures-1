#include "Employee.h"

int Employee::getId() const { return this->id; }
int Employee::getSalary() const { return this->salary; }
int Employee::getGrade() const { return this->grade; }
void Employee::increaseSalary(int amount) { this->salary += amount; }
void Employee::bumpGrade() { this->grade++; }
shared_ptr<Company> Employee::getEmployer() const { return this->employer; }
void Employee::setSalary(int s) { salary = s; }
void Employee::setGrade(int s) { grade = s; }
void Employee::setEmployer(shared_ptr<Company> comp) { this->employer = comp; }