/*--------------------------------------------------------------------------*/
/*----------------------------- File main.cpp ------------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * The main file implementation of the  ReAl problem.
 *
 * \version 0.1
 *
 * \date 24 - 02 - 2022
 *
 * \author Amin Asadi \n
 *         Department of \n
 *         Industrial Engineering and Business Information System  \n
 *         University of Twente \n
 *
 * \author Ali Ghezelsoflu \n
 *         Department of  \n
 *         Industrial Engineering and Business Information System  \n
 *         University of Twente \n
 *
 *
 * Copyright &copy by Amin Asadi, Ali Ghezelsoflu
 */
/*--------------------------------------------------------------------------*/
/*---------------------------- IMPLEMENTATION ------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <math.h>
#include "TestState.h"
#include "TestAlgorithm.h"
#include "MersenneTwister.h"

/*--------------------------------------------------------------------------*/
/*------------------------- NAMESPACE AND USING ----------------------------*/
/*--------------------------------------------------------------------------*/


int main(int argc, const char * argv[]) {

    int N = 19;
    double MPG = 28;
    double Speed = 60;

    vector<double>Distance { 64,     22.1,   40.8,  376,    31,    46.4, 15.3,   4.1,    83.5,     16.5,    50.4,  109,   39.1,   2.5,  28.2,   32.6,   4.1,  493};
    vector<double> lambda {0, 0.9964, 0.9848, 0.994, 0.9054, 0.794, 0.91, 0.9994, 0.99864, 0.99682, 0.98534, 0.925, 0.916, 0.9914, 0.95, 0.9908, 0.9916, 0.92, 0};

    vector<vector<double> > price = {
            { 10000, 10000, 10000, 10000 },
            { 1.99 , 1.81, 1.99 , 1.81},
            { 1.91 , 1.68,  1.99 , 1.81},
            { 1.77 , 1.82, 1.99 , 1.81},
            { 1.89 , 1.91, 1.77 , 1.82},
            { 1.99 , 1.84, 1.77 , 1.82},
            { 1.88 , 1.94, 1.99 , 1.84},
            { 1.51 , 1.67, 1.99 , 1.84},
            { 1.69 , 1.70, 1.94 , 1.84},
            { 1.83 , 1.67, 1.89 , 1.84},
            { 1.57 , 1.58, 1.91 , 1.84},
            { 1.71 , 1.71, 1.90 , 1.84},
            { 1.83 , 1.87, 1.79 , 1.84},
            { 1.74 , 1.71, 1.74 , 1.84},
            { 1.76 , 1.73, 1.72, 1.99},
            { 1.75 , 1.68, 1.70, 1.99},
            { 1.77 , 1.84, 1.78, 1.99},
            { 1.73 , 1.80, 1.76, 1.99},
            { 10000, 1000, 1000, 1000}
    };

/*
    vector<double>Distance (N-1, 0.0);
    for (int i =0 ; i< N-1; i++){
        Distance[i] = mtrand1.randInt(200) + 30;
    }
   // Distance[0] = 0;
    Distance[N-2] = 1000;
*/
    int FinalDay = 1 + (60 * (accumulate(Distance.begin(), Distance.end(), 0)) / Speed) / 540;

    cout<< FinalDay<<endl;
/*
    vector<vector<double> > price (N, vector<double>(FinalDay, 0.0));

    for (int i =0 ; i< N; i++){
        for (int j =0 ; j < FinalDay; j++) {
            price[i][j] = mtrand1.randInt(5) + 1;
            price[0][j] =  100;
        }
    }


    vector<double> lambda (N , 0.0);

    srand( (unsigned)time( NULL ) );
    for (int i =0 ; i< N; i++){
            lambda[i] = (float) rand()/RAND_MAX;
       // cout<< lambda[i]<<endl;
    }
*/
    vector<Action> allActions =createAllActions();
    vector<State> allStates = createAllStates() ;
   // allStates.push_back(s);


    int iteration;
    iteration = 200000;

    vector< double> Alpha( iteration + 1, 1.0);
    double aCoefficient = 20000;

    for(int k = 1 ; k<= iteration ; k++){

        Alpha[k] = (double) (aCoefficient/(aCoefficient+k));

    }
    Action a;
    a.Refueling =0;
    vector< vector< vector< double> > > vBar (N, vector< vector<double> > (2,vector<double>(allStates.size(), 0.0)));
    vector< vector< vector< double> > > vHat (N, vector< vector<double> > (2,vector<double>(allStates.size(), 0.0)));
    vector< vector< Action> > decisionRule (allStates.size(), (vector<Action>(N, a)));
/*
    for (int i = 0 ; i < allStates.size() ; i++)
    {
        for (int h = 0; h< allActions.size() ; h++){

            if (feasibleAction(allStates[i], allActions[h], Distance[0], MPG, Speed) == true) {
                State j = StateTransition(allStates[i], allActions[h], Distance[0], MPG, Speed);
                cout << allStates[i].FuLevel << " " << allActions[h].Refueling << " " << j.FuLevel << " " << endl;
            }

        }
    }
*/

   ReAL(allActions, allStates, Alpha, N, iteration, vBar,vHat, decisionRule,  Distance,   MPG,   Speed, price,  lambda, Alpha);



 //  cout << "Nearest value of x :" << round((400*28)/100.0) << "\n";

 //   cout<<allActions.size()<<" "<<endl;

/*
    for (int i = 0 ; i < allStates.size(); i++){
        cout<<allStates[i].index<<" "<<allStates[i].FuLevel<<" "<<  allStates[i].DriveTime<<" "<< allStates[i].PresentDay <<" " <<endl;
    }*/
/*
State testS ;
testS.FuLevel = 387;
testS.PresentDay = 4;
testS.DriveTime = 345;

State test = StateTransition( testS, allActions[0], 1000.0,  MPG,  Speed);
cout<<testS.DriveTime<<" "<<testS.PresentDay<<" "<<testS.FuLevel<<" "<<endl;
cout<<test.DriveTime<<" "<<test.PresentDay<<" "<<test.FuLevel<<" "<<endl;
*/
    return 0;
}
