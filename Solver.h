//
// Created by ressay on 14/11/18.
//

#ifndef PC2_SOLVER_H
#define PC2_SOLVER_H


#include "Domains.h"
#include "Constraint.h"

class Solver
{
    Domains D;
    vector<Variable> X;
    Constraint** C;
public:
    int iterations = 0;
    Solver(Domains &D, const vector<Variable> &X);
    void setConstraint(int i,int j, Constraint c);
    map<string,int> solve();
    void pc1(Domains&,vector<Variable>&,Constraint**,vector<int>);
    void pc2(Domains&,vector<Variable>&,Constraint**,vector<int>);
    bool revise(int i,int j,int k,Domains d, Constraint** c);
    void updateDomain(Domains& d, Constraint** c,vector<Variable>& x);
    void updateConstraint(Domains& d, Constraint** c,vector<Variable>& x);
    bool lookAhead(map<string,int>&,Domains&,Constraint**,vector<int>);
    Constraint** copy(Constraint** c);
    bool inconsistant(Constraint** c);
    int next(vector<Variable>& vec,Domains D,vector<int>);
    int nextH(vector<Variable>& vec,Domains D,vector<int>);
    bool allInstanciated(vector<int> instanciated);
    void print()
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
};


#endif //PC2_SOLVER_H
