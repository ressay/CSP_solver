#include <iostream>
#include <map>
#include <vector>
#include "Variable.h"
#include "Domains.h"
#include "Solver.h"
#include "chrono"

using namespace std;

Solver generateCSPRandom(int numVariables,int domainSize, double difficulty);

Constraint generateRandomConstraint(int N,int M, double difficulty,bool self);

Solver generateNQueenProblem(int N);

void queenStats();

void otherStats();

long getMillisTime()
{
    chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(
            chrono::system_clock::now().time_since_epoch());
    return ms.count();
}

int main()
{
//    Solver s = generateNQueenProblem(13);
//    Solver s = generateCSPRandom(8,6,0.3);
//    s.print();

//    long t1 = getMillisTime();
//    map<string,int> res = s.solve(false);
//    long t2 = getMillisTime();
//    printf("solved in %ld\n",(t2-t1));
//    map<string,int>::iterator it;
//    cout << "result: " << endl;
//    for(it = res.begin(); it != res.end(); it++)
//        cout << it->first << ": " << it->second << endl;
//    queenStats();
    otherStats();
    return 0;
}

void otherStats()
{
    FILE* f = fopen("stats3PC2.csv","w");
    fprintf(f,"taille du domaine");
    for (int i = 4; i <=18 ; i+=1)
    {
        fprintf(f,";%d",i);
    }
    fprintf(f,"\ntemps sans heuristique (ms)");
    for (int i = 4; i <=18 ; i+=1)
    {
        double diff = i/20.;
        Solver s = generateCSPRandom(8,i,0.5);
        long t1 = getMillisTime();
        map<string,int> res = s.solve(false);
        long t2 = getMillisTime();
        fprintf(f,";%ld",t2-t1);
    }
    fprintf(f,"\ntemps avec heuristique (ms)");
    for (int i = 4; i <=18 ; i+=1)
    {
        double diff = i/20.;
        Solver s = generateCSPRandom(8,i,0.5);
        long t1 = getMillisTime();
        map<string,int> res = s.solve(true);
        long t2 = getMillisTime();
        fprintf(f,";%ld",t2-t1);
    }
}

void queenStats()
{
    FILE* f = fopen("pc3.csv","w");
    fprintf(f,"nombre de reines");
    for (int j = 4; j < 13; ++j)
    {
        fprintf(f,";%d",j);
    }
    fprintf(f,"\ntemps sans heuristique (ms)");
    for (int i = 4; i < 13; ++i)
    {
        Solver s = generateNQueenProblem(i);
        long t1 = getMillisTime();
        map<string,int> res = s.solve(false);
        long t2 = getMillisTime();
        fprintf(f,";%ld",t2-t1);
    }

//    iterated: 1728
//    iterated: 3456
//    iterated: 3456
//    iterated: 5184
//    iterated: 8640
//    iterated: 6912
//    iterated: 6912

    fprintf(f,"\ntemps avec heuristique (ms)");
    for (int i = 4; i < 13; ++i)
    {
        Solver s = generateNQueenProblem(i);
        long t1 = getMillisTime();
        map<string,int> res = s.solve(true);
        long t2 = getMillisTime();
        fprintf(f,";%ld",t2-t1);
    }
    fclose(f);
}

int random(int min,int max)
{
    return min + rand()%(max-min);
}



Solver generateCSPRandom(int numVariables,int domainSize, double difficulty)
{
    vector<Variable> X;
    srand(time(NULL));
    // vecteur de variables
    for (int i = 0; i < numVariables; ++i)
        X.emplace_back("q"+std::to_string(i));
    // creation des domaines de variables
    Domains D(X);
    for (int i = 0; i < numVariables; ++i)
    {
        vector<int> domain;
        // taille du domaine aleatoire en suivant le parametre "domainSize"
        int size = random(domainSize/2+1,domainSize*1.5);
        for (int j = 0; j < size; ++j)
            domain.push_back(j+1);
        D.setDomain(X[i],domain);
    }
    // creation de la class Solver avec les parametres variables/domaines
    Solver solver(D,X);
    // remplissage de la matrice Mp de contraintes
    for (int i = 0; i < numVariables; ++i)
        for (int j = i+1; j < numVariables; ++j)
        {
            int N = D[X[i]].size(),M = D[X[j]].size();
            // l'entree i,j de la matrice Mp a la taille N,M les tailles des domaines de variables i,j
            // on genere une contrainte aleatoire en suivant la difficultee donnee en parametre
            solver.setConstraint(i,j,generateRandomConstraint(N,M,difficulty,i==j));
        }
    return solver;
}

Constraint generateRandomConstraint(int N,int M, double difficulty,bool self)
{
    int thresh = difficulty*100;
    Constraint c = Constraint(N,M);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            // le booleen "self" designe si cette entree est reflexive (i,i)
            // dans ce cas si i!=j l'entree est à 0
            if(self && i!=j) c[i][j] = 0;
            // sinon un nombre aleatoire est generee si il est inferieur a un seuil generee a partir
            // de la difficultee on l'entree est à 0 sinon elle est à 1
            else if(random(0,100) <= thresh)
                c[i][j] = 0;
            else
                c[i][j] = 1;
        }
    }
    return c;
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