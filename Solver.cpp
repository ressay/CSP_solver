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

map<string,int> Solver::solve(bool heuristic)
{
    map<string,int> v;
    vector<int> ins;
    for (int i = 0; i < X.size(); ++i)
    {
        ins.push_back(0);
    }
    vector<pair<int,int>> q;
    for (int i = 0; i < X.size(); ++i)
    {
        for (int j = i; j < X.size(); ++j)
        {
            q.emplace_back(i,j);
        }
    }
    iterations = 0;
    if(lookAhead(v,D,C,ins,q,heuristic))
    {
        cout << "après: " << iterations << " iterations, résultat trouvé!" << endl;
        return v;
    }
    cout << "pas de résultat\n";
    return v;
}

bool Solver::allSingletons(Domains d)
{
    for (int i = 0; i < X.size(); ++i)
    {
        if(d[X[i]].size() != 1)
            return false;
    }
    return true;
}

bool Solver::lookAhead(map<string,int>& A, Domains& d,Constraint ** c,
                       vector<int> instanciated,vector<pair<int,int>> q,bool heuristic)
{
    iterations++;
    pc2(d,X,c,q);
//    pc1(d,X,c,instanciated);
    if(inconsistant(c)) return false;
    if(allInstanciated(instanciated)) return true;
    if(allSingletons(d))
    {
        for (int i = 0; i < X.size(); ++i) {
            A[X[i].name] = d[X[i]][0];
        }
        return true;
    }

    int iV;
    if(heuristic) // instantiating next variable with heuristic
        iV = nextH(X,d,instanciated);
    else
        iV = next(X,d,instanciated);
    Variable v = X[iV];
    instanciated[iV] = 1; // set instanciated of the variable to true
    for (int i = 0; i < d[v].size(); ++i)
    {
        Domains newD = d;
        newD.clear(v);
        newD.add(v,d[v][i]); // setting new domain of the variable instantiated
        A[v.name] = d[v][i];
        map<string,int> newA = A; // copying the values instantiated
        Constraint** newC = copy(c); // copying the Mp Matrix for recursive method call
        updateConstraint(newD,newC,X); // update the new Mp Matrix to match the newly instantiated variable
        q.emplace_back(iV,iV); // adding in the queue the newly instantiated variable for next call's PC2
        if(lookAhead(A,newD,newC,instanciated,q,heuristic)) // call of look ahead
        {
//            A = newA;
            return true;
        }
    }
    return false;
}

void Solver::pc2(Domains &d, vector<Variable> &x, Constraint **c, vector<pair<int,int>> q)
{
    int it = 0;
    while (!q.empty())
    {

        pair<int,int> p = q[0];
//        q.pop_back();
        q.erase(q.begin());
        int i = p.first,j = p.second;
        for (int k = 0; k < x.size(); ++k)
        {
            it++;
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
    int it = 0;
    do
    {
        done = true;
        for (int i = 0; i < X.size(); ++i)
        {
            for (int j = 0; j < X.size(); ++j)
            {
                for (int k = 0; k < X.size(); ++k)
                {
                    it++;
                    if (revise(i, j, k, d, c)) done = false;
                }
            }
        }
    }
    while(!done);
    cout << "iterated: " << it << endl;
    updateDomain(d,c,x);
}

bool Solver::revise(int i, int j, int k, Domains d, Constraint **c)
{

    Constraint temp = c[i][j]&(c[i][k]*c[k][k]*c[k][j]);
    if(temp == c[i][j]) return false;
    c[i][j] = temp;
    return true;
}

//void Solver::updateDomain(Domains& d, Constraint **c,vector<Variable>& x)
//{
//    for (int i = 0; i < x.size(); ++i)
//    {
//        for (int j = 0; j < x.size(); ++j)
//        {
//            Constraint con = c[i][j];
//            Variable var = x[i];
//            for (int k = 0; k < con.n; ++k)
//            {
//                bool allZero = true;
//                for (int l = 0; l < con.m && allZero; ++l)
//                {
//                    if(con[k][l] != 0) allZero = false;
//                }
//                if(allZero)
//                {
//                    cout << "removing from i: " << i << " the " << k << endl;
//                    d.remove(var,k);
//                }
//            }
//        }
//    }
//}

void Solver::updateDomain(Domains& d, Constraint **c,vector<Variable>& x)
{
    for (int i = 0; i < x.size(); ++i)
    {
        Constraint con = c[i][i];
        Variable var = x[i];
        for (int l = 0; l < con.n; ++l)
        {
            if(con[l][l] == 0)
            {
                d.remove(var, l);
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

bool Solver::allInstanciated(vector<int> instanciated)
{
    for (int i = 0; i < instanciated.size(); ++i)
    {
        if(!instanciated[i])
            return false;
    }
    return true;
}


void Solver::print()
{
    for (int i = 0; i < X.size(); ++i)
    {
        for (int j = 0; j < X.size(); ++j)
        {
            cout << "Mp[" << i << "," << j << "]:" << endl;
            C[i][j].print();
        }
    }
}









