/*--------------------------------------------------------------------------*/
/*---------------------------- File Algorithm.h ----------------------------*/
/*--------------------------------------------------------------------------*/
/** @file
 * Header file for the class State.
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
/*----------------------------- DEFINITIONS --------------------------------*/
/*--------------------------------------------------------------------------*/
#ifndef TestAlgorithm_h
#define TestAlgorithm_h
                      /* self-identification: #endif at the end of the file */

/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/

#include "MersenneTwister.h"
#include <numeric>
#include<algorithm>
/*--------------------------------------------------------------------------*/
/*------------------------------ NAMESPACE ---------------------------------*/
/*--------------------------------------------------------------------------*/
using namespace std;
MTRand mtrand1;

// Random Poisson generating function
double GenerateRandomPoisson( double lambda ) {
    double u = mtrand1.rand();
    double limit = exp(-lambda);
    int n;
    for ( n = 0; u >= limit; n++)
        u *= mtrand1.rand();
    return n;
} //end Poisson function


/*--------------------------------------------------------------------------*/
/*-------------------------- State Transition ------------------------------*/
/*--------------------------------------------------------------------------*/

State StateTransition( State s, Action a,
                       double Distance, double Detour, double MPG, double Speed ) {


    State j;

    j.FuLevel = s.FuLevel + a.Refueling - (round((Distance*MPG)/100.0) + 2*round((Detour*MPG)/100.0));
    if (a.Refueling == 0 ){
        j.FuLevel = s.FuLevel - round((Distance*MPG)/100.0);
    }
  //  if (j.FuLevel < j.LB)   cout<<"Error, this is not possible!"<<endl;

    // j.DrivingTime = drTime + Distance/Speed;
    j.DriveTime =  s.DriveTime + round(60*Distance/Speed) + 2*round(60*Detour/Speed);
    if (a.Refueling == 0 ){
        j.DriveTime =  s.DriveTime + round(60*Distance/Speed);
    }
    j.PresentDay = s.PresentDay;

    int incrementDay = (j.DriveTime/j.MaxDrivingPerDay);

    if (j.DriveTime>=j.MaxDrivingPerDay){
        if (a.Refueling == 0 ) {
            j.DriveTime = round(60 * Distance / Speed) - (s.MaxDrivingPerDay - s.DriveTime);
        } else {
            j.DriveTime = round(60 * Distance / Speed) + 2*round(60*Detour/Speed) - (s.MaxDrivingPerDay - s.DriveTime);
        }
        j.PresentDay += incrementDay;
    }

    j.index = j.FuLevel - j.LB;

    return j;

}// end state transition function

/*--------------------------------------------------------------------------*/
/*--------------------------- Feasible Action ------------------------------*/
/*--------------------------------------------------------------------------*/

bool feasibleAction( State s, Action a,  double Distance, double Detour, double MPG,
                     double Speed) {

    bool cond = false;

    State j = StateTransition(  s,  a, Distance, Detour, MPG,  Speed );

    if (a.Refueling == 0 ) {
        if (j.FuLevel == s.FuLevel - round((Distance * MPG) / 100.0) &&
            j.FuLevel <= s.UB && j.FuLevel >= s.LB) {
            cond = true;
        }
    } else {
        if (a.Refueling <=   s.UB - s.FuLevel &&
            j.FuLevel == s.FuLevel + a.Refueling - round((Distance * MPG) / 100.0) - 2*round((Detour * MPG) / 100.0) &&
            j.FuLevel <= s.UB && j.FuLevel >= s.LB) {
            cond = true;
        }
    }
    return cond;

} // end feasible action function

/*--------------------------------------------------------------------------*/
/*-------------------------- Immediate Reward ------------------------------*/
/*--------------------------------------------------------------------------*/

double ImmediateReward ( Action a, State s, double price, double lambda,
                         int t ) {
    double Rew = 0.0;

    if ( s.FuLevel < s.LB && s.FuLevel > s.UB )
        Rew = 1000000000000.0;
    else
        Rew = price * a.Refueling * ( 1.0 + ( 1.0 - lambda ) );
    return Rew;
} // end immediate reward function

/*--------------------------------------------------------------------------*/
/*--------------------------- Terminal Reward ------------------------------*/
/*--------------------------------------------------------------------------*/

double TerminalReward ( State j, double price, double lambda ) {
    double Rew = 0.0;

    if ( j.FuLevel < j.LB && j.FuLevel > j.UB )
        Rew = 1000000000000.0;
    else
      //Rew = 0.00014* (price * (j.UB-j.FuLevel) * ( 1.0 + ( 1.0 - lambda ) ));
      Rew = 0.0;
    return Rew;
}// end terminal reward function

/*--------------------------------------------------------------------------*/
/*------------------------- Future Approximation ---------------------------*/
/*--------------------------------------------------------------------------*/
double FutureApprox ( State s ) {

    return s.FuLevel;

}// end future approximation function

/*--------------------------------------------------------------------------*/
/*--------------------------------- ReAL -----------------------------------*/
/*--------------------------------------------------------------------------*/

double ReAL( vector<Action>allActions, vector<State>allStates,
             vector< double > &Alpha, int N, int iteration,
             vector< vector < vector< double > > > &vBar,
    vector< vector< vector < double > > > &vHat,
    vector<vector< Action > > &decisionRule, vector<double> Distance,
    double MPG, double Speed, vector< vector< double >> price, vector< double > lambda, vector< double > Detour,
        vector< double > alpha )
{

    mtrand1.seed(5);

   // vector< double> random(iteration+1, 0.0);
    vector< double> random;


    for ( int k = 1 ; k<= iteration ; k++ ) {
    random.push_back( 1.0/( 1 + k-1 ) );
       //    random [k] =  (double) 0.2*(1.0 - (double) (k/iteration) );
      //  random[k] = (double)(1.0/(1+k-1));
        }

    vector< vector< State> >  visitedStates (N, vector<State> (iteration+1, allStates[0]));
    vector< vector<double> > CurrentContribution (N, vector<double> (iteration + 1, 500000.0));
    // vector< vector<double> > BestCurrentContribution (N, vector<double> (iteration + 1, 0.0));
    vector< vector<double> > ExpectedContribution (N, vector<double> (iteration + 1, 500000.0));
    vector< vector<double> > BestRewardSoFar (N, vector<double> (iteration + 1, 500000.0));
    //vector< vector< vector< double> > > zSmoothed (N, vector< vector<double> > (2, vector<double>(allStates.size(), 0.0)));
    vector< vector< Action> >  visitedActions (N, vector<Action>  (iteration + 1, allActions[0]));

    for( int s = 0; s < allStates.size(); s++ ) {
    // Middle 0 means current approximation for vBar and current observation for vHat, they do not get updates in the algorithm
    vBar[N-1][0][s] = TerminalReward ( allStates[s], price[N-1][price[0].size()-1],  lambda[N-1]);
    vHat[N-1][0][s] = vBar[N-1][0][s];
      //  cout << "\t\t\t vBar[" << N-1 << "][" << 0 << "][" << s << "] = " << vBar[N-1][0][s] << endl;
     //   cout << "\t\t\t vHat[" << N-1 << "][" << 0 << "][" << s << "] = " << vHat[N-1][0][s] << endl;
    }



    int k = 1;
    // Iteration loop

    State s;

    while ( k <= iteration ) {
       if (k < iteration)
           s = allStates[mtrand1.randInt(allStates.size() - 1)];

       else
    s = allStates[325]; // Initial fuel level
    State j;

    for (int t = 0 ; t < N-1 ; t++) {  //Exploration loop
        //
    visitedStates[t][k] = s;

    int index = -1;
    int DrTime = -1;
    int Day = 0;
    int Replication = 5;
/*
            double Random = mtrand1.rand();
            if ( Random < random[k]){
                Action action;
                do {
                    action = allActions[mtrand1.randInt(allActions.size()-1)];
                }
                while (!feasibleAction(s, action, Distance[t], Detour[t], MPG, Speed));
                j = StateTransition(s, action, Distance[t], Detour[t], MPG, Speed);
                CurrentContribution[t][k] = (double) ImmediateReward(action, s, price[t][Day], lambda[t], t);
                ExpectedContribution[t][k]= FutureApprox(j);
                   cout<<t<< " " << k << " "<< action.Refueling<<" "<<CurrentContribution[t][k]<<endl;

                double x = CurrentContribution[t][k] + ExpectedContribution[t][k];

                decisionRule[s.index][t] = action;

                //best = x;
                  //  cout<<t<< " " << k << " "<<decisionRule[s.index][t].Refueling<<" "<< x <<endl;

                visitedStates[t + 1][k] = j;
              //  visitedStates[t + 1][k][1] = j.DrivingTime;
              index = (int) j.index;
              DrTime = j.DriveTime;
              Day = (int) j.PresentDay;

            }
*/
      //   else {

                for (int a = 0; a < allActions.size(); a++) {

                    if (feasibleAction(s, allActions[a], Distance[t], Detour[t], MPG, Speed)) {

                        j = StateTransition(s, allActions[a], Distance[t],  Detour[t], MPG, Speed);

                        CurrentContribution[t][k] = (double) ImmediateReward(allActions[a], s, price[t][Day], lambda[t],
                                                                             t);
                        ExpectedContribution[t][k] = vBar[t + 1][0][j.index];

                        double x = CurrentContribution[t][k] + ExpectedContribution[t][k];


                        if (x < BestRewardSoFar[t][k]) {
                            decisionRule[s.index][t] = allActions[a];
                            visitedActions[t][k] = allActions[a];
                            BestRewardSoFar[t][k] = x;
                            index = (int) j.index;
                            DrTime = j.DriveTime;
                            Day = (int) j.PresentDay;
                        }
                    }
                }

        //  }
    visitedStates[t + 1][k] = allStates[index];
    //  BestRewardSoFar[t][k] = best;
    // Cuurent Vhat s then we smoothed it with previous vBar ---> update VBar
    s = visitedStates[t + 1][k];
    s.DriveTime = DrTime;
    s.PresentDay = Day;

    if ( t == N-2 ){
        s = visitedStates[t + 1][k];
        visitedStates[t+1][k].PresentDay = s.PresentDay;
        visitedStates[t+1][k].DriveTime = s.DriveTime;
    }

    }

 //   visitedStates[N-1][k] = StateTransition(s, allActions[0], Distance[N-1], Detour[N-1], MPG, Speed);


    for (int t = N-2; t>=0 ; t--){
    vHat[t][1][visitedStates[t][k].index] = BestRewardSoFar[t][k]; // new Observation Value
    vBar[t][1][visitedStates[t][k].index] = (1.0 - alpha[k]) * vBar[t][0][visitedStates[t][k].index] + (alpha[k]*vHat[t][1][visitedStates[t][k].index]) ; // new Approximation value
    vBar[t][0][visitedStates[t][k].index] = (double) vBar[t][1][visitedStates[t][k].index];
    }

    if (k == iteration) {

        cout<<"t "<<"PD"<<" "<<"DT"<<" "<<"P"<<"  "<<"D"<<"  "<<"De"<<" "<<"F1"<<" "<<"R"<<" "<<"F2"<<" "<<"V"<<endl;
        double TotRef= 0.0;
        double TotDistance= 0.0;
        double ObjectiveValue = 0.0;
        for(int t = 0; t < N; t++){

            if ( t < N-1 ) {
        cout  << t <<" "<<visitedStates[t][k].PresentDay << " " << visitedStates[t][k].DriveTime << " "
             << price[t][visitedStates[t][k].PresentDay] << " " << lambda[t] << " "<< Distance[t] << " " << Detour[t] << " "
             << allStates[visitedStates[t][k].index].FuLevel << " " << visitedActions[t][k].Refueling << " "
             << allStates[visitedStates[t + 1][k].index].FuLevel << " " << vBar[t][1][visitedStates[t][k].index] << " "
             << endl;


    }

    else{
                cout  << t <<" "<<visitedStates[t][k].PresentDay << " " << visitedStates[t][k].DriveTime << " "
                      << price[t][visitedStates[t][k].PresentDay] << " " << lambda[t] << endl;
    }

            TotRef += visitedActions[t][k].Refueling;
            ObjectiveValue += visitedActions[t][k].Refueling * price[t][visitedStates[t][k].PresentDay];
            if (visitedActions[t][k].Refueling == 0 ) {
                TotDistance += Distance[t];
            }else {
                TotDistance += Distance[t] + 2*Detour[t];
            }
        }
            cout<<"Total refueling is "<< TotRef<<endl;
            cout << "ObjectiveValue = " << ObjectiveValue << endl;
           cout << "TotDistance = " << TotDistance << endl;

    }

    k++;
    int h0 =allStates[325].index;
   //    cout<< k<<" "<<vBar[0][1][h0]<<" "<<endl;
  //  cout<<vBar[0][0][h0]<<""<<endl;
    }

return vBar[0][0][allStates.size()-1];
} //end ReAL

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

int timeval_subtract(struct timeval & result, struct timeval x, struct timeval y)
{
    // Perform the carry for the later subtraction by updating y. //
    if (x.tv_usec < y.tv_usec)
    {
        int nsec = (y.tv_usec - x.tv_usec) / 1000000 + 1;
        y.tv_usec -= 1000000 * nsec;
        y.tv_sec += nsec;
    }
    if (x.tv_usec - y.tv_usec > 1000000) {
        int nsec = (x.tv_usec - y.tv_usec) / 1000000;
        y.tv_usec += 1000000 * nsec;
        y.tv_sec -= nsec;
    }

    // Compute the time remaining to wait.
    // tv_usec is certainly positive. //
    result.tv_sec = x.tv_sec - y.tv_sec;
    result.tv_usec = x.tv_usec - y.tv_usec;

    // Return 1 if result is negative. //
    return x.tv_sec < y.tv_sec;
}

#endif /* TestAlgorithm_h */