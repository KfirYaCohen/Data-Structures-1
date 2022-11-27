#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <memory>

//#include "Company.h"
using std::shared_ptr;
class Company;  // Forward declaration

class Employee {
   private:
    int id;
    shared_ptr<Company> employer;
    int salary;
    int grade;

   public:
    Employee(int id, shared_ptr<Company> employer, int salary, int grade) : id(id), employer(employer), salary(salary), grade(grade) {}
    Employee(int id) : id(id) {}
    Employee(int id, int salary) : id(id), salary(salary) {}
    int getId() const;
    int getSalary() const;
    int getGrade() const;
    shared_ptr<Company> getEmployer() const;

    void setSalary(int s);
    void increaseSalary(int amount);
    void setGrade(int s);
    void bumpGrade();
    void setEmployer(shared_ptr<Company> comp);
};

#endif  // EMPLOYEE_H