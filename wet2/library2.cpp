#include "library2.h"

#include <cmath>

#include "EmployeeManager.h"

void *Init(int k) {
    if (k <= 0) {
        return NULL;
    }
    EmployeeManager *DS = new EmployeeManager(k);
    return (void *)DS;
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade) {
    if (DS == NULL || employeeID <= 0 || companyID <= 0 || companyID > ((EmployeeManager *)DS)->getK() || grade < 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->AddEmployee(employeeID, companyID, grade);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemoveEmployee(void *DS, int employeeID) {
    if (DS == NULL || employeeID <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->RemoveEmployee(employeeID);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        // std::cerr << e.what() << '\n';
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor) {
    if (DS == NULL || factor <= 0.0 || companyID2 == companyID1 || companyID1 <= 0 || companyID2 <= 0 || companyID1 > ((EmployeeManager *)DS)->getK() || companyID2 > ((EmployeeManager *)DS)->getK()) {
        return INVALID_INPUT;
    }
    try {
        ((EmployeeManager *)DS)->AcquireCompany(companyID1, companyID2, factor);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return INVALID_INPUT;
    }
    return SUCCESS;
}

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease) {
    if (DS == NULL || employeeID <= 0 || salaryIncrease <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->EmployeeSalaryIncrease(employeeID, salaryIncrease);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade) {
    if (DS == NULL || employeeID <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->PromoteEmployee(employeeID, bumpGrade);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m) {
    if (DS == NULL || companyID < 0 || companyID > ((EmployeeManager *)DS)->getK() || m <= 0) return INVALID_INPUT;
    int res = 0;
    try {
        res = ((EmployeeManager *)DS)->SumOfBumpGradeBetweenTopWorkersByGroup(companyID, m);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    printf("SumOfBumpGradeBetweenTopWorkersByGroup: %d\n", res);
    return SUCCESS;
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary) {
    if (DS == NULL || higherSalary < 0 || lowerSalary < 0 || lowerSalary > higherSalary || companyID < 0 || companyID > ((EmployeeManager *)DS)->getK()) return INVALID_INPUT;
    try {
        double average = ((EmployeeManager *)DS)->AverageBumpGradeBetweenSalaryByGroup(companyID, lowerSalary, higherSalary);
        printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", average);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType CompanyValue(void *DS, int companyID) {
    double standing;
    if (DS == NULL || companyID <= 0 || companyID > ((EmployeeManager *)DS)->getK()) return INVALID_INPUT;
    try {
        standing = ((EmployeeManager *)DS)->CompanyValue(companyID);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    printf("CompanyValue: %.1f\n", standing);
    return SUCCESS;
}

// StatusType BumpGradeToEmployees(void *DS, int lowerSalary, int higherSalary, int bumpGrade) { return SUCCESS; }

void Quit(void **DS) {
    delete (EmployeeManager *)*DS;
    *DS = NULL;
};
