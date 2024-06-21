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



//
// Ex 2.1
//

   int M=1000000;             //Total number of throws
   int N=100;                 //Number of blocks
   int L=int(M/N);            //Number of throws in each block, please use for M a multiple of N


   double I[N];            // dichiaro array statico delle medie integrali di ciascun blocco
   double I_imp[N];        // dichiaro array statico delle medie integrali di ciascun blocco con importance sampling
   double sum1;
   double sum2;
   float current;
   float x_ran;

// riempio array dei blocchi I 
   for(int k=0; k<N; k++){
      sum1 = 0; 
      sum2 = 0;                  //non so se è corretto fare blocking così, dove in ogni blocco mi dimentico del precedente...
      for(int i=0; i<L; i++){
         current = (M_PI/2)* cos((M_PI/2)*rnd.Rannyu());
         sum1 += current;
         x_ran = 1-sqrt(1-rnd.Rannyu());  //pseudo random number sampled between 0 and 1 following -2x+2
         current = (M_PI/2)* cos((M_PI/2)*(x_ran))/(-2*(x_ran)+2);        
          // dove la pdf scelta per l'importance sampling è -2x +2 e ho usato inverse transform
         sum2 += current;
      }
      I[k] = sum1/L;
      I_imp[k] = sum2/L;
   }

   double Iprog[N];        // dichiaro array statico dell' integrale determinato 
                           // progressivamente arrivati al blocco i-esimo
   double I_impprog[N];

// riempio array delle medie progressive
   for(int k=0; k<N; k++){
      sum1 = 0;
      sum2 = 0;
      for(int i=0; i<=k; i++){
         sum1 += I[i];
         sum2 += I_imp[i];
      }
      Iprog[k] = sum1 / (k+1);
      I_impprog[k] = sum2 / (k+1);
   }

   double sumI;         
   double sumI2;
   double sumI_imp;
   double sumI_imp2;


   double varoI[N];              //dichiaro array statico della varianza della media al blocco i-esimo
   double varoI_imp[N];

//riempio array delle varianze della media
   for(int k=0; k<N; k++){
      sumI= 0;
      sumI2= 0;
      sumI_imp= 0;
      sumI_imp2= 0;

      for(int i=0; i<= k; i++){
         sumI += I[i];
         sumI2 += pow(I[i],2);
         sumI_imp += I_imp[i];
         sumI_imp2 += pow(I_imp[i],2);
      }

      varoI[k] = sumI2 / (k+1) - pow(sumI / (k+1), 2);
      varoI_imp[k] = sumI_imp2 / (k+1) - pow(sumI_imp / (k+1), 2);
   }

//scrivo su un file integrale progressivo, deviazione standard della media,
//(dove le deviazioni sono calcolate per blocking)
   ofstream out;
   out.open("integral_uni.out");
   out<< 1 <<"\t"<<I[0]<<"\t"<<0<<"\t"<<I_imp[0]<<"\t"<<0<<"\t"<<endl;
   for(int i=1; i<N; i++){
      out<< i+1 <<"\t"<< Iprog[i] <<"\t"<< sqrt(varoI[i]/i) <<"\t"
         << I_impprog[i] <<"\t"<< sqrt(varoI_imp[i]/i)<<endl;
   }
   out.close();


//
// Ex 2.2
//


   int A=10000;               //Total number of throws
   int B=100;                 //Number of blocks
   int C=int(A/B);            //Number of throws in each block, please use for M a multiple of N


   double RW[B][100];         // dichiaro matrice statica delle distanze di ciascun blocco (prima []) e per ciascun passo (seconda [])
   double RW_con[B][100];     // dichiaro matrice statica delle distanze continue di ciascun blocco (prima []) e per ciascun passo (seconda [])

    for(int i = 0; i<B; i++){
        for(int j = 0; j<100; j++){
            RW[i][j] = 0;
            RW_con[i][j] = 0;
        }
    }

   double x, y, z;
   double current_phi;
   double current_theta;
   double select_coord;

// riempio matrici dei blocchi  
   for(int k=0; k<B; k++){                
      for(int i=0; i<C; i++){
          x=0;
          y=0;
          z=0;
          // simulo random walk discreta
          for(int j=0; j<100; j++){
              RW[k][j] += (pow(x,2) + pow(y,2)+ pow(z,2));
              current = 1;
              if(rnd.Rannyu()<0.5) current = -1;
              select_coord = rnd.Rannyu();
              if( select_coord < 1/3){
                  x += current;
              }else if(select_coord > 1/3 && select_coord < 2/3){
                  y += current;
              }else{
                  z += current;
              }
          }
          x=0;
          y=0;
          z=0;
          // simulo random walk continua
          for(int j=0; j<100; j++){
              RW_con[k][j] += (pow(x,2) + pow(y,2)+ pow(z,2));

              current_theta = rnd.Rannyu(0, 2 * M_PI);
              current_phi = acos(2* rnd.Rannyu() - 1);

              x += sin(current_theta) * cos(current_phi);
              y += sin(current_theta) * sin(current_phi);
              z += cos(current_theta);
              }
          }

      for(int t =0; t<100; t++){
          RW[k][t] = sqrt(RW[k][t] / C);
          RW_con[k][t] = sqrt(RW_con[k][t] / C);

      }
   }

   double RWav[100] = {};        // dichiaro array statico delle medie della distanza raggiunta dalla RW dopo i passi 
   double RW_conav[100] ={};

// riempio array delle medie
   for(int k=0; k<B; k++){
       for(int l = 0; l<100; l++){
           RWav[l] += RW[k][l];
           RW_conav[l] += RW_con[k][l];
       }
    }
    for(int l = 0; l<100; l++){
        RWav[l] = RWav[l] / B;
        RW_conav[l] = RW_conav[l] / B;
    }

//calcolo le varianze per ogni step della RW
   double sumRW[100] = {};         
   double sumRW2[100] = {};
   double sumRW_con[100] = {};
   double sumRW_con2[100] = {};


   double varoRW[100];              
   double varoRW_con[100];

//riempio array delle varianze della media
   for(int i=0; i<B; i++){
      for(int k=0; k< 100; k++){
         sumRW[k] += RW[i][k];
         sumRW2[k] += pow(RW[i][k],2);
         sumRW_con[k] += RW_con[i][k];
         sumRW_con2[k] += pow(RW_con[i][k],2);
      }
   }
    for(int k=0; k< 100; k++){
        varoRW[k] = sumRW2[k] / B - pow(sumRW[k] / B, 2);
        varoRW_con[k] = sumRW_con2[k] / B - pow(sumRW_con[k] / B, 2);
    }


//scrivo su un file integrale progressivo, deviazione standard della media,
//(dove le deviazioni sono calcolate per blocking)
   out.open("RW.out");
   for(int i=0; i<100; i++){
      out<< i <<"\t"<< RWav[i] <<"\t"<< sqrt(varoRW[i]/B) <<"\t"
         << RW_conav[i] <<"\t"<< sqrt(varoRW_con[i]/B)<<endl;
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
