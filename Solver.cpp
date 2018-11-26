//
// Created by ressay on 14/11/18.
//

#include <iostream>
#include "Solver.h"

Solver::Solver(Domains &D, const vector<Variable> &X) : D(D), X(X)
{
    C = new Constraint*[X.size()];
    for (int i = 0; i < X.size(); ++i)
    {
        C[i] = new Constraint[X.size()];
        for (int j = 0; j < X.size(); ++j)
        {
            C[i][j].setConstraint(D[X[i]].size(),D[X[j]].size(),i!=j);
        }
    }

}

void Solver::setConstraint(int i, int j, Constraint c)
{
    Constraint s = C[i][j];
    if(c.n != s.n || c.m != s.m)
        return;
    C[i][j] = C[i][j]&c;
    C[j][i] = C[j][i]&c.transpose();
}

map<string,int> Solver::solve()
{
    map<string,int> v;
    vector<int> ins;
    for (int i = 0; i < X.size(); ++i)
    {
        ins.push_back(0);
    }
    iterations = 0;
    if(lookAhead(v,D,C,ins))
    {
        cout << "after: " << iterations << " iterations, done!" << endl;
        return v;
    }
    cout << "no answer";
    return v;
}

bool Solver::lookAhead(map<string,int>& A, Domains& d,Constraint ** c,
                       vector<int> instanciated)
{
    iterations++;
    pc2(d,X,c,instanciated);
    d.print();
    if(inconsistant(c))
        return false;
    if(allInstanciated(instanciated))
        return true;
    int iV = nextH(X,d,instanciated);
    Variable v = X[iV];
    instanciated[iV] = 1;
    cout << "instanciating: " << v.name << endl;
    for (int i = 0; i < d[v].size(); ++i)
    {
        cout << "val: " << d[v][i] << endl;
        Domains newD = d;
        newD.clear(v);
        newD.add(v,d[v][i]);
        map<string,int> newA = A;
        newA[v.name] = d[v][i];
        Constraint** newC = copy(c);
        updateConstraint(newD,newC,X);
        if(lookAhead(newA,newD,newC,instanciated))
        {
            A = newA;
            return true;
        }
    }
    return false;
}

void Solver::pc2(Domains &d, vector<Variable> &x, Constraint **c, vector<int> ins)
{
    vector<pair<int,int>> q;
    for (int i = 0; i < x.size(); ++i)
    {
        for (int j = i+1; j < x.size(); ++j)
        {
            q.emplace_back(i,j);
        }
    }
    while (!q.empty())
    {
        pair<int,int> p = q[0];
        q.pop_back();
        int i = p.first,j = p.second;
        for (int k = 0; k < x.size(); ++k)
        {
            if(k == i && k == j)continue;
            Constraint temp = c[i][k]&(c[i][j]*c[j][j]*c[j][k]);
            if(!(temp == c[i][k]))
            {
                c[i][k] = temp;
                c[k][i] = temp.transpose();
                if(i<k) q.emplace_back(i,k); else q.emplace_back(k,i);
            }
            temp = c[k][j]&(c[k][i]*c[i][i]*c[i][j]);
            if(!(temp == c[k][j]))
            {
                c[k][j] = temp;
                c[j][k] = temp.transpose();
                if(k<j) q.emplace_back(k,j); else q.emplace_back(j,k);
            }
        }
    }
    updateDomain(d,c,x);
}

void Solver::pc1(Domains& d, vector<Variable>& x, Constraint ** c,vector<int> instanciated)
{
    bool done;
    do
    {
        done = true;
        for (int i = 0; i < X.size(); ++i)
        {
            for (int j = 0; j < X.size(); ++j)
            {
                for (int k = 0; k < X.size(); ++k)
                {
                    if (revise(i, j, k, d, c)) done = false;
                }
            }
        }
    }
    while(!done);
    updateDomain(d,c,x);
}

bool Solver::revise(int i, int j, int k, Domains d, Constraint **c)
{

    Constraint temp = c[i][j]&(c[i][k]*c[k][k]*c[k][j]);
    if(temp == c[i][j]) return false;
    c[i][j] = temp;
    return true;
}

void Solver::updateDomain(Domains& d, Constraint **c,vector<Variable>& x)
{
    for (int i = 0; i < x.size(); ++i)
    {
        for (int j = 0; j < x.size(); ++j)
        {
            Constraint con = c[i][j];
            Variable var = x[i];
            for (int k = 0; k < con.n; ++k)
            {
                bool allZero = true;
                for (int l = 0; l < con.m && allZero; ++l)
                {
                    if(con[k][l] != 0) allZero = false;
                }
                if(allZero)
                {
                    d.remove(var,k);
                }
            }
        }
    }
}

void Solver::updateConstraint(Domains &d, Constraint **c, vector<Variable> &x)
{
    for (int i = 0; i < x.size(); ++i)
    {
        for (int j = i+1; j < x.size(); ++j)
        {
            Variable var = x[i];
            Variable support = x[j];
            Constraint domC = d.getConstraintFromDomains(var,support);
            c[i][j] = c[i][j]&domC;
            domC = domC.transpose();
            c[j][i] = c[j][i]&domC;
        }
    }
}



Constraint **Solver::copy(Constraint **c)
{
    Constraint** cop = new Constraint*[X.size()];
    for (int i = 0; i < X.size(); ++i)
    {
        cop[i] = new Constraint[X.size()];
        for (int j = 0; j < X.size(); ++j)
        {
            cop[i][j] = c[i][j];
        }
    }
    return cop;
}

bool Solver::inconsistant(Constraint **c)
{
    for (int i = 0; i < X.size(); ++i)
    {
        for (int j = 0; j < X.size(); ++j)
        {
            if(c[i][j].isZero())
            {
//                cout << "inconsistant in: " << i << j << endl;
//                c[i][j].print();
                return true;
            }
        }
    }
    return false;
}

int Solver::next(vector<Variable>& vec, Domains D,vector<int> instanciated)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        if(!instanciated[i])
            return i;
    }
}

bool Solver::allInstanciated(vector<int> instanciated)
{
    for (int i = 0; i < instanciated.size(); ++i)
    {
        if(!instanciated[i])
            return false;
    }
    return true;
}

int Solver::nextH(vector<Variable> &vec, Domains D, vector<int> instanciated)
{
    int minD = INT32_MAX;
    int savedI = 0;
    for (int i = 0; i < vec.size(); ++i)
    {
        int size = D[vec[i]].size();
        if(!instanciated[i] && size < minD)
        {
            savedI = i;
            minD = size;
        }
    }
    return savedI;
}









