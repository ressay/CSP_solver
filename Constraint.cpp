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

Constraint::Constraint(int n, int m, int **R) : n(n), m(m)
{
    M = new int*[n];
    for (int i = 0; i < n; ++i)
    {
        M[i] = new int[m];
        for (int j = 0; j < m; ++j)
        {
            M[i][j] = R[i][j];
        }
    }
}

Constraint::Constraint(int n, int m) : n(n), m(m)
{
    M = new int*[n];
    for (int i = 0; i < n; ++i)
    {
        M[i] = new int[m];
    }
}

Constraint::Constraint()
{}

int *&Constraint::operator[](int i)
{
    return M[i];
}

Constraint &Constraint::operator&(Constraint &con)
{
    if(con.n == n && con.m == m)
    {
        Constraint* result = new Constraint(n,m);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                (*result)[i][j] = M[i][j]*con[i][j];
            }
        }
        return *result;
    }
}

Constraint &Constraint::operator*(Constraint &con)
{
    if(con.n == m)
    {
        Constraint* result = new Constraint(n,con.m);
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < con.m; ++j)
            {
                (*result)[i][j] = 0;
                for (int k = 0; k < m && !(*result)[i][j]; ++k)
                {
                    (*result)[i][j] = (*result)[i][j] || M[i][k]*con[k][j];
                }
            }
        }
        return *result;
    }
}

void Constraint::operator=(Constraint &con)
{
    n = con.n;
    m = con.m;
    M = new int*[n];
    for (int i = 0; i < n; ++i)
    {
        M[i] = new int[m];
        for (int j = 0; j < con.m; ++j)
        {
            M[i][j] = con[i][j];
        }
    }
}

bool Constraint::operator==(Constraint &con)
{
    if(con.n == n && con.m == m)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < con.m; ++j)
            {
                if(M[i][j] != con[i][j])
                    return false;
            }
        }
        return true;
    }
    return false;
}

