//
// Created by ressay on 14/11/18.
//

#ifndef PC2_VARIABLE_H
#define PC2_VARIABLE_H

#include <vector>
#include "string"

using namespace std;

class Variable
{
public:
    Variable(const string &name) : name(name)
    {}

    string name;
    void operator=(Variable& v)
    {
        name = v.name;
    }
};


#endif //PC2_VARIABLE_H
