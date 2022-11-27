#include "EmployeeManager.h"

EmployeeManager::EmployeeManager()
    : Companies(AVLTree<shared_ptr<Company>, CompanyCmpById>(CompanyCmpById())),
      NonEmptyCompanies(AVLTree<shared_ptr<Company>, CompanyCmpById>(CompanyCmpById())),
      EmployeesByID(AVLTree<shared_ptr<Employee>, EmployeeCmpById>(EmployeeCmpById())),
      EmployeesBySalary(AVLTree<shared_ptr<Employee>, EmployeeCmpBySalary>(EmployeeCmpBySalary())),
      recordEarner(NULL) {}

void EmployeeManager::AddCompany(int companyID, int value) {
    shared_ptr<Company> toAdd = make_shared<Company>(companyID, value);  // 2+4
    Companies.insert(toAdd);                                             // 1+3
}

// a function to search a company by Id, returns null if not found
shared_ptr<Company> EmployeeManager::searchCompany(int id) {
    shared_ptr<Company> companyToSearch = make_shared<Company>(id, 0);
    Node<shared_ptr<Company>> *compNode = Companies.search(companyToSearch);
    if (compNode == NULL) {
        return NULL;
    } else {
        return compNode->getValue();
    }
}

// a function to search a employee by Id, returns null if not found
shared_ptr<Employee> EmployeeManager::searchEmployee(int id) {
    shared_ptr<Employee> employeeToSearch = make_shared<Employee>(id, 0);
    Node<shared_ptr<Employee>> *empNode = EmployeesByID.search(employeeToSearch);
    if (empNode == NULL) {
        return NULL;
    } else {
        return empNode->getValue();
    }
}
// a function that updates the highest earner in the system
void EmployeeManager::updateRecordEarner() { recordEarner = EmployeesBySalary.getMaxValue(); }

void EmployeeManager::AddEmployee(int employeeID, int companyID, int salary, int grade) {
    shared_ptr<Company> company = searchCompany(companyID);  // 2
    if (company == NULL) {                                   // 2
        throw CompanyDoesNotExists();
    }
    shared_ptr<Employee> toAdd = make_shared<Employee>(employeeID, company, salary, grade);  // 3+5
    EmployeesByID.insert(toAdd);                                                             // 1+6
    EmployeesBySalary.insert(toAdd);                                                         // 7
    if (company->getSize() == 0) {                                                           // 4
        NonEmptyCompanies.insert(company);                                                   // 4
    }
    this->updateRecordEarner();      // 8
    company->addEmployee(toAdd);     // 9 + 10
    company->updateHighestEarner();  // 11
}

void EmployeeManager::RemoveEmployee(int employeeID) {
    shared_ptr<Employee> toRemove = searchEmployee(employeeID);  // 1
    if (toRemove == NULL) {                                      // 1
        throw EmployeeDoesNotExists();                           // 1
    }
    EmployeesByID.remove(toRemove);                         // 7
    shared_ptr<Company> company = toRemove->getEmployer();  // 2
    company->removeEmployee(toRemove);                      // 2
    EmployeesBySalary.remove(toRemove);                     // 3
    this->updateRecordEarner();                             // 4
    company->updateHighestEarner();                         // 5
    if (company->getSize() == 0) {                          // 6
        NonEmptyCompanies.remove(company);                  // 6
    }
}

void EmployeeManager::RemoveCompany(int companyID) {
    shared_ptr<Company> toRemove = searchCompany(companyID);  // 1
    if (toRemove == NULL) {
        throw CompanyDoesNotExists();  // 1
    }
    if (toRemove->getSize() != 0) {  // 2
        throw CompanyNotEmpty();
    }
    Companies.remove(toRemove);          // 3
    NonEmptyCompanies.remove(toRemove);  // 3
}

void EmployeeManager::GetHighestEarnerInEachCompany(int NumOfCompanies, int **Employees) {
    if (NonEmptyCompanies.getCount() < NumOfCompanies) {  // 1
        throw BadArgument();                              // 1
    }
    int *array = (int *)malloc(NumOfCompanies * sizeof(int));   // 2
    shared_ptr<Company> comps[NumOfCompanies];                  // 3
    NonEmptyCompanies.getAmountInOrder(NumOfCompanies, comps);  // 4+5
    for (int i = 0; i < NumOfCompanies; i++) {                  // 6
        array[i] = (comps[i]->getHighestEarner())->getId();     // 6
    }
    *(Employees) = array;  // 2
    return;
}

void EmployeeManager::GetCompanyInfo(int companyID, int *Value, int *numEmployees) {
    shared_ptr<Company> toSearch = searchCompany(companyID);  // 1
    if (toSearch == NULL) {                                   // 1
        throw CompanyDoesNotExists();                         // 1
    }
    *Value = toSearch->getValue();        // 2
    *numEmployees = toSearch->getSize();  // 2
}

void EmployeeManager::GetEmployeeInfo(int employeeID, int *employerID, int *salary, int *grade) {
    shared_ptr<Employee> toSearch = searchEmployee(employeeID);  // 1
    if (toSearch == NULL) {                                      // 1
        throw EmployeeDoesNotExists();                           // 1
    }
    *employerID = (toSearch->getEmployer())->getId();  // 2
    *salary = toSearch->getSalary();                   // 2
    *grade = toSearch->getGrade();                     // 2
}

void EmployeeManager::IncreaseCompanyValue(int companyID, int valueIncrease) {
    shared_ptr<Company> toSearch = searchCompany(companyID);  // 1
    if (toSearch == NULL) {                                   // 1
        throw CompanyDoesNotExists();                         // 1
    }
    toSearch->increaseValue(valueIncrease);  // 2
}

void EmployeeManager::PromoteEmployee(int employeeID, int salaryIncrease, int bumpgrade) {
    shared_ptr<Employee> emp = searchEmployee(employeeID);  // 1
    if (emp == NULL) {                                      // 1
        throw EmployeeDoesNotExists();                      // 1
    }
    EmployeesBySalary.remove(emp);            // 2
    emp->getEmployer()->removeEmployee(emp);  // 3
    emp->increaseSalary(salaryIncrease);      // 4
    EmployeesBySalary.insert(emp);            // 5
    emp->getEmployer()->addEmployee(emp);     // 5
    if (bumpgrade > 0) {                      // 4
        emp->bumpGrade();                     // 4
    }
    emp->getEmployer()->updateHighestEarner();  // 6
    updateRecordEarner();                       // 6
}

void EmployeeManager::HireEmployee(int employeeID, int newCompanyID) {
    shared_ptr<Employee> emp = searchEmployee(employeeID);  // 1
    if (emp == NULL) {                                      // 1
        throw EmployeeDoesNotExists();                      // 1
    }
    shared_ptr<Company> newComp = searchCompany(newCompanyID);  // 2
    if (newComp == NULL) {                                      // 2
        throw CompanyDoesNotExists();                           // 2
    }
    shared_ptr<Company> oldComp = emp->getEmployer();
    if (oldComp->getId() == newCompanyID) {  // 3
        throw EmployeeAlreadyHired();        // 3
    }
    oldComp->removeEmployee(emp);           // 4
    if (oldComp->getSize() == 0) {          // 5
        NonEmptyCompanies.remove(oldComp);  // 5
        oldComp->updateHighestEarner();  // 6
    } else {
        oldComp->updateHighestEarner();  // 6
    }
    if (newComp->getSize() == 0) {          // 7
        NonEmptyCompanies.insert(newComp);  // 7
    }
    newComp->addEmployee(emp);       // 8
    newComp->updateHighestEarner();  // 9
    emp->setEmployer(newComp);       // 10
}

// a function that merges two sorted arrays of Employees
template <class Cmp>
void MergeEmployees(shared_ptr<Employee> *array1, shared_ptr<Employee> *array2, int l1, int l2, shared_ptr<Employee> *mergedArray, Cmp cmp) {
    int i = 0, j = 0, k = 0;

    while (i < l1 && j < l2) {
        if (cmp(array1[i], array2[j]) < 0)
            mergedArray[k++] = array1[i++];
        else
            mergedArray[k++] = array2[j++];
    }
    while (i < l1) {
        mergedArray[k] = array1[i];
        i++;
        k++;
    }

    while (j < l2) mergedArray[k++] = array2[j++];
}

void EmployeeManager::AcquireCompany(int acquirerID, int targetID, double factor) {
    shared_ptr<Company> acquirer = searchCompany(acquirerID);  // 1
    shared_ptr<Company> target = searchCompany(targetID);      // 2
    if (acquirer == NULL || target == NULL) {                  // 1+2                                                                                     // 1.1
        throw CompanyDoesNotExists();                          // 1+2
    }
    if (!(target->getValue() * 10 <= acquirer->getValue())) {  // 3
        throw BadArgument();
    }
    int a = acquirer->getSize();
    int t = target->getSize();
    int targetValue = target->getValue();
    if (t > 0) {
        // setting up arrays by ID
        EmployeeCmpById cmpByID = EmployeeCmpById();
        shared_ptr<Employee> mergedArrayByID[a + t];  // 4.3
        shared_ptr<Employee> acquirerArrayByID[a];    // 4.1
        shared_ptr<Employee> targetArrayByID[t];      // 4.2
        acquirer->getEmployeesInOrderById(a, acquirerArrayByID);
        target->getEmployeesInOrderById(t, targetArrayByID);

        // setting up arrays by salary
        EmployeeCmpBySalary cmpBySalary = EmployeeCmpBySalary();
        shared_ptr<Employee> mergedArrayBySalary[a + t];  // 4.3
        shared_ptr<Employee> acquirerArrayBySalary[a];    // 4.1
        shared_ptr<Employee> targetArrayBySalary[t];      // 4.2

        acquirer->getEmployeesInOrderBySalary(a, acquirerArrayBySalary);  // 4.1
        target->getEmployeesInOrderBySalary(t, targetArrayBySalary);      // 4.2

        // Updating the new employer
        for (int i = 0; i < t; i++) {                   // 4.4
            targetArrayByID[i]->setEmployer(acquirer);  // 4.4
        }

        // merging arrays
        MergeEmployees(acquirerArrayByID, targetArrayByID, a, t, mergedArrayByID, cmpByID);                  // 4.3
        MergeEmployees(acquirerArrayBySalary, targetArrayBySalary, a, t, mergedArrayBySalary, cmpBySalary);  // 4.3
        // building AVL tree from sorted arrays and replacing old trees
        acquirer->replaceTrees(mergedArrayByID, mergedArrayBySalary, a + t);  // 4.5 + 5
        target->emptyComapny();                                               // 7
    }

    this->RemoveCompany(targetID);  // 7
    if (a == 0 && t > 0) {
        NonEmptyCompanies.insert(acquirer);  // 6
    }
    acquirer->setValue(factor * (targetValue + acquirer->getValue()));  // 8
    acquirer->updateHighestEarner();                                    // 9
}

void EmployeeManager::GetHighestEarner(int companyID, int *employeeID) {
    if (companyID > 0) {                                      // 1
        shared_ptr<Company> comp = searchCompany(companyID);  // 1.1
        if (comp == NULL) {                                   // 1.1
            throw CompanyDoesNotExists();                     // 1.1
        }
        shared_ptr<Employee> emp = comp->getHighestEarner();  // 1.2
        if (emp == NULL) {                                    // 1.2
            throw CompanyIsEmpty();                           // 1.2
        }
        *(employeeID) = emp->getId();  // 1.2
    } else {                           // 2
        if (recordEarner == NULL) {    // 2.1
            throw CompanyIsEmpty();    // 2.1
        }
        *(employeeID) = recordEarner->getId();  // 2.1
    }
}

void EmployeeManager::GetAllEmployeesBySalary(int companyID, int **employees, int *numOfEmployees) {
    if (companyID > 0) {                                      // 1
        shared_ptr<Company> comp = searchCompany(companyID);  // 1.1
        if (comp == NULL) {                                   // 1.1
            throw CompanyDoesNotExists();                     // 1.1
        }                                                     // 1.1
        int compSize = comp->getSize();                       // 1.2
        if (compSize == 0) {                                  // 1.2
            throw CompanyIsEmpty();                           // 1.2
        }
        int *array = (int *)malloc(compSize * sizeof(int));  // 1.2
        shared_ptr<Employee> elems[compSize];                // 1.3
        comp->getEmployeesInOrderBySalary(compSize, elems);  // 1.4
        for (int i = 1; i <= compSize; i++) {                // 1.5
            array[i - 1] = elems[compSize - i]->getId();     // 1.5
        }
        *numOfEmployees = compSize;  // 1.6
        (*employees) = array;        // 1.7

    } else {                               // 2
        int n = EmployeesByID.getCount();  // 2.1
        if (n == 0) {                      // 2.2
            throw CompanyIsEmpty();        // 2.2
        }
        int *array = (int *)malloc(n * sizeof(int));   // 2.1
        shared_ptr<Employee> elems[n];                 // 2.3
        EmployeesBySalary.getAmountInOrder(n, elems);  // 2.4
        for (int i = 1; i <= n; i++) {                 // 2.5
            array[i - 1] = elems[n - i]->getId();      // 2.5
        }
        *numOfEmployees = n;   // 2.6
        (*employees) = array;  // 2.7
    }
}

// A function object to validate the conditions in the GetNumEmployeesMatching
class IdInRange {
   private:
    int min;
    int max;

   public:
    IdInRange(int min, int max) : min(min), max(max) {}
    int operator()(shared_ptr<Employee> e) {  // returns 0 if in range, 1 if above range, -1 if lower than range
        int eId = e->getId();
        if (min <= eId && eId <= max) {
            return 0;
        } else if (eId < min) {
            return -1;
        } else {
            return 1;
        }
    }
};

// A function object to validate the conditions in the GetNumEmployeesMatching
class IdAndSalaryInRange {
   private:
    int minId;
    int maxId;
    int minSalary;
    int minGrade;

   public:
    IdAndSalaryInRange(int minId, int maxId, int minSalary, int minGrade) : minId(minId), maxId(maxId), minSalary(minSalary), minGrade(minGrade) {}
    int operator()(shared_ptr<Employee> e) {  // returns 0 if in range and Salary&grade valid, 1 if above id range, -1 if lower than id range, 2 if in id range but salary/grade not valid
        int eId = e->getId();
        int salary = e->getSalary();
        int grade = e->getGrade();
        if (minId <= eId && eId <= maxId) {
            if (salary >= minSalary && grade >= minGrade) {
                return 0;
            } else {
                return 2;
            }
        } else if (eId < minId) {
            return -1;
        } else {
            return 1;
        }
    }
};

void EmployeeManager::GetNumEmployeesMatching(int companyID, int minEmployeeID, int maxEmployeeId, int minSalary, int minGrade, int *totalNumOfEmployees, int *numOfEmployees) {
    // setting up the two conditions
    IdInRange cond1 = IdInRange(minEmployeeID, maxEmployeeId);
    IdAndSalaryInRange cond2 = IdAndSalaryInRange(minEmployeeID, maxEmployeeId, minSalary, minGrade);
    if (companyID > 0) {                                      // 1
        shared_ptr<Company> comp = searchCompany(companyID);  // 1.1
        if (comp == NULL) {                                   // 1.1
            throw CompanyDoesNotExists();                     // 1.1
        }
        int compSize = comp->getSize();  // 1.2
        if (compSize == 0) {             // 1.2
            throw CompanyIsEmpty();      // 1.2
        }

        *totalNumOfEmployees = comp->getEmployeesMathchingInRange(cond1);  // 1.3
        *numOfEmployees = comp->getEmployeesMathchingInRange(cond2);       // 1.3

    } else {  // 2
        int n = EmployeesByID.getCount();
        if (n == 0) {
            throw NoEmployees();
        }
        *totalNumOfEmployees = EmployeesByID.getAmountInRange(cond1);
        *numOfEmployees = EmployeesByID.getAmountInRange(cond2);
    }
}

EmployeeManager::~EmployeeManager() { recordEarner.reset(); }
void EmployeeManager::Quit() {
    recordEarner.reset();

    int n = NonEmptyCompanies.getCount();          // 2.1
    shared_ptr<Company> elems[n];                  // 3
    NonEmptyCompanies.getAmountInOrder(n, elems);  // 3
    for (int i = 0; i < n; i++) {
        elems[i]->empty();  // 3
    }
    EmployeesByID.emptyTree();      // 1
    EmployeesBySalary.emptyTree();  // 2
    Companies.emptyTree();          // 4
    NonEmptyCompanies.emptyTree();  // 4
}
