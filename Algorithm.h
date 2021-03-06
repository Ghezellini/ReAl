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
                       double Distance, double MPG, double Speed ) {


    State j;

    j.FuLevel = s.FuLevel + a.Refueling - round((Distance*MPG)/100.0);

    //  if (j.FuLevel < j.LB)   cout<<"Error, this is not possible!"<<endl;

    // j.DrivingTime = drTime + Distance/Speed;
    j.DriveTime =  s.DriveTime + round(60*Distance/Speed);
    j.PresentDay = s.PresentDay;

    int incrementDay = (j.DriveTime/j.MaxDrivingPerDay);

    if (j.DriveTime>=j.MaxDrivingPerDay){
        j.DriveTime = round(60*Distance/Speed) - (s.MaxDrivingPerDay - s. DriveTime);
        j.PresentDay += incrementDay;
    }

    j.index = j.FuLevel - j.LB;

    return j;

}// end state transition function

/*--------------------------------------------------------------------------*/
/*--------------------------- Feasible Action ------------------------------*/
/*--------------------------------------------------------------------------*/

bool feasibleAction( State s, Action a,  double Distance, double MPG,
                     double Speed) {

    bool cond = false;

    State j = StateTransition(  s,  a,
                                Distance,  MPG,  Speed );
    if ( a.Refueling <= s.UB - s.FuLevel &&
         j.FuLevel == s.FuLevel + a.Refueling - round((Distance*MPG)/100.0) &&
         j.FuLevel <= s.UB && j.FuLevel >= s.LB )  {
        cond = true;
    }
    return cond;

} // end feasible action function

/*--------------------------------------------------------------------------*/
/*-------------------------- Immediate Reward ------------------------------*/
/*--------------------------------------------------------------------------*/

double ImmediateReward ( Action a, State s, double price, double lambda,
                         int t ) {
    double Rew = 0.0;

    if ( s.FuLevel < s.LB )
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

    if ( j.FuLevel < j.LB )
        Rew = 1000000000000.0;
    else
        // Rew = 0.0014* (price * (j.UB-j.FuLevel) * ( 1.0 + ( 1.0 - lambda ) ));
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
             vector< double > &Alpha, int Number_Routes, vector< int > N, int iteration,
             vector< vector < vector< double > > > &vBar,
vector< vector< vector < double > > > &vHat,
vector<vector< Action > > &decisionRule, vector< vector< double >> Distance,
double MPG, double Speed, vector< vector< vector< double >>> price, vector< vector< double >> lambda,
        vector< double > alpha )
{

mtrand1.seed(5);

vector< double > random;


for ( int k = 1 ; k<= iteration ; k++ ) {
random.push_back( 1.0/( 1 + k-1 ) );
}
for ( int r = 0 ; r < Number_Routes ; r++ ) {
vector<vector<State> > visitedStates(N[r], vector<State>(iteration + 1, allStates[0]));
vector<vector<double> > CurrentContribution(N[r], vector<double>(iteration + 1, 500000.0));
// vector< vector<double> > BestCurrentContribution (N, vector<double> (iteration + 1, 0.0));
vector<vector<double> > ExpectedContribution(N[r], vector<double>(iteration + 1, 500000.0));
vector<vector<double> > BestRewardSoFar(N[r], vector<double>(iteration + 1, 500000.0));
//vector< vector< vector< double> > > zSmoothed (N, vector< vector<double> > (2, vector<double>(allStates.size(), 0.0)));
vector<vector<Action> > visitedActions(N[r], vector<Action>(iteration + 1, allActions[0]));

for (int s = 0; s < allStates.size(); s++) {
// Middle 0 means current approximation for vBar and current observation for vHat, they do not get updates in the algorithm
vBar[N[r] - 1][0][s] = TerminalReward(allStates[s], price[r][N[r] - 1][price[r][0].size() - 1], lambda[r][N[r] - 1]);
vHat[N[r] - 1][0][s] = vBar[N[r] - 1][0][s];
//   cout<< vBar[N-1][0][s]<<"Khar"<<endl;
}


int k = 1;
// Iteration loop

State s;

while (k <= iteration) {
if (k < iteration)
s = allStates[mtrand1.randInt(allStates.size() - 1)];

else
s = allStates[50]; // Initial fuel level

// s.FuLevel = mtrand1.randInt(s.UB-s.LB) + s.LB ;


// Random Initilization

State j;

for (int t = 0; t < N[r] - 1; t++) {  //Exploration loop
//
//visitedActions[0][k].Refueling = 0.0; // We are not able to refuel in the origin
visitedStates[t][k] = s;

int index = -1;
int DrTime = -1;
int Day = 0;
/*
            double Random = mtrand1.rand();
            if (Random <random[k]){
                Action a;
                do {
                     a = allActions[mtrand1.randInt(allActions.size()-1)];
                }
                while (feasibleAction(s, a, Distance[t], MPG, Speed) == false);

                CurrentContribution[t][k] = (double) ImmediateReward(a, s, price[t], lambda, t);
                //ExpectedContribution[t][k]= FutureApprox(j);
                //   cout<<t<< " " << k << " "<< allActions[a].Refueling<<" "<<CurrentContribution[t][k]<<endl;

                ExpectedContribution[t][k] = vBar[t + 1][0][j.index];


                double x = CurrentContribution[t][k] + ExpectedContribution[t][k];

                decisionRule[s.index][t] = a;

                best = x;
                //    cout<<t<< " " << k << " "<<decisionRule[s.index][t].Refueling<<" "<<best<<endl;

                visitedStates[t + 1][k][0] = j.FuLevel;
                visitedStates[t + 1][k][1] = j.DrivingTime;
                index = (int) j.index;

            }
*/
//           else {

for (int a = 0; a < allActions.size(); a++) {

if (feasibleAction(s, allActions[a], Distance[r][t], MPG, Speed)) {

j = StateTransition(s, allActions[a], Distance[r][t], MPG, Speed);

CurrentContribution[t][k] = (double) ImmediateReward(allActions[a], s, price[r][t][Day], lambda[r][t],
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


visitedStates[t + 1][k] = allStates[index];
//  BestRewardSoFar[t][k] = best;
// Cuurent Vhat s then we smoothed it with previous vBar ---> update VBar
s = visitedStates[t + 1][k];
s.DriveTime = DrTime;
s.PresentDay = Day;

if (t == N[r] - 2) {
//  s = visitedStates[t + 1][k];
visitedStates[t + 1][k].PresentDay = s.PresentDay;
visitedStates[t + 1][k].DriveTime = s.DriveTime;
}

}

//   visitedStates[N-1][k] = StateTransition(s, allActions[0], Distance[N-1], MPG, Speed);


for (int t = N[r] - 2; t >= 0; t--) {
vHat[t][1][visitedStates[t][k].index] = BestRewardSoFar[t][k]; // new Observation Value
vBar[t][1][visitedStates[t][k].index] = (1.0 - alpha[k]) * vBar[t][0][visitedStates[t][k].index] +
(alpha[k] *
vHat[t][1][visitedStates[t][k].index]); // new Approximation value
vBar[t][0][visitedStates[t][k].index] = (double) vBar[t][1][visitedStates[t][k].index];
}

if (k == iteration) {

cout << "t " << "PD" << " " << "DT" << " " << "P" << "  " << "D" << " " << "F1" << " " << "R" << " "
<< "F2" << " " << "V" << endl;
double TotRef = 0.0;
for (int t = 0; t < N[r]; t++) {

if (t < N[r] - 1) {
cout << t << " " << visitedStates[t][k].PresentDay << " " << visitedStates[t][k].DriveTime
<< " "
<< price[r][t][visitedStates[t][k].PresentDay] << " " << Distance[r][t] << " "
<< allStates[visitedStates[t][k].index].FuLevel << " " << visitedActions[t][k].Refueling
<< " "
<< allStates[visitedStates[t + 1][k].index].FuLevel << " "
<< vBar[t][1][visitedStates[t][k].index] << " "
<< endl;


} else {
cout << t << " " << visitedStates[t][k].PresentDay << " " << endl;
}

TotRef += visitedActions[t][k].Refueling;
}
cout << "Total refueling is " << TotRef << endl;


/*
        for (int s = 0 ; s<allStates.size(); s++){
            cout << allStates[s].FuLevel<<" "<<  vBar[0][1][s] << " " << endl;

            for(int t = 0; t < N; t++) {
                cout << decisionRule[s][t].Refueling << " " <<endl;
            }
        }
*/
}

k++;
int h0 = allStates[0].index;
//   cout<< k<<" "<<vBar[0][1][h0]<<" "<<endl;
//cout<<vBar[0][0][h0]<<""<<endl;
}

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