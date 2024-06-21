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
#include <algorithm>
#include <random>

#include "population.h"

using namespace std;

//Population class functions

Population :: Population(Random& rnd, int numchrom, Trip t, float p_c, float p_m_1, float p_m_2, float p_m_3, float p_m_4){
    _rnd = rnd;
    if((numchrom%2)!= 0) cerr<<"number of chromosome must be even!" << endl;
    _numchrom = numchrom;
    _numci = t.get_numci();
    if((_numci == 0 || _numci == 1) || _numci == 2) cerr << "number of cities too low, the path is trivial";
    _p_c = p_c;
    _p_m_1 = p_m_1;
    _p_m_2 = p_m_2;
    _p_m_3 = p_m_3;
    _p_m_4 = p_m_4;

    _trips.push_back(t);

    vector<int> ind;
    vector<City> temp_trip;

    for(int i=1; i < t.get_numci(); i++ ){
        ind.push_back(i);
    }

    Trip tr(t);
    random_device rd; //questo random device non mi piace perchè ogni volta usa dei seed diversi
    for(int i=1; i<_numchrom + 1; i++){
        mt19937 g(rd());
        shuffle(ind.begin(), ind.end(), g);

        temp_trip.push_back(t[0]);

        for(int j=0; j<t.get_numci()-1; j++){
            temp_trip.push_back(t[ind[j]]);
        }

        tr.set_cities(temp_trip);
        tr.check();
        _trips.push_back(tr);
        temp_trip.clear();
    }

    _trips.erase(_trips.begin());
}

Population :: ~Population(){}

void Population :: print(){
    for(int i=0; i<_numchrom; i++){
        for(int j=0; j<_numci; j++){
            cout << _trips[i][j].index <<" ";
        }
        cout<<endl;
    }
}

int Population :: get_numchrom(){
    return _numchrom;
}

Trip Population :: get_best_trip(){
    int a = 0;
    double l = _trips[0].length();

    for(int i=1; i<_numchrom; i++){
        if(_trips[i].length()<l){
            l = _trips[i].length();
            a=i;
        }
    }
    return _trips[a];
}

double Population :: half_av(){
    vector<double> lengths;

    for(int i=0; i<_numchrom; i++){
        lengths.push_back(_trips[i].length());
    }

    sort(lengths.begin(), lengths.end());

    double sum=0;
    for(int i=0; i< _numchrom/2; i++) sum += lengths[i];

    return sum/(_numchrom/2);
}

bool Population :: _is_same(Trip t1, Trip t2){
    for(int i=0; i<_numci; i++){
        if(t1[i].index != t2[i].index) return false;
    }

    return true;
}

Trip Population :: _select(){
    float P[_numchrom];
    double totlen = 0;
    double l;

    for(int i=0; i<_numchrom; i++){
        l = exp(- _trips[i].length());
        P[i] = l;
        totlen += l;
    }

    for(int i=0; i<_numchrom; i++){
        P[i] /= totlen;
    }

    float s = P[0];
    int ind = 0;
    float rand = _rnd.Rannyu();

    while(s < rand){
        ind++;
        s+= P[ind];
    }

    return _trips[ind];
}

void Population :: step(){
    vector<Trip> offspring;

//numchrom must be an even number
    for(int i=0; i<_numchrom/2; i++){
        offspring.push_back(this->_select());
        offspring.push_back(this->_select());
        while(this->_is_same(offspring[2*i],offspring[2*i+1])){
            offspring.pop_back();
            offspring.push_back(this->_select());
        }

        if(_rnd.Rannyu()<_p_c)   this->_crossover(offspring[2*i],offspring[2*i+1]);
        if(_rnd.Rannyu()<_p_m_1) this->_mutate_1(offspring[2*i]);
        if(_rnd.Rannyu()<_p_m_1) this->_mutate_1(offspring[2*i+1]);
//        if(_rnd.Rannyu()<_p_m_2) this->_mutate_2(offspring[2*i]);
//        if(_rnd.Rannyu()<_p_m_2) this->_mutate_2(offspring[2*i+1]);
        if(_rnd.Rannyu()<_p_m_3) this->_mutate_3(offspring[2*i]);
        if(_rnd.Rannyu()<_p_m_3) this->_mutate_3(offspring[2*i+1]);
        if(_rnd.Rannyu()<_p_m_4) this->_mutate_4(offspring[2*i]);
        if(_rnd.Rannyu()<_p_m_4) this->_mutate_4(offspring[2*i+1]);
    }

    _trips = offspring;

}

void Population :: _crossover(Trip& t1, Trip& t2){
    int a;
    a = int(_rnd.Rannyu()*(_numci -1)) + 1;

    vector<City> copia1;
    vector<City> copia2;
    for(int i=0; i<_numci; i++){
        copia1.push_back(t1[i]);
        copia2.push_back(t2[i]);
    }

    vector<City> coda1;
    vector<City> coda2;
    for(int i=0; i<_numci - a; i++){
        coda1.push_back(t1[a+i]);
        coda2.push_back(t2[a+i]);
    }

    int ind = 0;
    for(int i=0; i<_numci; i++){
        for(int j=0; j<_numci - a; j++){
            if(copia1[i].index == coda2[j].index){
                t2.set_city(a+ind,copia1[i]);
                ind++;
                break;
            }
        }
    }

    ind = 0;
    for(int i=0; i<_numci; i++){
        for(int j=0; j<_numci - a; j++){
            if(copia2[i].index == coda1[j].index){
                t1.set_city(a+ind,copia2[i]);
                ind++;
            }
        }
    }

    if(!t1.check()) cerr<<"crossover failure: new trip does not satisfy tsp conditions" << endl;
    if(!t2.check()) cerr<<"crossover failure: new trip does not satisfy tsp conditions" << endl;
}

void Population :: _mutate_1(Trip& t){
    int a, b;

    //for contiguos indixes
    // a = int(_rnd.Rannyu()*(t.get_numci()-1)) + 1;
    // b = (1 + (a-1))%(_numci - 1) + 1;

    do{
    a = int(_rnd.Rannyu()*(t.get_numci()-1)) + 1;
    b = int(_rnd.Rannyu()*(t.get_numci()-1)) + 1;
    }while(a==b);

    City temp_ci = t[a];

    t.set_city(a, t[b]);
    t.set_city(b, temp_ci);

    if(!t.check()) cerr<<"mutation 1 failure: new trip does not satisfy tsp conditions" << endl;
}

// void Population :: _mutate_2(Trip& t){
//     int m, n, a;
//     // m = int(_rnd.Rannyu()*(_numci)) + 1;
//     // a = int(_rnd.Rannyu()*(_numci - 1));
//     // n = int(_rnd.Rannyu()*(_numci - 2)) + 1;
//     m=2;
//     a=0;
//     n=1;

//     cerr<<m<<" "<<n<<" "<<a<<endl;

//     vector<City> temp_ci;
//     for(int i= 0; i<_numci; i++){
//         temp_ci.push_back(t[i]);
//     }

//     for(int i=0; i<m; i++){
//         temp_ci[1 + (a+n+i)%(_numci-1)] = t[1 + (a+i)%(_numci-1)];
//     }


//     for(int i=1; i<a+1;i++){
//         temp_ci[i] = t[i];
//     }

//     for(int i=0; i<_numci-1-a-m; i++){
//         temp_ci[a + 1 + (a+n+m+i)%(_numci-1)] = t[1 + (a+m+i)%(_numci-1)];
//     }

//     for(int i=0; i<_numci; i++){
//         t.set_city(i, temp_ci[i]);
//     }

//     if(!t.check()) cerr<<"mutation 2 failure: new trip does not satisfy tsp conditions" << endl;
// }

void Population :: _mutate_3(Trip& t){
    int m, a, b;
    m = int(_rnd.Rannyu() * ((_numci-1)/2)) + 1; //there must be a non trivial number of cities
    if(m >= double(_numci)/2) cerr<<"mutation 3 failure: number of contiguos cities to large" << endl;

    a = int(_rnd.Rannyu()*(_numci -1)) + 1;
    b = (int(_rnd.Rannyu()*((_numci - 1 - m) + 1)) + a + m )%(_numci - 1);
    
    City temp_ci(0,0,0);

    for(int i=0; i<m ; i++){
        temp_ci = t[1+ (a+i)%(_numci-1)];
        t.set_city(1+ (a+i)%(_numci-1), t[1+(b+i)%(_numci-1)]);
        t.set_city(1+(b+i)%(_numci-1), temp_ci);
    }

    if(!t.check()) cerr<<"mutation 3 failure: new trip does not satisfy tsp conditions" << endl;
}

void Population :: _mutate_4(Trip& t){
    int m, a;
    m = int(_rnd.Rannyu()*(_numci - 2)) + 2;
    a = int(_rnd.Rannyu()*(_numci - m)) + 1;

    vector<City> temp_ci;
    for(int i= 0; i<_numci; i++){
        temp_ci.push_back(t[i]);
    }


    for(int i = 0; i<m; i++){
        temp_ci[i+a] = t[i+a];
    }

    for(int i = 0; i<m; i++){
        t.set_city(a+i, temp_ci[a+m-i-1]);
    }

    if(!t.check()) cerr<<"mutation 4 failure: new trip does not satisfy tsp conditions" << endl;
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