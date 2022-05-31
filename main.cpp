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
#include "TestState.h"
#include "TestAlgorithm.h"
#include"RandGen.h"

/*--------------------------------------------------------------------------*/
/*------------------------- NAMESPACE AND USING ----------------------------*/
/*--------------------------------------------------------------------------*/


int main(int argc, const char *argv[]) {

 //   int Number_Routes = 1; // Number of routes
 //   vector<int> N{19}; // Number of Nodes (Origin + Number fuel stations = Destination)

  //  int N = 10;
    int number_routes = 3;
    //int* number_nodes = new int[number_routes] {5,4,3};
    int* number_nodes = new int[number_routes];
    double MPG = 28;
    double Speed = 60;
    int last_node = 0;
    int Seed = 50;
    RandLab RL(Seed);
    double total_distance = 0;

    for (int r = 0; r < number_routes; r++) {
        number_nodes[r] = RL.randint(3, 5);

        cout << "N[" <<r << "] =" << number_nodes[r] << endl;
    }


 //   vector<double> Distance = {0.7, 14.4, 30, 59.3, 277, 136, 0, 363, 76, 33.8, 53.2, 12, 1, 90, 15, 57.5, 8, 147, 3, 32, 38, 1, 253, 140, 0, 15, 52, 215, 36, 35, 40, 115, 53, 100, 0, 40, 134, 4.6, 48, 285, 0, 192, 159, 0, 345, 87, 29.4, 46, 12, 2 , 85.3, 20.9, 51.6, 110, 39.1, 3.6, 31.8, 36.7, 3.8, 220, 0, 8.8, 8.1, 43.7, 59.5, 142, 107, 884, 0, 64, 22.1, 40.8, 376, 31, 46.4, 15.3, 4.1, 83.5, 16.5, 50.4, 109, 39.1, 2.5, 28.2, 32.6, 4.1, 1167, 0, 31, 11, 3.8, 85.3, 20.9, 51.6, 110, 39.4, 5, 32.1, 35.1, 4.1, 442} ;
   // vector<double> lambda = { 0, 0.986, 0.912, 0.834, 0.9994, 0.9963, 0, 0, 0.58, 0.9054, 0.794, 0.91, 0.9994, 0.99864, 0.99682, 0.98534, 0.925,  0.916,  0.9914, 0.95, 0.99908, 0.9916, 0.92, 0.9874, 0, 0, 0.698, 0.32, 0.99114, 0.99814, 0.9914, 0.916, 0.925, 0.99354, 0, 0, 0.8996, 0.99864, 0.9994, 0.9054, 0, 0, 0.9866, 0, 0, 0.58, 0.9054, 0.794, 0.91, 0.9994, 0.99864, 0.99682, 0.98534, 0.925, 0.916, 0.9914, 0.95, 0.99908, 0.9916, 0.92 ,0, 0, 0.908, 0.952, 0.8524, 0.973, 0.976, 0.876 ,0 , 0, 0.9964, 0.9848, 0.994, 0.9054, 0.794, 0.91, 0.9994, 0.99864, 0.99682, 0.98534, 0.925, 0.916, 0.9914, 0.95, 0.9908, 0.9916, 0.92, 0 , 0, 0.91, 0.9994, 0.99864, 0.99682, 0.98534, 0.925, 0.916, 0.9914, 0.95, 0.99908, 0.9916, 0.92 , 0} ;
 //   vector<double> Distance = { 0.7, 14.4, 30, 59.3, 277, 136, 0, 363, 76, 33.8, 53.2, 12, 1, 90, 15, 57.5, 8, 147, 3, 32, 38, 1, 253, 140, 0, 15, 52, 215, 36, 35, 40, 115, 53, 100, 0, 40, 134, 4.6, 48, 285, 0, 192, 159, 0, 345, 87, 29.4, 46, 12, 2 , 85.3, 20.9, 51.6, 110, 39.1, 3.6, 31.8, 36.7, 3.8, 220, 0, 8.8, 8.1, 43.7, 59.5, 142, 107, 884, 0, 64, 22.1, 40.8, 376, 31, 46.4, 15.3, 4.1, 83.5, 16.5, 50.4, 109, 39.1, 2.5, 28.2, 32.6, 4.1, 1167, 0, 31, 11, 3.8, 85.3, 20.9, 51.6, 110, 39.4, 5, 32.1, 35.1, 4.1, 442, 0, 33.9, 56.7, 39.1, 3, 30.1, 32.6, 4.1, 153, 0, 21.9, 133, 6.4, 34.8, 32.5, 3.4, 34, 56.9, 65, 50.6, 318, 0, 113, 87.7, 95.2, 20.9, 155, 39.1, 4.1, 29.5, 32.6, 4.1, 121, 16.3} ;
 //   vector<double> lambda = { 0, 0.986, 0.912, 0.834, 0.9994, 0.9963, 0, 0, 0.58, 0.9054, 0.794, 0.91, 0.9994, 0.99864, 0.99682, 0.98534, 0.925,  0.916,  0.9914, 0.95, 0.99908, 0.9916, 0.92, 0.9874, 0, 0, 0.698, 0.32, 0.99114, 0.99814, 0.9914, 0.916, 0.925, 0.99354, 0, 0, 0.8996, 0.99864, 0.9994, 0.9054, 0, 0, 0.9866, 0, 0, 0.58, 0.9054, 0.794, 0.91, 0.9994, 0.99864, 0.99682, 0.98534, 0.925, 0.916, 0.9914, 0.95, 0.99908, 0.9916, 0.92 ,0, 0, 0.908, 0.952, 0.8524, 0.973, 0.976, 0.876 ,0 , 0, 0.9964, 0.9848, 0.994, 0.9054, 0.794, 0.91, 0.9994, 0.99864, 0.99682, 0.98534, 0.925, 0.916, 0.9914, 0.95, 0.9908, 0.9916, 0.92, 0 , 0, 0.91, 0.9994, 0.99864, 0.99682, 0.98534, 0.925, 0.916, 0.9914, 0.95, 0.99908, 0.9916, 0.92 , 0, 0, 0.8524, 0.916, 0.9914, 0.95, 0.99908, 0.9916, 0.92, 0, 0, 0.6, 0.92, 0.9916, 0.99908, 0.9914, 0.95, 0.916, 0.8524, 0.925, 0.99354 , 0,  0, 0.9896, 0.972, 0.99682, 0.98534, 0.916, 0.9914, 0.95, 0.99908, 0.9916, 0.92, 0.998, 0 };

    int N = 0;
    for (int r = 0; r < number_routes; r++) {
        N += number_nodes[r];

    }
    cout << "last_node = " << N << endl;

    vector<double>D (N, 0.0);
    int index_d = 0;
    for (int i = 0 ; i < number_routes; i++) {
        for (int j = index_d; j < number_nodes[i] + index_d; j++) {

            D[j] = RL.uniform(10,100);
            D[number_nodes[i] + index_d - 1] = 0;

        }

        index_d += number_nodes[i];
    }

    vector<double>Distance (D.begin(), D.begin()+(N-1));
    for (int i = 0 ; i < N - 1; i++) {
        cout << "Distance[" << i << "] = " << D[i] << endl;
    }

    vector<double> De (N , 0.0);
    int index_de = 0;
    for (int i = 0 ; i < number_routes; i++) {
        for (int j = index_de; j < number_nodes[i] + index_de; j++) {

            De[j] = RL.uniform(0.5, 10);
            De[number_nodes[i] + index_de - 1] = 0;

        }
        index_de += number_nodes[i];
    }
    vector<double>Detour (De.begin(), De.begin()+(N-1));
    for (int i = 0 ; i < N - 1; i++) {
        cout << "Detour[" << i << "] = " << Detour[i] << endl;
    }


    int FinalDay = 1 + (60 * (accumulate(Distance.begin(), Distance.end(), 0)) / Speed) / 540;

    cout << "FinalDay = " << FinalDay << endl;

    vector<vector<double> > price (N, vector<double>(FinalDay, 0.0));
    for (int d = 0; d < FinalDay; d++) {
    int index_p = 0;
    for (int i = 0 ; i < number_routes; i++) {
        for (int j = index_p; j < number_nodes[i] + index_p; j++) {

                price[j][d] = RL.uniform(0.95, 2.5);
                price[index_p][d] = 1000;
                price[number_nodes[i] + index_p - 1][d] = 1000;

             //  cout << "price[" << j << "][" << d << "] = " << price[j][d] << endl;
            }
            index_p += number_nodes[i];
        }
    }
    vector<double> lambda (N , 0.0);
    int index_l = 0;
    for (int i = 0 ; i < number_routes; i++) {
        for (int j = index_l; j < number_nodes[i] + index_l; j++) {

            lambda[j] = RL.uniform(0.5, 0.999);
            lambda[index_l] = 0;
            lambda[number_nodes[i] + index_l - 1] = 0;
          // cout << "lambda[" << j << "] = " << lambda[j] << endl;
        }
        index_l += number_nodes[i];
    }


    vector<Action> allActions =createAllActions();
    vector<State> allStates = createAllStates() ;
   // allStates.push_back(s);


    int iteration;
    iteration = 200000;

    vector< double> Alpha( iteration + 1, 1.0);
    double aCoefficient;
    aCoefficient = 10000;

    for(int k = 1 ; k<= iteration ; k++){

        Alpha[k] = (double) (aCoefficient/(aCoefficient+k));

    }
    Action a;
    a.Refueling = -1;
    vector<vector<vector<double> > > vBar(N, vector<vector<double> >(2, vector<double>(allStates.size(), 70000)));
    vector<vector<vector<double> > > vHat(N, vector<vector<double> >(2, vector<double>(allStates.size(), 70000)));
    vector<vector<Action> > decisionRule(allStates.size(), (vector<Action>(N, a)));


   ReAL(allActions, allStates, Alpha, N, iteration, vBar,vHat, decisionRule,  Distance,   MPG,   Speed, price,  lambda, Detour, Alpha);



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
