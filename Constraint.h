//
// Created by ressay on 14/11/18.
//

#ifndef PC2_CONSTRAINT_H
#define PC2_CONSTRAINT_H

#include "iostream"
using namespace std;

class Constraint
{
public:
    const static int UNIVERSAL = 1;
    const static int IDENTITY = 0;
    int n,m;
    int** M;

    Constraint(int n, int m, int** R);
    Constraint(int n, int m);
    Constraint(); // constructors
    void setConstraint(int n, int m,int flag); //set constraint as UNIVERSAL or IDENTITY
    bool isZero(); // checks if constraint is all zero
    Constraint& transpose(); // transpose the constraint
    void print(); // prints the contsraint matrix
    int* &operator[](int i); // overload the [] operator to simplify access
    Constraint &operator&(Constraint& con); // overload the & operator to simplify intersection
    Constraint &operator*(Constraint& con); // overload the * operator to simplify the composition
    void operator=(Constraint& con); // overload the = operator to simplify the copy function
    bool operator==(Constraint& con); // overload the == operator to simplify comparison

};


#endif //PC2_CONSTRAINT_H
