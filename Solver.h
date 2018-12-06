//
// Created by ressay on 14/11/18.
//

#ifndef PC2_SOLVER_H
#define PC2_SOLVER_H


#include "Domains.h"
#include "Constraint.h"

class Solver
{   // CSP parameters
    Domains D;
    vector<Variable> X;
    Constraint** C;
public:
    int iterations = 0;
    Solver(Domains &D, const vector<Variable> &X); // constructor
    void setConstraint(int i,int j, Constraint c); // set constraint between variables i and j
    map<string,int> solve(bool heuristic); // solve the CSP
    bool lookAhead(map<string,int>&,Domains&,Constraint**,vector<int>,
                   vector<pair<int,int>> q,bool heuristic);
    void pc2(Domains&,vector<Variable>&,Constraint**,vector<pair<int,int>>);
    bool inconsistant(Constraint** c);
    int next(vector<Variable>& vec,Domains D,vector<int>);
    int nextH(vector<Variable>& vec,Domains D,vector<int>);
    Constraint** copy(Constraint** c);
    void updateDomain(Domains& d, Constraint** c,vector<Variable>& x);
    void updateConstraint(Domains& d, Constraint** c,vector<Variable>& x);
    bool allInstanciated(vector<int> instanciated);
    void pc1(Domains&,vector<Variable>&,Constraint**,vector<int>);
    bool revise(int i,int j,int k,Domains d, Constraint** c);
    bool allSingletons(Domains d);
    void print();
};


#endif //PC2_SOLVER_H
