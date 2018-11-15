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
    Constraint(int n, int m, int** R) : n(n), m(m)
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

    Constraint(int n, int m) : n(n), m(m)
    {
        M = new int*[n];
        for (int i = 0; i < n; ++i)
        {
            M[i] = new int[m];
        }
    }

    Constraint()
    {}

    void setConstraint(int n, int m,int flag);
    bool isZero();
    Constraint& transpose();
    void print();

    int n,m;
    int** M;
    int* &operator[](int i)
    {
        return M[i];
    }

    Constraint &operator&(Constraint& con)
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
//        cout << "got here2 crap" << endl;
    }

    Constraint &operator*(Constraint& con)
    {
//        cout << "doing: " << endl;
//        print();
//        cout << "*" << endl;
//        con.print();
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
//        cout << "got here crap" << endl;
    }

    void operator=(Constraint& con)
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

    bool operator==(Constraint& con)
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

};


#endif //PC2_CONSTRAINT_H
