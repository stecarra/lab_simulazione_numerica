/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include "mpi.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "random.h"
#include "population.h"

using namespace std;
 
int main(int argc, char *argv[]){


   //file where the first column stores best lengths and second column
   //stores how much time it required
   //every five rows the number of processors grows by 1, starting from 1
   std::ofstream outfile("times_p.out", std::ios::app);

   // Check if the file is open
   if (!outfile.is_open()) {
      std::cerr << "Failed to open the file." << std::endl;
      return 1;
    }

   int size, rank;
   MPI_Init(&argc,&argv); 
   MPI_Comm_size(MPI_COMM_WORLD, &size); 
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   double tstart = MPI_Wtime();

//random initialization, can use the same seeds for every process
//the important thing is that the chromosomes in the first generation are 
//generated differently in each continent, but this is already the case using 
//std shuffle method

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


//// 110 provinces of Italy ////

   City citta(0, 0, 0);
   double x, y;
   vector<City> viaggio_temp;

   int numc = 110;

   ifstream in;
   in.open("cap_prov_ita.dat");

   for(int i=0; i< numc; i++){
      in >> x >> y;
      citta.x = x;
      citta.y = y;
      citta.index = i;
      viaggio_temp.push_back(citta);
   }

   in.close();

   Trip viaggio(viaggio_temp);

   Population sample(rnd, 200, viaggio, 0.8, 0.1, 0, 0.1, 0.1);

   //ofstream out1;
   //out1.open("lengths_prov_1p.out");

   int numgen = 20;
   int numcomm = 500;

   double best_len = 1000;
   //double target_len = 135;
   double temp_len;

   int OwnBestTrip[numc]; // modificare affinchè si cambino i best trip e non un numero a caso
   int temp[numc];

   for(int i=0; i<numc; i++){
      OwnBestTrip[i] = sample.get_best_trip()[i].index;
   }

   for(int j =0; j< numcomm; j++){
      for(int i=0; i<numgen; i++){
         if (rank == 0) cout <<"completamento: " << (double(j) * numgen + double(i))/ (numgen*numcomm) <<endl;
         sample.step();

         temp_len = sample.get_best_trip().length();
         if (temp_len<best_len) best_len = temp_len; 

         //out1<<sample.get_best_trip().length()<<"\t"<<sample.half_av()<<endl;
      }

      for(int i=0; i<numc; i++){
         OwnBestTrip[i] = sample.get_best_trip()[i].index;
      }

      int next_rank = (rank + 1) % size;
      int prev_rank = (rank - 1 + size) % size;

      // //stampa best trip per rank 0
      // if(rank ==0){
      //    for(int t=0; t<numc; t++){
      //       cout<<sample.get_best_trip()[t].index<<" ";
      //    }
      //    cout<<endl;
      // }

      MPI_Status status;
      MPI_Sendrecv(OwnBestTrip, numc, MPI_INT, next_rank, 0, 
                   temp, numc, MPI_INT, prev_rank, 0, 
                   MPI_COMM_WORLD, &status);


      for(int i=0; i<numc; i++){
         OwnBestTrip[i] = temp[i];
      }

      // set new own best trip
      vector<City> new_best_trip;
      int ind;

      for(int i=0; i<numc; i++){
         ind=0;
         while(OwnBestTrip[i] != viaggio_temp[ind].index){
            ind++;
         }
         new_best_trip.push_back(viaggio_temp[ind]);
      }

      Trip new_best(new_best_trip);

      sample.set_best_trip(new_best);

      // //stampa best trip per rank 1
      // if(rank == 1){
      //    for(int t=0; t<numc; t++){
      //       cout<<sample.get_best_trip()[t].index<<" ";
      //    }
      //    cout<<endl<<endl;
      // }

   }

   //out1.close();

  


   if(rank == 0){
      ofstream outvi;
      outvi.open("trip_prov_4p.out");

      outvi << best_len<<endl;

      Trip t(sample.get_best_trip());

      for(int i=0; i<numc; i++){
         outvi<< t[i].index << "\t"<<t[i].x<<"\t"<<t[i].y<<endl;
      }

      outvi.close();
   }

   int irecv[size];

   for(int i=0;i<size;i++){
      irecv[i]=0; 
   }
   
   int isend = best_len;

   MPI_Gather(&isend,1,MPI_INTEGER,irecv,1,MPI_INTEGER,0, MPI_COMM_WORLD);

   if(rank == 0){
      for(int i=0; i<size; i++){
         if(irecv[i]<best_len) best_len = irecv[i];
      }
      cout << "this is process "<<rank <<" speaking : \n";
      cout <<"best lengths:"<<endl;
      for(int i=0;i<size;i++){
         cout << irecv[i] << endl; 
      }
      cout<<"absolute best:"<<endl<<best_len<<endl;
   }


   rnd.SaveSeed();

   double tend = MPI_Wtime();

   if(rank == 0){
      cout<<"il tempo trascorso a cercare è stato "<< tend - tstart <<endl;
      //uncomment following line if you want to add info to times_p.out
      //outfile << best_len << "\t" << tend - tstart <<endl;
   }

 
   MPI_Finalize();

   outfile.close();

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