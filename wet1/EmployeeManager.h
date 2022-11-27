#ifndef EMPLOYEE_MANAGER_H
#define EMPLOYEE_MANAGER_H

#include <stdlib.h>

#include "AVLTree.h"
#include "Company.h"
#include "Employee.h"
#include "exceptions.h"

using std::shared_ptr;


class CompanyCmpById {
   private:
   public:
    int operator()(shared_ptr<Company> c1, shared_ptr<Company> c2) { return (c1->getId() - c2->getId()); }
};

class EmployeeManager {
   private:
    AVLTree<shared_ptr<Company>, CompanyCmpById> Companies;
    AVLTree<shared_ptr<Company>, CompanyCmpById> NonEmptyCompanies;
    AVLTree<shared_ptr<Employee>, EmployeeCmpById> EmployeesByID;
    AVLTree<shared_ptr<Employee>, EmployeeCmpBySalary> EmployeesBySalary;
    shared_ptr<Employee> recordEarner;

   public:
    EmployeeManager();
    ~EmployeeManager();
    void AddCompany(int CompanyID, int Value);
    void AddEmployee(int EmployeeID, int CompanyID, int Salary, int Grade);
    void RemoveCompany(int CompanyID);
    void RemoveEmployee(int EmployeeID);
    void GetCompanyInfo(int CompanyID, int *Value, int *NumEmployees);               
    void GetEmployeeInfo(int EmployeeID, int *EmployerID, int *Salary, int *Grade);  
    void IncreaseCompanyValue(int CompanyID, int ValueIncrease);
    void PromoteEmployee(int EmployeeID, int SalaryIncrease, int BumpGrade);
    void HireEmployee(int EmployeeID, int NewCompanyID);
    void AcquireCompany(int AcquirerID, int TargetID, double Factor);
    void GetHighestEarner(int CompanyID, int *EmployeeID);
    void GetAllEmployeesBySalary(int CompanyID, int **Employees, int *NumOfEmployees);
    void GetHighestEarnerInEachCompany(int NumOfCompanies, int **Employees);
    void GetNumEmployeesMatching(int CompanyID, int MinEmployeeID, int MaxEmployeeId, int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees);
    void updateRecordEarner();
    shared_ptr<Company> searchCompany(int id);
    shared_ptr<Employee> searchEmployee(int id);
    void Quit();
};
#endif  // EMPLOYEE_MANAGER_H