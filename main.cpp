#include <iostream>
#include <map>
#include <vector>
#include "Variable.h"
#include "Domains.h"
#include "Solver.h"

using namespace std;

Solver generateCSPRandom(int numVariables,int domainSize, double difficulty);

Solver generateNQueenProblem(int N);

int main()
{
//    vector<Variable> X;
//    X.emplace_back("a");
//    X.emplace_back("b");
//    Domains D(X);
//    D.setDomain(X[0],{1,5,6,9});
//    D.setDomain(X[1],{0,2,6});
//    Solver solver(D,X);
//    int** m = new int*[D[X[0]].size()];
//    for (int i = 0; i < D[X[0]].size(); ++i)
//    {
//        m[i] = new int[D[X[1]].size()];
//        for (int j = 0; j < D[X[1]].size(); ++j)
//        {
//            if(D[X[0]][i]==D[X[1]][j])
//                m[i][j] = 1;
//            else m[i][j] = 0;
//        }
//    }
//
//    Constraint c(4,3,m);
//    solver.setConstraint(0,1,c);
//    map<string,int> res = solver.solve();
//    cout << "a: " << res["a"] << " b: " << res["b"] << endl;
//    Solver s = generateNQueenProblem(8);
    Solver s = generateCSPRandom(4,5,0.7);
    s.print();
    map<string,int> res = s.solve();
    map<string,int>::iterator it;
    cout << "result: " << endl;
    for(it = res.begin(); it != res.end(); it++)
        cout << it->first << ": " << it->second << endl;
    return 0;
}

int random(int min,int max)
{
    return min + rand()%(max-min);
}

Constraint generateRandomConstraint(int N,int M, double difficulty,bool self)
{
    int thresh = difficulty*100;
    Constraint c = Constraint(N,M);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if(self && i!=j) c[i][j] = 0;
            else if(random(0,100) <= thresh)
                c[i][j] = 0;
            else
                c[i][j] = 1;
        }
    }
    return c;
}

Solver generateCSPRandom(int numVariables,int domainSize, double difficulty)
{
    vector<Variable> X;
    srand(time(NULL));
    for (int i = 0; i < numVariables; ++i)
    {
        X.emplace_back("q"+std::to_string(i));
    }

    Domains D(X);
    for (int i = 0; i < numVariables; ++i)
    {
        vector<int> domain;
        int size = random(domainSize/2+1,domainSize*1.5);
        for (int j = 0; j < size; ++j)
        {
            domain.push_back(j+1);
        }
        D.setDomain(X[i],domain);
    }
    Solver solver(D,X);

    for (int i = 0; i < numVariables; ++i)
    {
        for (int j = i+1; j < numVariables; ++j)
        {
            int N = D[X[i]].size(),M = D[X[j]].size();
            solver.setConstraint(i,j,generateRandomConstraint(N,M,difficulty,i==j));
        }
    }
    return solver;
}

Solver generateNQueenProblem(int N)
{
    vector<Variable> X;
    vector<int> domain;
    for (int i = 0; i < N; ++i)
    {
        domain.push_back(i+1);
        X.emplace_back("q"+std::to_string(i));
    }
    Domains D(X);
    for (int i = 0; i < N; ++i)
    {
        D.setDomain(X[i],domain);
    }
    Solver solver(D,X);
    int** m = new int*[N];
    for (int i = 0; i < N; ++i)
    {
        m[i] = new int[N];
        for (int j = 0; j < N; ++j)
        {
            if(D[X[0]][i]!=D[X[1]][j])
                m[i][j] = 1;
            else m[i][j] = 0;
        }
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = i+1; j < N; ++j)
        {
            solver.setConstraint(i,j,Constraint(N,N,m));
        }
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = i+1; j < N; ++j)
        {

            for (int k = 0; k < N; ++k)
            {
                for (int l = 0; l < N; ++l)
                {
                    if(k - l == i - j || k - l == j - i)
                        m[k][l] = 0;
                    else m[k][l] = 1;

                }
            }
            solver.setConstraint(i,j,Constraint(N,N,m));
        }
    }
    return solver;
}