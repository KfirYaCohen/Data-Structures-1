#ifndef UFNODE_H
#define UFNODE_H

#include <memory>

using std::make_shared;
using std::shared_ptr;

class UFNode {
   private:
    UFNode* father;
    int size;
    shared_ptr<Company> master;
    shared_ptr<Company> company;
    long double r;

   public:
    UFNode(shared_ptr<Company> company) : father(NULL), size(1), master(company), company(company), r(company->getID()){};
    UFNode() : father(NULL), size(-1), master(NULL), company(NULL){};
    UFNode* getFather() { return father; }
    int getSize() { return size; }
    long double getR() { return r; }
    shared_ptr<Company> getMaster() { return master; }
    shared_ptr<Company> getCompany() { return company; }
    void setCompany(shared_ptr<Company> c) { this->master = c; }
    void setFather(UFNode* father) { this->father = father; }
    void increaseSize(int size) { this->size += size; }
    void setSize(int size) { this->size = size; }
    void setR(long double r) { this->r = r; }
    void setMaster(shared_ptr<Company> master) { this->master = master; }
    ~UFNode() {
        master.reset();
        company.reset();
    };
    void empty() {
        master.reset();
        company.reset();
    }
};

#endif  // UFNODE_H