/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#ifndef __Population__
#define __Population__

#include "trip.h"

class Population {

    private:
    std::vector<Trip> _trips;
    Random _rnd;
    int _numchrom;
    int _numci;
    float _p_c;
    float _p_m_1;
    float _p_m_2;
    float _p_m_3;
    float _p_m_4;
    bool _is_same(Trip, Trip);
    Trip _select();
    void _crossover(Trip&, Trip&); //serve qualcos'altro per fare crossover come argomento?
    void _mutate_1(Trip&); //pair permutation 
    //void _mutate_2(Trip&); //shift 
    void _mutate_3(Trip&); //groups permutation 
    void _mutate_4(Trip&); //inversion 
    //void _sel_sort();

    public:
    Population(Random&, int, Trip, float, float, float, float, float);
    ~Population();
    void print();
    Trip get_best_trip();
    double half_av();
    int get_numchrom();
    void step();

};

#endif  //__Population__
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/