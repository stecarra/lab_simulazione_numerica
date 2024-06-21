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

   // for(int i=0; i<20; i++){
   //    cout << rnd.Rannyu() << endl;
   // }

//
// Ex 1.1
//

   int M=100000;              //Total number of throws
   int N=100;                 //Number of blocks
   int L=int(M/N);            //Number of throws in each block, please use for M a multiple of N


   double ave[N];            // dichiaro array statico delle medie di ciascun blocco
   double var[N];            // dichiaro array statico delle varianze di ciascun blocco
   double sum1, sum2;
   float current;

// riempio array dei blocchi ave
   for(int k=0; k<N; k++){
      sum1 = 0;
      sum2 = 0;    
      for(int i=0; i<L; i++){
         current = rnd.Rannyu();
         sum1 += current;
         sum2 += pow((current - 0.5),2);
      }
      ave[k] = sum1/L;
      var[k] = sum2/L;
   }

   double aveprog[N];        // dichiaro array statico delle medie progressive arrivati al blocco i-esimo
   double varprog[N];        // dichiaro array statico delle varianze progressive arrivati al blocco i-esimo

// riempio array delle medie progressive e delle varianze progressive
   for(int k=0; k<N; k++){
      sum1 = 0;
      sum2 = 0;
      for(int i=0; i<=k; i++){
         sum1 += ave[i];
         sum2 += var[i];
      }
      aveprog[k] = sum1 / (k+1);
      varprog[k] = sum2 / (k+1);
   }

   double sumav;         
   double sumav2;
   double sumvar;         
   double sumvar2; 

   double varoav[N];              //dichiaro array statico della varianza della media al blocco i-esimo
   double varovar[N];             //dichiaro array statico della varianza della varianza al blocco i-esimo

//riempio array delle varianze della media e della varianza
   for(int k=0; k<N; k++){
      sumav= 0;
      sumav2= 0;
      sumvar = 0;
      sumvar2 = 0;
      for(int i=0; i<= k; i++){
         sumav += ave[i];
         sumav2 += pow(ave[i],2);
         sumvar += var[i];
         sumvar2 += pow(var[i],2);
      }

      varoav[k] = sumav2 / (k+1) - pow(sumav / (k+1), 2);
      varovar[k] = sumvar2 / (k+1) - pow(sumvar / (k+1), 2);
   }

//scrivo su un file media progressiva, deviazione standard della media,
//varianza progressiva e deviazione standard della varianza 
//(dove le deviazioni sono calcolate per blocking)
   ofstream out;
   out.open("data");
   out<< 1 <<"\t"<<ave[0]<<"\t"<<0<<"\t"<< 1/ double(12) << "\t"<<0<<endl;
   for(int i=1; i<N; i++){
      out<< i+1 <<"\t"<< aveprog[i] <<"\t"<< sqrt(varoav[i]/i) <<"\t"
                      << varprog[i] <<"\t"<< sqrt(varovar[i]/i) <<endl;
   }
   out.close();

// calcolo 100 volte il chi quadro per 10^4 numeri random 
// per controllare che la distribuzione sia uniforme


   int numbin = 100;
   long n[numbin];                     //istogramma da 0 a 1 di 100 bins

   double chi_s[100];                  //array statico di chi quadri
   int index;
   double sum;


   for(int j=0; j<100; j++){
      for(int i = 0; i<numbin; i++){
         n[i] = 0;
      }
      for(int k=0; k<10000; k++){
         current = rnd.Rannyu();
         index = (current * numbin) / 1;
         n[index] += 1;
      }
      sum = 0;
      for(int k = 0; k<numbin; k++){
         sum += pow((n[k] - 10000/numbin),2);
      }
      chi_s[j]= sum / (10000/numbin);
   }


//stampo i 100 chi quadri trovati in un file per vedere se seguono la distribuzione aspettata
   ofstream chi;
   chi.open("chi.out");
   for(int i=0; i<100; i++){
      chi<<i+1<<"\t"<<chi_s[i]<<endl;
   }
   chi.close();

//
//Ex 1.2
//


//scrivo su un file 4 colonne con 10000 rispettivamente numeri distribuiti uniformemente, medie tra due,
//10 e 100 numeri distribuiti uniformemente
   ofstream uni;
   uni.open("uni.out");
   double s;
   for(int i=0; i<10000; i++){
      uni<<rnd.Rannyu()<<"\t"<<(rnd.Rannyu()+rnd.Rannyu())/2<<"\t";
      s=0;
      for(int k=0; k<10; k++){
         s+= rnd.Rannyu();
      }
      uni<< s/10 <<"\t";
      s=0;
      for(int k=0; k<100; k++){
         s+= rnd.Rannyu();
      }
      uni<< s/100 <<endl;
   }
   uni.close();

//ripeto la stessa cosa per valori generati da distribuzione esponenziale con lamnda=1....
   ofstream exp;
   exp.open("exp.out");

   for(int i=0; i<10000; i++){
      exp<<rnd.Exp(1)<<"\t"<<(rnd.Exp(1)+rnd.Exp(1))/2<<"\t";
      s=0;
      for(int k=0; k<10; k++){
         s+= rnd.Exp(1);
      }
      exp<< s/10 <<"\t";
      s=0;
      for(int k=0; k<100; k++){
         s+= rnd.Exp(1);
      }
      exp<< s/100 <<endl;
   }
   exp.close();

//e da distribuzione lorentziana con mu=0 e gamma=1
   ofstream lor;
   lor.open("lor.out");

   for(int i=0; i<10000; i++){
      lor<<rnd.Lor(1,0)<<"\t"<<(rnd.Lor(1,0)+rnd.Lor(1,0))/2<<"\t";
      s=0;
      for(int k=0; k<10; k++){
         s+= rnd.Lor(1,0);
      }
      lor<< s/10 <<"\t";
      s=0;
      for(int k=0; k<100; k++){
         s+= rnd.Lor(1,0);
      }
      lor<< s/100 <<endl;
   }
   lor.close();

//magari c'è un modo per fare i tre casi in un colpo sol
//magari con switch case? boh


//
//Ex 1.3
//

//simulo l'esperimento di Buffon

   float d = 3;     //pongo che le linee distano 3 e la lunghezza dell'ago è pari a 1

   long T_over = 0;      //T_over numero di volte che l'ago incrocia le linee
   float P = 0;
   long T_tot =0;        //T_tot numero totale di lanci

   M=10000000;           //Total number of throws
   N=100;                 //Number of blocks
   L=int(M/N);      

   float x;        //x1 posizione di un capo dell'ago
   float a, b;  //a,b numeri tra -1 e 1 per estrarre coefficiente angolare
   double m;        //m coefficiente angolare

   double avpig[100];
   float varpig[100];
   float avpigprog[100];


   ofstream pig;
   pig.open("pigreco.out");

   for(int i=0; i<N; i++){
      sum1=0;
      sum2=0;
      for(int j=0; j<L; j++){
         T_tot ++;
         x=rnd.Rannyu(1.5, 3);

         if(rnd.Rannyu()<0.5){
            do{
               a=rnd.Rannyu(-1,1);
               b=rnd.Rannyu(-1,1);
            }while(a==0 || (pow(a,2)+ pow(b,2))>1);

            m=b/a;

            if(x+ sqrt(1/(1 + pow(m,2))) > 3){
               T_over++;
            }
         }


         P = double(T_over)/T_tot;
         if(P!=0){
            sum1 += 2/(P*d);
         }else{
            sum1 += 0;
         }
         avpig[i] = sum1 / L;
      }
   }


   
   sum1=0;
   sum2=0;
   for(int i=0; i<N; i++){
      cout<<avpig[i]<<endl;
      sum1 = sum1 + avpig[i];
      sum2+=pow(avpig[i],2);
      avpigprog[i]= sum1 / (i+1);
      varpig[i]= sum2/(i+1) - pow(avpigprog[i],2);
   }

   pig<<0<<"\t"<<avpigprog[0]<<"\t"<<0<<endl;
   for(int i=1; i<N; i++){
         pig<<i<<"\t"<<avpigprog[i]<<"\t"<<sqrt(varpig[i]/i)<<endl;
   }

   pig.close();

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
