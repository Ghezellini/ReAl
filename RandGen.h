#pragma once
#include<random>;

class RandLab {

public:
    int Seed;
    std::default_random_engine Gen;

    double rand();  //continuous uniform destribution in [0,1]
    int randint(int, int);  //discrete uniform destribution in [a,b]
    double uniform(double, double); //continuous uniform destribution in [a,b]


    RandLab(int sd) {
        this->Seed = sd;
        this->Gen.seed(sd);
    }
    RandLab() {}
    ~RandLab() {}
};