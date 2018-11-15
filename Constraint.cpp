//
// Created by ressay on 14/11/18.
//

#include "Constraint.h"
#include "iostream"
using namespace std;

void Constraint::setConstraint(int n, int m, int flag)
{
    this->n = n;
    this->m = m;
    M = new int*[n];
    for (int i = 0; i < n; ++i)
    {
        M[i] = new int[m];
        for (int j = 0; j < m; ++j)
        {
            M[i][j] = (i==j)?1:flag;
        }
    }
}

bool Constraint::isZero()
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if(M[i][j] != 0)
                return false;
        }
    }
    return true;
}

Constraint &Constraint::transpose()
{
    Constraint* result = new Constraint(m,n);
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            (*result)[i][j] = M[j][i];
        }
    }
    return *result;
}

void Constraint::print()
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

