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

    int N = 50;
    double MPG = 28;
    double Speed = 70;

    vector<double>Distance (N, 0.0);
    for (int i =0 ; i< N; i++){
        Distance[i] = mtrand1.randInt(200) + 30;
    }

    int FinalDay = 1 + (60 * (accumulate(Distance.begin(), Distance.end(), 0)) / Speed) / 540;

    //cout<< FinalDay<<endl;

    vector<vector<double> > price (N, vector<double>(FinalDay, 0.0));

    for (int i =0 ; i< N; i++){
        for (int j =0 ; j < FinalDay; j++) {
            price[i][j] = mtrand1.randInt(5) + 1;
        }
    }

    vector<double> lambda (N , 0.0);

    srand( (unsigned)time( NULL ) );
    for (int i =0 ; i< N; i++){
            lambda[i] = (float) rand()/RAND_MAX;
       // cout<< lambda[i]<<endl;
    }

    vector<Action> allActions =createAllActions();
    vector<State> allStates = createAllStates() ;
   // allStates.push_back(s);


    int iteration;
    iteration = 200000;

    vector< double> Alpha( iteration + 1, 1.0);
    double aCoefficient = 25000;

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
testS.FuLevel = 100;
testS.PresentDay = 1;
testS.DriveTime = 533;

State test = StateTransition( testS, allActions[0], 100.0,  MPG,  Speed);
cout<<testS.DriveTime<<" "<<testS.PresentDay<<" "<<testS.FuLevel<<" "<<endl;
cout<<test.DriveTime<<" "<<test.PresentDay<<" "<<test.FuLevel<<" "<<endl;
 */
    return 0;
}
