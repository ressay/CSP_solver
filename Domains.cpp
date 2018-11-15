//
// Created by ressay on 14/11/18.
//

#include "Domains.h"

bool Domains::exist(Variable &v, int i)
{
    return exists[v.name][i];
}

void Domains::remove(Variable &v, int i)
{
    if(!exist(v,i))
        return;
    exists[v.name][i] = false;
    vector<int> newV;
    for (int j = 0; j < D[v.name].size(); ++j)
    {
        if(exist(v,j))
            newV.push_back(D[v.name][j]);
    }
    current[v.name] = newV;
}

void Domains::add(Variable &v, int val)
{
    int i = findIndexOfVal(v,val);
    if(exist(v,i))
        return;
    exists[v.name][i] = true;
    vector<int> newV;
    for (int j = 0; j < D[v.name].size(); ++j)
    {
        if(exist(v,j))
            newV.push_back(D[v.name][j]);
    }
    current[v.name] = newV;
}

void Domains::clear(Variable& v)
{
    for (int j = 0; j < D[v.name].size(); ++j)
    {
        exists[v.name][j] = false;
    }
    current[v.name].clear();
}

Constraint Domains::getConstraintFromDomains(Variable &v1, Variable &v2)
{
    int n = D[v1.name].size(), m = D[v2.name].size();
    Constraint res = Constraint(n,m);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            res[i][j] = exist(v1,i)*exist(v2,j);
        }
    }
    return res;
}

Domains::Domains(vector<Variable>& vec)
{
    for (auto &i : vec)
    {
        D[i.name] = vector<int>();
        exists[i.name] = vector<bool>();
        current[i.name] = vector<int>();
    }
}

void Domains::setDomain(Variable &v, const vector<int> &d)
{
    D[v.name] = d;
    current[v.name] = d;
    for (int i = 0; i < d.size(); ++i)
    {
        exists[v.name].push_back(true);
        cout << current[v.name][i] << " ";
    }
    cout << endl;

}

void Domains::print()
{
    map<string,vector<int>>::iterator it;
    for(it = current.begin();it != current.end();it++)
    {
        string name = it->first;
        vector<int> v = it->second;
        cout << name << ": ";
        for (int i = 0; i < v.size(); ++i)
        {
            cout << v[i] << " ";
        }
        cout << endl;
    }
}

int Domains::findIndexOfVal(Variable& v,int val)
{
    for (int i = 0; i < D[v.name].size(); ++i)
    {
        if(D[v.name][i] == val)
            return i;
    }
    return -1;
}
