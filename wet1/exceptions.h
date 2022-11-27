#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class AlreadyExists : public std::exception {
   public:
    char const* what() const noexcept override { return "AlreadyExists"; }
};

class DoesNotExists : public std::exception {
   public:
    char const* what() const noexcept override { return "DoesNotExists"; }
};

class CompanyDoesNotExists : public std::exception {
   public:
    char const* what() const noexcept override { return "CompanyDoesNotExists"; }
};

class EmployeeAlreadyExists : public std::exception {
   public:
    char const* what() const noexcept override { return "EmployeeAlreadyExists"; }
};

class EmployeeDoesNotExists : public std::exception {
   public:
    char const* what() const noexcept override { return "EmployeeDoesNotExists"; }
};
class EmployeeAlreadyHired : public std::exception {
   public:
    char const* what() const noexcept override { return "EmployeeAlreadyHired"; }
};
class CompanyNotEmpty : public std::exception {
   public:
    char const* what() const noexcept override { return "CompanyNotEmpty"; }
};
class CompanyAlreadyExists : public std::exception {
   public:
    char const* what() const noexcept override { return "CompanyAlreadyExists"; }
};
class BadArgument : public std::exception {
   public:
    char const* what() const noexcept override { return "BadArgument"; }
};
class CompanyIsEmpty : public std::exception {
   public:
    char const* what() const noexcept override { return "CompanyIsEmpty"; }
};
class NoEmployees : public std::exception {
   public:
    char const* what() const noexcept override { return "NoEmployees"; }
};

#endif  // EXCEPTIONS_H