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
#include <cstdlib>
#include <vector>

#include "trip.h"

using namespace std;

//Trip class functions

Trip :: Trip(std::vector<City> c){
    _numci = c.size();
    _cities = c; // lo copia o lo deep copia?
}

Trip :: ~Trip(){};

void Trip :: set_city(int index, City c){
    if(index<0 || index>= _numci) cerr<<"set_city failure: index out of bound" <<endl;
    _cities[index] = c;

}

void Trip :: set_cities(std::vector<City> c){
    _cities = c; // come lo copia?
    if(!this->check()) cerr<<"set_city failure: new trip does not satisfy tsp conditions" <<endl;
}

City Trip :: operator[](int i){
    return _cities[i];
}

int Trip :: get_numci(){
    return _numci;
}

bool Trip :: check(){
    bool flag;
    //check if first city is number one
    if(_cities[0].index != 0){
        std::cerr << "The first city in the trip is not the travelling salesman's home" << std::endl;
        return false;
    }

    //check if ts visits every city
    for(int i = 0; i<_numci; i++){
        flag = false;
        for(int j = 0; j<_numci; j++){
            if(_cities[j].index == i) flag = true;
        }
        if(!flag){
            std::cerr << "ts does not visit city " << i <<std::endl;
            return false;
        }
    }

    return true;
}

//returning L2 of the trip
double Trip :: length(){
    double l = 0;
    for(int i=0; i<_numci-1; i++){
        l += sqrt(pow(_cities[i+1].x - _cities[i].x, 2) + pow(_cities[i+1].y - _cities[i].y, 2));
    }
    l += sqrt(pow(_cities[_numci-1].x - _cities[0].x, 2) + pow(_cities[_numci-1].y - _cities[0].y, 2));

    return l;
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