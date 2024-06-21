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
#include <string>
#include <cmath>
#include "random.h"

using namespace std;


double massi(double a, double b){
    if(a>b) return a;
    return b;
}



 
int main (int argc, char *argv[]){

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

   // for(int i=0; i<20; i++){
   //    cout << rnd.Rannyu() << endl;
   // }


//
// Ex 3.1
//

   int M=100000;              //Total number of throws
   int N=100;                 //Number of blocks
   int L=int(M/N);            //Number of throws in each block, please use for M a multiple of N


   double call[N];   
   double put[N];         
   double var_call[N];           
   double var_put[N];
   double calldisc[N];
   double putdisc[N];
   double var_calldisc[N];
   double var_putdisc[N];
   double sumc1;
   double sumcdisc1;
   double sump1;
   double sumpdisc1;
   float S;

// riempio array dei blocchi
   for(int k=0; k<N; k++){
      sumc1 = 0;
      sump1 = 0; 
      sumcdisc1 = 0;
      sumpdisc1 = 0;   
      for(int i=0; i<L; i++){
         S = 100 * exp((0.1 - pow(0.25,2)/2) + 0.25 * rnd.Gauss(0,1));    // valuto S(T) con formula montecarlo diretta
         sumc1 += exp(-0.1) * massi(0, S - 100);
         sump1 += exp(-0.1) * massi(0, 100 - S);
         S = 100;
         for(int j = 0; j<100; j++){                    // valuto s(T) discretizzando il tempo e suddividendo il periodo in 100 intervalli
            S = S * exp((0.1 - pow(0.25,2)/2)*0.01 + 0.25*rnd.Gauss(0,1)*sqrt(0.01));
         }
         sumcdisc1 += exp(-0.1) * massi(0, S - 100);
         sumpdisc1 += exp(-0.1) * massi(0, 100 - S);
      }
      call[k] = sumc1/L;
      put[k] = sump1/L;
      calldisc[k] = sumcdisc1/L;
      putdisc[k] = sumpdisc1/L;
   }

   double callprog[N];        
   double putprog[N];        
   double calldiscprog[N];
   double putdiscprog[N];

// riempio array delle medie progressive
   for(int k=0; k<N; k++){
      sumc1 = 0;
      sump1 = 0;
      sumcdisc1 = 0;
      sumpdisc1 = 0;
      for(int i=0; i<=k; i++){
         sumc1 += call[i];
         sump1 += put[i];
         sumcdisc1 += calldisc[i];
         sumpdisc1 += putdisc[i];
      }
      callprog[k] = sumc1 / (k+1);
      putprog[k] = sump1 / (k+1);
      calldiscprog[k] = sumcdisc1 / (k+1);
      putdiscprog[k] = sumpdisc1 / (k+1);
   }

   double sumc;         
   double sumcsq;
   double sump;         
   double sumpsq;
   double sumcdisc;         
   double sumcsqdisc;
   double sumpdisc;         
   double sumpsqdisc;

//    double varoav[N];              //dichiaro array statico della varianza della media al blocco i-esimo
//    double varovar[N];             //dichiaro array statico della varianza della varianza al blocco i-esimo

//riempio array delle varianze
   for(int k=0; k<N; k++){
      sumc = 0;
      sumcsq = 0;
      sump = 0;
      sumpsq = 0;
      sumcdisc = 0;
      sumcsqdisc = 0;
      sumpdisc = 0;
      sumpsqdisc = 0;
      for(int i=0; i<= k; i++){
         sumc += call[i];
         sumcsq += pow(call[i],2);
         sump += put[i];
         sumpsq += pow(put[i],2);
         sumcdisc += calldisc[i];
         sumcsqdisc += pow(calldisc[i],2);
         sumpdisc += putdisc[i];
         sumpsqdisc += pow(putdisc[i],2);
      }

      var_call[k] = sumcsq / (k+1) - pow(sumc / (k+1), 2);
      var_put[k] = sumpsq / (k+1) - pow(sump / (k+1), 2);
      var_calldisc[k] = sumcsqdisc / (k+1) - pow(sumcdisc / (k+1), 2);
      var_putdisc[k] = sumpsqdisc / (k+1) - pow(sumpdisc / (k+1), 2);

   }

//scrivo su un file media progressiva, deviazione standard della media,
//varianza progressiva e deviazione standard della varianza 
//(dove le deviazioni sono calcolate per blocking)
   ofstream out;
   out.open("data");
   out<< 1 <<"\t"<<callprog[0]<<"\t"<<0<<"\t"<< calldiscprog[0] << "\t"<<0<<"\t"
      <<putprog[0]<<"\t"<<0<<"\t"<<putdiscprog[0]<<"\t"<<0<<endl;
   for(int i=1; i<N; i++){
        out<< i <<"\t"<<callprog[i]<<"\t"<<sqrt(var_call[i]/i)<<"\t"
                << calldiscprog[i] << "\t"<<sqrt(var_calldisc[i]/i)<<"\t"
                <<putprog[i]<<"\t"<<sqrt(var_put[i]/i)<<"\t"
                <<putdiscprog[i]<<"\t"<<sqrt(var_putdisc[i]/i)<<endl;
   }
   out.close();




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
