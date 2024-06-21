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
#include "population.h"

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


//// 34 cities placed randomly on a circumference of radius 1 ////

   City citta(0, 0, 0);
   vector<City> viaggio_temp;

   int numc = 34;
   double theta=0;
   //double delta_theta = 2 * M_PI/numc;

   for(int i=0; i<numc; i++){
      theta = rnd.Rannyu(0, 2*M_PI);
      citta.x = cos(theta);
      citta.y = sin(theta);
      citta.index = i;
      viaggio_temp.push_back(citta);
      //theta += delta_theta;
   }

   Trip viaggio(viaggio_temp);

   Population sample(rnd, 200, viaggio, 0.8, 0.1, 0, 0.1, 0.1);

   ofstream out1;
   out1.open("lengths_circ.out");

   for(int i=0; i<1000; i++){
      sample.step();
      out1<<sample.get_best_trip().length()<<"\t"<<sample.half_av()<<endl;
   }

   out1.close();

   ofstream outvi;
   outvi.open("trip_circ.out");

   Trip t(sample.get_best_trip());

   for(int i=0; i<numc; i++){
      outvi<< t[i].index << "\t"<<t[i].x<<"\t"<<t[i].y<<endl;
   }

   outvi.close();


//// 34 cities placed randomly on a square of side 2 ////

   City citta2(0, 0, 0);
   vector<City> viaggio_temp2;

   int numc2 = 34;

   for(int i=0; i<numc2; i++){
      citta2.x = rnd.Rannyu(-1, 1);
      citta2.y = rnd.Rannyu(-1, 1);
      citta2.index = i;
      viaggio_temp2.push_back(citta2);
   }

   Trip viaggio2(viaggio_temp2);

   Population sample2(rnd, 200, viaggio2, 0.8, 0.1, 0, 0.1, 0.1);

   ofstream out2;
   out2.open("lengths_sq.out");

   for(int i=0; i<1000; i++){
      sample2.step();
      out2<<sample2.get_best_trip().length()<<"\t"<<sample2.half_av()<<endl;
   }

   out2.close();

   ofstream outsq;
   outsq.open("trip_sq.out");

   Trip t2(sample2.get_best_trip());

   for(int i=0; i<numc; i++){
      outsq<< t2[i].index << "\t"<<t2[i].x<<"\t"<<t2[i].y<<endl;
   }

   outsq.close();


   // sample.print();
   // cout<<endl<<endl;

   // for(int i=0; i<1000; i++){
   //    sample.step();
   //    cout<<"best l "<<sample.get_best_trip().length()<<endl<<"av l "<<sample.half_av()<<endl;
   // }

   // for(int i=0; i<numc; i++){
   //    cout<<sample.get_best_trip()[i].index<<" ";
   // }
   // cout<<endl<<sample.get_best_trip().length();
   // cout<<endl<<endl;

//end population constructor try run//

// //mutation debug try
//    for(int i=0; i<100;i++){
//    sample.step();
//    sample.print();
//    cout<<endl;
//    }
// //end mutation debug

// //crossover debug
//    for(int i=0; i<10; i++){
//       sample.step();
//       sample.print();
//       cout<<endl;
//    }
// //end crossover debug



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