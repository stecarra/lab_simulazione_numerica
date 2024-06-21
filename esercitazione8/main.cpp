/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <iostream>
#include <fstream>
#include <cmath>
#include "random.h"

using namespace std;

int n_tot;
int n_acc;

double delta; //to determine heuristically aiming for a 50% acceptance


double hamilton(double x, double mu, double sigma){
    double psi = exp(-pow(x-mu,2)/(2*pow(sigma,2))) + exp(-pow(x+mu,2)/(2*pow(sigma,2)));
    return (-0.5 * ((exp(-pow(x-mu,2)/(2*pow(sigma,2)))/pow(sigma,2))*(-1 + pow(x-mu,2)/pow(sigma,2)) 
           + (exp(-pow(x+mu,2)/(2*pow(sigma,2)))/pow(sigma,2))*(-1 + pow(x+mu,2)/pow(sigma,2))) + pow(x,4) - pow(x,2)*(double)5/2)/psi;
}


bool metro_step(double &x, double mu, double sigma, Random& rnd){
    n_tot ++;
    double xnew = rnd.Rannyu(x-delta, x+delta);
    double prob_old = pow(exp(-pow(x-mu,2)/(2*pow(sigma,2))) + exp(-pow(x+mu,2)/(2*pow(sigma,2))),2);
    double prob_new = pow(exp(-pow(xnew-mu,2)/(2*pow(sigma,2))) + exp(-pow(xnew+mu,2)/(2*pow(sigma,2))),2);
    
    if(prob_new > prob_old){
        x = xnew;
        n_acc ++;
        return true;
   
    }else{
        if(rnd.Rannyu() < (prob_new/prob_old)){
            x = xnew;
            n_acc ++;
            return true;
        }else{
            return false;
        }  
    }
}


int main(){

    Random rnd;
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()){
        Primes >> p1 >> p2 ;
    } else cerr << "PROBLEM: Unable to open Primes" << endl;
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open()){
        while ( !input.eof() ){
            input >> property;
            if( property == "RANDOMSEED" ){
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed,p1,p2);
            }
        } 
        input.close();
    } else cerr << "PROBLEM: Unable to open seed.in" << endl;

    n_tot = 0;
    n_acc = 0;

    delta = 1;  //should it depend on mu and sigma?

    double mu;
    double sigma;

    ifstream in;
    in.open("input.dat");
    string s;
    in >> s >> mu 
       >> s >> sigma;
    in.close();

    int N = 100; //number of blocks
    int m = 10000; //block length
    double H_av[N];
    bool flag;

    double x = rnd.Rannyu(-1, 1);

    //uncomment hist commands if you want to save some x
    //ofstream hist;
    //hist.open("sampled_x.out");

    double sum;
    for(int i=0; i<N; i++){
        sum = 0;
        for(int j= 0; j<m; j++){
            sum += hamilton(x, mu, sigma);
            do{
                flag = metro_step(x, mu, sigma, rnd);
            }while(!flag);
            //hist << x << endl;
        }
        H_av[i] = sum/m;
    }

    //hist.close();

    ofstream out;
    out.open("hamiltonian.out");

    rnd.BlockAv(H_av, N, out);

    cout << double(n_acc) / n_tot<<endl;


    rnd.SaveSeed();
    return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/