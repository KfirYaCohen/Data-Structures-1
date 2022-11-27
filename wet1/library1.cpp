#include "library1.h"

#include "EmployeeManager.h"

void *Init() {
    EmployeeManager *DS = new EmployeeManager();
    return (void *)DS;
}
StatusType AddCompany(void *DS, int CompanyID, int Value) {
    if (DS == NULL || CompanyID <= 0 || Value <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->AddCompany(CompanyID, Value);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AddEmployee(void *DS, int EmployeeID, int CompanyID, int Salary, int Grade) {
    if (DS == NULL || EmployeeID <= 0 || CompanyID <= 0 || Salary <= 0 || Grade < 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->AddEmployee(EmployeeID, CompanyID, Salary, Grade);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemoveCompany(void *DS, int CompanyID) {
    if (DS == NULL || CompanyID <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->RemoveCompany(CompanyID);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemoveEmployee(void *DS, int EmployeeID) {
    if (DS == NULL || EmployeeID <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->RemoveEmployee(EmployeeID);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetCompanyInfo(void *DS, int CompanyID, int *Value, int *NumEmployees) {
    if (DS == NULL || CompanyID <= 0 || NumEmployees == NULL || Value == NULL) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->GetCompanyInfo(CompanyID, Value, NumEmployees);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetEmployeeInfo(void *DS, int EmployeeID, int *EmployerID, int *Salary, int *Grade) {
    if (DS == NULL || EmployeeID <= 0 || EmployerID == NULL || Grade == NULL || Salary == NULL) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->GetEmployeeInfo(EmployeeID, EmployerID, Salary, Grade);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType IncreaseCompanyValue(void *DS, int CompanyID, int ValueIncrease) {
    if (DS == NULL || ValueIncrease <= 0 || CompanyID <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->IncreaseCompanyValue(CompanyID, ValueIncrease);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType PromoteEmployee(void *DS, int EmployeeID, int SalaryIncrease, int BumpGrade) {
    if (DS == NULL || SalaryIncrease <= 0 || EmployeeID <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->PromoteEmployee(EmployeeID, SalaryIncrease, BumpGrade);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType HireEmployee(void *DS, int EmployeeID, int NewCompanyID) {
    if (DS == NULL || EmployeeID <= 0 || NewCompanyID <= 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->HireEmployee(EmployeeID, NewCompanyID);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AcquireCompany(void *DS, int AcquirerID, int TargetID, double Factor) {
    if (DS == NULL || AcquirerID <= 0 || TargetID <= 0 || TargetID == AcquirerID || Factor < 1.0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->AcquireCompany(AcquirerID, TargetID, Factor);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetHighestEarner(void *DS, int CompanyID, int *EmployeeID) {
    if (DS == NULL || EmployeeID == NULL || CompanyID == 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->GetHighestEarner(CompanyID, EmployeeID);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetAllEmployeesBySalary(void *DS, int CompanyID, int **Employees, int *NumOfEmployees) {
    if (DS == NULL || Employees == NULL || NumOfEmployees == NULL || CompanyID == 0) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->GetAllEmployeesBySalary(CompanyID, Employees, NumOfEmployees);
    } catch (std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetHighestEarnerInEachCompany(void *DS, int NumOfCompanies, int **Employees) {
    if (DS == NULL || Employees == NULL || NumOfCompanies < 1) return INVALID_INPUT;
    try {
        ((EmployeeManager *)DS)->GetHighestEarnerInEachCompany(NumOfCompanies, Employees);
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetNumEmployeesMatching(void *DS, int CompanyID, int MinEmployeeID, int MaxEmployeeId, int MinSalary, int MinGrade, int *TotalNumOfEmployees, int *NumOfEmployees) {
    if (DS == NULL || CompanyID == 0 || MinEmployeeID < 0 || MaxEmployeeId < 0 || MinSalary < 0 || MinGrade < 0 || TotalNumOfEmployees == NULL || NumOfEmployees == NULL ||
        MinEmployeeID > MaxEmployeeId) {
        return INVALID_INPUT;
    }
    try {
        ((EmployeeManager *)DS)->GetNumEmployeesMatching(CompanyID, MinEmployeeID, MaxEmployeeId, MinSalary, MinGrade, TotalNumOfEmployees, NumOfEmployees);
    } catch (...) {
        return FAILURE;
    }
    return SUCCESS;
}

void Quit(void **DS) {
    ((EmployeeManager *)*DS)->Quit();
    delete (EmployeeManager *)*DS;
    *DS = NULL;
}