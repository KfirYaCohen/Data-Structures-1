
#include "Companies.h"
#include "EmployeesHT.h"
#include "EmployeesTree.h"

using std::make_shared;
using std::shared_ptr;

class EmployeeManager {
   private:
    Companies companies;
    EmployeesTree employeesTree;
    EmployeesHT employees;

   public:
    EmployeeManager(int k) : companies(Companies(k)), employeesTree(EmployeesTree()), employees(EmployeesHT(10)) {
        for (int i = 1; i <= k; i++) {
            companies.makeSet(i);
        }
    };
    ~EmployeeManager() {
        companies.empty();
        employees.empty();
    };

    int getK() { return companies.getSize(); }

    void AddEmployee(int employeeID, int companyID, int grade) {
        shared_ptr<Company> comp = (companies.find(companyID))->getMaster();
        shared_ptr<Employee> emp = make_shared<Employee>(employeeID, comp, grade);
        employees.insert(emp);
        employees.updateInternSumGrades(+grade);
        comp->addEmployee(emp);
    }

    void RemoveEmployee(int employeeID) {
        shared_ptr<Employee> emp = employees.find(employeeID);
        employees.remove(emp); 
        if (emp->getSalary() == 0) {
            employees.updateInternSumGrades(-1 * emp->getGrade());
        }
        shared_ptr<Company> comp = emp->getEmployer();
        comp->removeEmployee(emp);
        if (emp->getSalary() > 0) {
            employeesTree.remove(emp);
        }
        emp->setEmployer(NULL);
    }

    void AcquireCompany(int companyID1, int companyID2, double factor) {
        UFNode *a = companies.find(companyID1);
        UFNode *t = companies.find(companyID2);

        shared_ptr<Company> aMother = a->getMaster();
        shared_ptr<Company> tMother = t->getMaster();

        if (aMother->getID() == tMother->getID()) {
            throw BadArgument();
        }
        long double tValue = CompanyValue(tMother->getID());
        companies.acquire(a, t, factor, tValue);
        mergeEmployees(aMother, tMother, factor);
        aMother->getAllEmployees()->updateEmployer(aMother);
    }

    void EmployeeSalaryIncrease(int employeeID, int salaryIncrease) {
        shared_ptr<Employee> emp = employees.find(employeeID);
        long long int oldSalary = emp->getSalary();
        if (oldSalary > 0) {
            employeesTree.remove(emp);
            emp->getEmployer()->getPaidEmployees()->remove(emp);
        }
        emp->setSalary(oldSalary + salaryIncrease);
        emp->getEmployer()->getPaidEmployees()->insert(emp);
        employeesTree.insert(emp);
        if (oldSalary == 0) {  // updating sumgrade of interns
            employees.updateInternSumGrades(-1 * emp->getGrade());
            emp->getEmployer()->getAllEmployees()->updateInternSumGrades(-1 * emp->getGrade());
        }
    }

    void PromoteEmployee(int employeeID, int bumpGrade) {
        shared_ptr<Employee> emp = employees.find(employeeID);
        if (bumpGrade > 0) {
            long long int oldGrade = emp->getGrade();
            if (emp->getSalary() > 0) {
                employeesTree.remove(emp);
                emp->getEmployer()->getPaidEmployees()->remove(emp);
                emp->setGrade(oldGrade + bumpGrade);
                emp->getEmployer()->getPaidEmployees()->insert(emp);
                employeesTree.insert(emp);
            } else {
                emp->setGrade(oldGrade + bumpGrade);
                employees.updateInternSumGrades(+bumpGrade);
                emp->getEmployer()->getAllEmployees()->updateInternSumGrades(+bumpGrade);
            }
        }
    }

    int SumOfBumpGradeBetweenTopWorkersByGroup(int companyID, int m) {
        long long int sum = 0;
        if (companyID > 0) {
            shared_ptr<Company> comp = companies.find(companyID)->getMaster();
            EmployeesTree *paidEmployees = comp->getPaidEmployees();
            if (paidEmployees->getCount() < m) {
                throw BadArgument();
            }
            paidEmployees->getSumGradeByIndex(paidEmployees->getCount() - m, &sum);
        } else {
            if (employeesTree.getCount() < m) {
                throw BadArgument();
            }
            employeesTree.getSumGradeByIndex(employeesTree.getCount() - m, &sum);
        }
        return sum;
    }

    double AverageBumpGradeBetweenSalaryByGroup(int companyID, int lowerSalary, int higherSalary) {
        long long int sum1 = 0;
        long long int sum2 = 0;
        long double totalSum = 0;
        int numOfEmployees = 0;
        int minRank = 0;
        int maxRank = 0;
        int size = 0;
        long double avg = 0;
        if (companyID > 0) {
            shared_ptr<Company> comp = companies.find(companyID)->getMaster();
            if (comp->getAllEmployees()->getCount() == 0) {
                throw DoesNotExists();
            }
            size = comp->getPaidEmployees()->getCount();
            minRank = comp->getPaidEmployees()->getRankOfMinSalary(lowerSalary);
            maxRank = comp->getPaidEmployees()->getRankOfMaxSalary(higherSalary);
            if (lowerSalary == 0) {
                totalSum += comp->getAllEmployees()->getInternSumGrades();
                numOfEmployees += comp->getAllEmployees()->getCount() - size;
            }
        } else {
            if (employees.getCount() == 0) {
                throw DoesNotExists();
            }
            size = employeesTree.getCount();
            minRank = employeesTree.getRankOfMinSalary(lowerSalary);
            maxRank = employeesTree.getRankOfMaxSalary(higherSalary);

            if (lowerSalary == 0) {
                totalSum += employees.getInternSumGrades();
                numOfEmployees += employees.getCount() - size;
            }
        }
        sum1 = SumOfBumpGradeBetweenTopWorkersByGroup(companyID, size - minRank);
        sum2 = SumOfBumpGradeBetweenTopWorkersByGroup(companyID, size - maxRank);
        totalSum += sum1 - sum2;
        numOfEmployees += maxRank - minRank;
        if (numOfEmployees == 0) {
            throw DoesNotExists();
        }
        avg = totalSum / numOfEmployees;

        return avg;
    }
    void print() {
        employees.print();
        employeesTree.print();
    }

    double CompanyValue(int companyID) {
        return companies.sumR(companyID);
    }
};
