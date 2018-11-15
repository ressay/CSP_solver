//
// Created by ressay on 14/11/18.
//

#ifndef PC2_DOMAIN_H
#define PC2_DOMAIN_H

#include <vector>
#include <map>
#include "string"
#include "Variable.h"
#include "Constraint.h"

using namespace std;

class Domains
{
    map<string,vector<int>> D;
    map<string,vector<bool>> exists;
    map<string,vector<int>> current;
public:
    explicit Domains(vector<Variable>& vec);
    void setDomain(Variable& v,const vector<int>& d);
    void remove(Variable& v,int i);
    void add(Variable& v,int i);
    void clear(Variable& v);
    bool exist(Variable& v,int i);
    Constraint getConstraintFromDomains(Variable& v1, Variable& v2);
    void print();
    int findIndexOfVal(Variable& v,int val);


    vector<int> &operator[](const string &i)
    {
        return current[i];
    }
    vector<int> &operator[](const Variable &i)
    {
        return current[i.name];
    }


    void operator=(Domains& dom)
    {
        D = dom.D;
        exists = dom.exists;
        current = dom.current;
    }
};


#endif //PC2_DOMAIN_H
