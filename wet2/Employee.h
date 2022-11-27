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
    long long int salary;
    long long int grade;

   public:
    Employee(int id, shared_ptr<Company> employer, int grade) : id(id), employer(employer), salary(0), grade(grade) {}
    Employee(int id) : id(id) {}
    Employee(int id, int salary) : id(id), salary(salary) {}
    ~Employee() { employer.reset(); }
    int getId() const;
    long long int getSalary() const;
    long long int getGrade() const;
    shared_ptr<Company> getEmployer() const;
    bool operator==(shared_ptr<Employee> e1) { return e1->getId() == id; }
    void setSalary(long long int s);
    void increaseSalary(int amount);
    void setGrade(long long int s);
    // void bumpGrade();
    void setEmployer(shared_ptr<Company> comp);
    void resetEmployer() {
        if (employer != NULL) {
            employer.reset();
            employer = NULL;
        }
    }
};

#endif  // EMPLOYEE_H