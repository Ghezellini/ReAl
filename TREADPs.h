//
//  TREADPs.h
//  TRE_ADP
//
//  Created by Amin Asadi on 10/29/20.
//  Copyright © 2020 Amin Asadi. All rights reserved.
//

#ifndef TREADPs_h
#define TREADPs_h

double DoublePass (vector<Action>allActions, vector<State>allStates, vector< double> Alpha, int MBatteries, int N, int Phi, double Thr , double deltaC, double deltaE, int iteration, int replications, vector<double>lambdaVec, int CostReplace, double ExchangeVec, vector<double>ChargeVec, vector<double>DischargeVec, double CapacityIncrement, vector< vector< vector< double> > > &vBar, vector< vector< vector< double> > > &vHat, double LAMBDA ){
    
    vector< double> random(iteration+1, 0.0);
    
    for(int k = 1 ; k<= iteration/10 ; k++){
    
        random [k] = (double) (0.2- 0.2*(10*k/iteration));
    }
    
 //   int allStates[visitedStates[t][k]].getNumFull();
 //   double allStates[visitedStates[t][k]].getAvg();
    int FutureCharge = 0;
    double FutureCapacity = 0.0;
    double reward = 0.0;
    double uBest = 0;
    //   int Best = -1;
    // Visited state at time t and iteration k.
     vector< vector< int> >  visitedActions (N, vector<int>  (allStates.size(), 0.0));
     vector< vector< int> >  visitedStates (N, vector<int> (iteration+1, 0.0));

    vector< vector<double> > CurrentContribution (N, vector<double> (iteration+1, 0.0));
    vector< vector<double> > BestCurrentContribution (N, vector<double> (iteration+1, 0.0));
    vector< vector<double> > ExpectedContribution (N, vector<double> (iteration+1, 0.0));
    vector< vector<double> > BestRewardSoFar (N, vector<double> (iteration+1, -1000.0));
    vector< vector< vector< double> > > zSmoothed (N, vector< vector<double> > (2, vector<double>(allStates.size(), 0.0)));
    

    
        for(int s = 0; s < allStates.size(); s++){
            vBar[N-1][0][s] = terminalReward(allStates[s], ExchangeVec, N,Thr);
            vHat[N-1][1][s] = vBar[N-1][0][s];
        }

/*
    for(int t =0 ; t<=N-2 ; t++){
        for(int s = 0; s < allStates.size(); s++){
            if (allStates[s].getAvgCap()==0)    vBar[t][0][s] = 0.0;
            else

            vBar[t][0][s] = (-381.6374041 + MBatteries*10.4145067 - 0.1190217*t + 1.4656651*allStates[s].getNumberFull() + 442.8659749*allStates[s].getAvgCap());


        }
    }
 */
    
    // Set the demand (uncertainty) at time t and iteration k.
    vector< vector< vector< double> > > W (N, vector< vector<double> > (iteration+1, vector<double>(replications, 0.0)));
    for(int t = 0; t < N; t++){
        for (int k =0 ; k <=iteration ; k++){
            for (int m =0 ; m <replications ; m++){
       //     W[t][k] = 20;
      // if(k<iteration)            W[t][k][m] = GenerateRandomPoisson(lambdaVec[t]);
      //      else       W[t][k][] =  floor(lambdaVec[t]);
                    W[t][k][m]= GenerateRandomPoisson(lambdaVec[t]);
                    if (k==iteration && m == replications-1)
                        W[t][k][m] =  floor(lambdaVec[t]);
                }
            }
    }
    
    int k = 1;
    int jFound = -1;
    int jSelected = -1;
    // Iteration loop
    
    while (k<= iteration){
        
        // Set the initial state of each iteration to be non-absorbing state

        // random initial state

        do  {
            (visitedStates[0][k]  = (double) round(mtrand1.randInt(allStates.size()-1)));
        } while(allStates[visitedStates[0][k]].getAvgCap() < Thr);

        // sometimes initial state is not random
        if (k % 2 == 0) {
            visitedStates[0][k] = (double) round(indexReturn(MBatteries, 1.0, Thr, CapacityIncrement, MBatteries));
        }
     
     //   visitedStates[0][k] = (double) round(indexReturn(MBatteries, 1.0, Thr, CapacityIncrement, MBatteries));

     // for loop time
        for (int t =0 ; t<N-1 ; t++) {
             jSelected = -1;

            // for loop action
            for (int a = 0 ; a<allActions.size() ; a++) {
                 if (CheckPossibleAction(allStates[visitedStates[t][k]], allActions[a], MBatteries, Phi, Thr) ==true)
                {
                    jFound = -1;

                    // no need to have this assume m = 1
                    for (int m = 0 ; m <replications ; m++){
                    FutureCharge = (double) allStates[visitedStates[t][k]].getNumberFull() + allActions[a].getNumReplace()+ max(allActions[a].getChargeDischarge(),0) - abs(min(allActions[a].getChargeDischarge(),0))- min((double)allStates[visitedStates[t][k]].getNumberFull()-abs(min(allActions[a].getChargeDischarge(),0)), W[t][k][m]);
                    
                    FutureCapacity = (double) CapacityTransition(allStates[visitedStates[t][k]].getNumberFull(), allStates[visitedStates[t][k]].getAvgCap(), allActions[a].getChargeDischarge(), allActions[a].getNumReplace(), FutureCharge, deltaC, deltaE, MBatteries, Thr);
                    
                    jFound = (double) round(indexReturn(FutureCharge, FutureCapacity, Thr, CapacityIncrement, MBatteries));
                    ExpectedContribution[t][k] += vBar[t+1][0][jFound];
                    }

                    // future reward
                    ExpectedContribution[t][k] = (double) (1.0/replications)*ExpectedContribution[t][k];
                    
                    // Immidiate reward
                    CurrentContribution[t][k] = (double) (- CostReplace * allActions[a].getNumReplace()
                                                          - ChargeVec[t] * max(0,allActions[a].getChargeDischarge())
                                                          + DischargeVec[t] * abs(min(0,allActions[a].getChargeDischarge()))
                                                          + ExchangeVec* (1.0 +(allStates[visitedStates[t][k]].getAvgCap()-Thr)/(1.0-Thr)) *
                                                          (min( (double) allStates[visitedStates[t][k]].getNumberFull()
                                                           - abs(min(0,allActions[a].getChargeDischarge())),W[t][k][replications-1])));
                    reward = (double) (CurrentContribution[t][k] + ExpectedContribution[t][k]);

                    // this random is used for exploration
                    double Random = mtrand1.rand();
                    // Find the best action and reward at time t and iteration k.

                    // An action found, no exploration  == best action
                    if  (reward >= BestRewardSoFar[t][k] || Random <random[k]){
                        visitedActions[t][k] = (double) a;
                        BestRewardSoFar[t][k] = (double) reward;
                        BestCurrentContribution[t][k] = (double) CurrentContribution[t][k];
                        jSelected= (double) jFound;
                        }
                    }
                }//End Action Loop
            visitedStates[t+1][k] = (double) jSelected;

        }// End forward time loop
        for (int t = N-2; t>=0 ; t--){
            vHat[t][1][visitedStates[t][k]] = (double) BestRewardSoFar[t][k];
            vBar[t][1][visitedStates[t][k]] = (double) ((double)Alpha[k]*(double) vHat[t][1][visitedStates[t][k]] + (double)(1.0 - Alpha[k]) * (double) vBar[t][0][visitedStates[t][k]]);
            vBar[t][0][visitedStates[t][k]] = (double) vBar[t][1][visitedStates[t][k]];
        }

        if (k==iteration) {
            for(int t = 0; t < N; t++){
                cout<< t<<" "<< allStates[visitedStates[t][k]].getNumberFull()<<" "<< allStates[visitedStates[t][k]].getAvgCap()<<" "<< allActions[visitedActions[t][k]].getChargeDischarge()<<" "<<allActions[visitedActions[t][k]].getNumReplace()<<" "<<vBar[t][1][visitedStates[t][k]]<<" "<< W[t][k][replications-1]<<" "<< endl;
            }
        }
        int h0 = (double) round(indexReturn(MBatteries, 1.0, Thr, CapacityIncrement, MBatteries));
        cout<< k<<" "<<vBar[0][1][h0]<<" "<<endl;

                
        
        k++;
        
    }// End iteration loop
    
    
    return vBar[0][1][allStates.size()-1];
}
// ****************************************************************************************************//


double VIforTRE (vector<Action>allActions, vector<State>allStates, vector< double> Alpha, int MBatteries, int N, int Phi, double Thr , double deltaC, double deltaE, int iteration, int replications, vector<double>lambdaVec, int CostReplace, double ExchangeVec, vector<double>ChargeVec, vector<double>DischargeVec, double CapacityIncrement, vector< vector< vector< double> > > &vBar, vector< vector< vector< double> > > &vHat, double LAMBDA ){
    
    vector< double> random(iteration+1, 0.0);
    
    for(int k = 1 ; k<= iteration/10 ; k++){
    
        random [k] = (double) (0.2- 0.2*(10*k/iteration));
    }

    int FutureCharge = 0;
    double FutureCapacity = 0.0;
    double reward = 0.0;
    double uBest = 0;
    //   int Best = -1;
    // Visited state at time t and iteration k.
     vector< vector< double> >  visitedActions (N, vector<double>  (iteration+1, 0.0));
     vector< vector< double> >  visitedStates (N, vector<double> (iteration+1, 0.0));
    vector< vector< double> >  Delta (N, vector<double> (iteration+1, 0.0));


    vector< vector<double> > CurrentContribution (N, vector<double> (iteration+1, 0.0));
    vector< vector<double> > BestCurrentContribution (N, vector<double> (iteration+1, 0.0));
    vector< vector<double> > ExpectedContribution (N, vector<double> (iteration+1, 0.0));
    vector< vector<double> > BestRewardSoFar (N, vector<double> (iteration+1, -1000.0));
    

    
        for(int s = 0; s < allStates.size(); s++){
            vBar[N-1][0][s] = terminalReward(allStates[s], ExchangeVec, N,Thr);
        }
    

    
    // Set the demand at time t and iteration k.
    vector< vector< vector< double> > > W (N, vector< vector<double> > (iteration+1, vector<double>(replications, 0.0)));
    for(int t = 0; t < N; t++){
        for (int k =0 ; k <=iteration ; k++){
            for (int m =0 ; m <replications ; m++){
       //     W[t][k] = 20;
      // if(k<iteration)            W[t][k][m] = GenerateRandomPoisson(lambdaVec[t]);
      //      else       W[t][k][] =  floor(lambdaVec[t]);
                    W[t][k][m]= GenerateRandomPoisson(lambdaVec[t]);
                    if (k==iteration && m == replications-1)
                        W[t][k][m] =  floor(lambdaVec[t]);
                }
        }
    }
    
    int k = 1;
    // Iteration loop
    
    while (k<= iteration){
        
        // Set the initial state of each iteration to be non-absorbing state
    
        do  {
            (visitedStates[0][k]  = (double) mtrand1.randInt(allStates.size()-1));
        } while(allStates[visitedStates[0][k]].getAvgCap() < Thr);
        
        if (k % 2 == 0) {
            visitedStates[0][k] = (double) round(indexReturn(MBatteries, 1.0, Thr, CapacityIncrement, MBatteries));
        }
     
     //   visitedStates[0][k] = (double) round(indexReturn(MBatteries, 1.0, Thr, CapacityIncrement, MBatteries));

        for (int t =0 ; t<N-1 ; t++) {
            int jSelected = -1;

            
            for (int a = 0 ; a<allActions.size() ; a++) {
                 if (CheckPossibleAction(allStates[visitedStates[t][k]], allActions[a], MBatteries, Phi, Thr) ==true)
                {
                    int jFound = -1;

                    for (int m = 0 ; m <replications ; m++){
                    FutureCharge = (double) allStates[visitedStates[t][k]].getNumberFull() + allActions[a].getNumReplace()+ max(allActions[a].getChargeDischarge(),0) - abs(min(allActions[a].getChargeDischarge(),0))- min((double)allStates[visitedStates[t][k]].getNumberFull()-abs(min(allActions[a].getChargeDischarge(),0)), W[t][k][m]);
                    
                    FutureCapacity = (double) CapacityTransition(allStates[visitedStates[t][k]].getNumberFull(), allStates[visitedStates[t][k]].getAvgCap(), allActions[a].getChargeDischarge(), allActions[a].getNumReplace(), FutureCharge, deltaC, deltaE, MBatteries, Thr);
                    
                    jFound = (double) round(indexReturn(FutureCharge, FutureCapacity, Thr, CapacityIncrement, MBatteries));
                    ExpectedContribution[t][k] += vBar[t+1][0][jFound];
                    }
                    ExpectedContribution[t][k] = (1.0/replications)*ExpectedContribution[t][k];
                    
                    CurrentContribution[t][k] = (double) (- CostReplace * allActions[a].getNumReplace()
                                                          - ChargeVec[t] * max(0,allActions[a].getChargeDischarge())
                                                          + DischargeVec[t] * abs(min(0,allActions[a].getChargeDischarge()))
                                                          + ExchangeVec* (1.0 +(allStates[visitedStates[t][k]].getAvgCap()-Thr)/(1.0-Thr)) *
                                                          (min( (double) allStates[visitedStates[t][k]].getNumberFull()
                                                           - abs(min(0,allActions[a].getChargeDischarge())),W[t][k][replications-1])));
                    reward = (double) (CurrentContribution[t][k] + ExpectedContribution[t][k]);
                    
                    double Random = mtrand1.rand();
                    // Find the best action and reward at time t and iteration k.
                    if  (reward >= BestRewardSoFar[t][k] || Random<random[k]){
                        visitedActions[t][k] = (double) a;
                        BestRewardSoFar[t][k] = (double) reward;
                        BestCurrentContribution[t][k] = (double) CurrentContribution[t][k];
                        jSelected= (double) jFound;
                    }
                    
                }
                }//End Action Loop
            visitedStates[t+1][k] = jSelected;
            /*
            Delta[t][k] = (double) BestCurrentContribution[t][k] + (double) vBar[t+1][0][visitedStates[t+1][k]] - (double) vBar[t][0][visitedStates[t][k]];
            
            for (int t1 = t ; t1>=0 ; t1--){
                
                vBar[t1][1][visitedStates[t1][k]] = (double) vBar[t1][0][visitedStates[t1][k]] + (double) Alpha[k]* (double)pow(LAMBDA, t-t1)* (double) Delta[t][k];
                vBar[t1][0][visitedStates[t1][k]] = (double) vBar[t1][1][visitedStates[t1][k]];
                
            }
            */
             
            
            vHat[t][1][visitedStates[t][k]] = BestRewardSoFar[t][k];
            vBar[t][1][visitedStates[t][k]] = (double) (Alpha[k]*vHat[t][1][visitedStates[t][k]] + (1.0 - Alpha[k]) * vBar[t][0][visitedStates[t][k]]);
            vBar[t][0][visitedStates[t][k]] = (double) vBar[t][1][visitedStates[t][k]];
            
            
            if (k==iteration)
                cout<< t<<" "<< allStates[visitedStates[t][k]].getNumberFull()<<" "<< allStates[visitedStates[t][k]].getAvgCap()<<" "<< allActions[visitedActions[t][k]].getChargeDischarge()<<" "<<allActions[visitedActions[t][k]].getNumReplace()<<" "<<vBar[t][1][visitedStates[t][k]]<<" "<< W[t][k][replications-1]<<" "<< endl;



        }// End forward time loop
        int h0 = (double) round(indexReturn(MBatteries, 1.0, Thr, CapacityIncrement, MBatteries));
        cout<< k<<" "<<vBar[0][1][h0]<<" "<<endl;

        k++;
        
    }// End iteration loop
    
    
    return vBar[0][1][allStates.size()-1];
}

// ****************************************************************************************************//


double TDforTRE (vector<Action>allActions, vector<State>allStates, vector< double> Alpha, int MBatteries, int N, int Phi, double Thr , double deltaC, double deltaE, int iteration, int replications, vector<double>lambdaVec, int CostReplace, double ExchangeVec, vector<double>ChargeVec, vector<double>DischargeVec, double CapacityIncrement, vector< vector< vector< double> > > &vBar, vector< vector< vector< double> > > &vHat, double LAMBDA ){
    

    int FutureCharge = 0;
    double FutureCapacity = 0.0;
    double reward = 0.0;
    double uBest = 0;
    //   int Best = -1;
    // Visited state at time t and iteration k.
     vector< vector< double> >  visitedActions (N, vector<double>  (iteration+1, 0.0));
     vector< vector< double> >  visitedStates (N, vector<double> (iteration+1, 0.0));
    vector< vector< double> >  Delta (N, vector<double> (iteration+1, 0.0));


    vector< vector<double> > CurrentContribution (N, vector<double> (iteration+1, 0.0));
    vector< vector<double> > BestCurrentContribution (N, vector<double> (iteration+1, 0.0));
    vector< vector<double> > ExpectedContribution (N, vector<double> (iteration+1, 0.0));
    vector< vector<double> > BestRewardSoFar (N, vector<double> (iteration+1, -1000.0));
    

    
        for(int s = 0; s < allStates.size(); s++){
            vBar[N-1][0][s] = terminalReward(allStates[s], ExchangeVec, N,Thr);
        }


    
    // Set the demand at time t and iteration k.
    vector< vector< vector< double> > > W (N, vector< vector<double> > (iteration+1, vector<double>(replications, 0.0)));
    for(int t = 0; t < N; t++){
        for (int k =0 ; k <=iteration ; k++){
            for (int m =0 ; m <replications ; m++){
       //     W[t][k] = 20;
      // if(k<iteration)            W[t][k][m] = GenerateRandomPoisson(lambdaVec[t]);
      //      else       W[t][k][] =  floor(lambdaVec[t]);
                    W[t][k][m]= GenerateRandomPoisson(lambdaVec[t]);
                    if (k==iteration && m == replications-1)
                        W[t][k][m] =  floor(lambdaVec[t]);
                }
        }
    }
    
    int k = 1;
    // Iteration loop
    
    while (k<= iteration){
        
        // Set the initial state of each iteration to be non-absorbing state
    
        do  {
            (visitedStates[0][k]  = (double) mtrand1.randInt(allStates.size()-1));
        } while(allStates[visitedStates[0][k]].getAvgCap() < Thr);
        
        if (k % 10 == 0) {
            visitedStates[0][k] = (double) round(indexReturn(MBatteries, 1.0, Thr, CapacityIncrement, MBatteries));
        }
     
     //   visitedStates[0][k] = (double) round(indexReturn(MBatteries, 1.0, Thr, CapacityIncrement, MBatteries));

        for (int t =0 ; t<N-1 ; t++) {
            int jSelected = -1;

            for (int a = 0 ; a<allActions.size() ; a++) {
                 if (CheckPossibleAction(allStates[visitedStates[t][k]], allActions[a], MBatteries, Phi, Thr) ==true)
                {
                    int jFound = -1;

                    for (int m = 0 ; m <replications ; m++){
                    FutureCharge = (double) allStates[visitedStates[t][k]].getNumberFull() + allActions[a].getNumReplace()+ max(allActions[a].getChargeDischarge(),0) - abs(min(allActions[a].getChargeDischarge(),0))- min((double)allStates[visitedStates[t][k]].getNumberFull()-abs(min(allActions[a].getChargeDischarge(),0)), W[t][k][m]);
                    
                    FutureCapacity = (double) CapacityTransition(allStates[visitedStates[t][k]].getNumberFull(), allStates[visitedStates[t][k]].getAvgCap(), allActions[a].getChargeDischarge(), allActions[a].getNumReplace(), FutureCharge, deltaC, deltaE, MBatteries, Thr);
                    
                    jFound = (double) round(indexReturn(FutureCharge, FutureCapacity, Thr, CapacityIncrement, MBatteries));
                    ExpectedContribution[t][k] += vBar[t+1][0][jFound];
                    }
                    ExpectedContribution[t][k] = (1.0/replications)*ExpectedContribution[t][k];
                    
                    CurrentContribution[t][k] = (double) (- CostReplace * allActions[a].getNumReplace()
                                                          - ChargeVec[t] * max(0,allActions[a].getChargeDischarge())
                                                          + DischargeVec[t] * abs(min(0,allActions[a].getChargeDischarge()))
                                                          + ExchangeVec* (1.0 +(allStates[visitedStates[t][k]].getAvgCap()-Thr)/(1.0-Thr)) *
                                                          (min( (double) allStates[visitedStates[t][k]].getNumberFull()
                                                           - abs(min(0,allActions[a].getChargeDischarge())),W[t][k][replications-1])));
                    reward = (double) (CurrentContribution[t][k] + ExpectedContribution[t][k]);
                    
                    double Random = mtrand1.rand();
                    // Find the best action and reward at time t and iteration k.
                    if  (reward >= BestRewardSoFar[t][k] || Random < 0.005){
                        visitedActions[t][k] = (double) a;
                        BestRewardSoFar[t][k] = (double) reward;
                        BestCurrentContribution[t][k] = (double) CurrentContribution[t][k];
                        jSelected= (double) jFound;
                    }
                    
                }
                }//End Action Loop

            visitedStates[t+1][k] = jSelected;
            vBar[t][0][visitedStates[t][k]] = BestRewardSoFar[t][k];

            Delta[t][k] = (double) BestCurrentContribution[t][k] + (double) vBar[t+1][0][visitedStates[t+1][k]] - (double) vBar[t][0][visitedStates[t][k]];
            
            for (int t1 = t ; t1>=0 ; t1--){
                
                vBar[t1][1][visitedStates[t1][k]] = (double) vBar[t1][0][visitedStates[t1][k]] + (double) Alpha[k]* (double)pow(LAMBDA, t-t1)* (double) Delta[t][k];
                vBar[t1][0][visitedStates[t1][k]] = (double) vBar[t1][1][visitedStates[t1][k]];
                
            }
            
             
            
            
            if (k==iteration)
                cout<< t<<" "<< allStates[visitedStates[t][k]].getNumberFull()<<" "<< allStates[visitedStates[t][k]].getAvgCap()<<" "<< allActions[visitedActions[t][k]].getChargeDischarge()<<" "<<allActions[visitedActions[t][k]].getNumReplace()<<" "<<vBar[t][1][visitedStates[t][k]]<<" "<< W[t][k][replications-1]<<" "<< endl;



        }// End forward time loop
        /*
        vHat[N-1][1][visitedStates[N-1][k]] = terminalReward(allStates[visitedStates[N-1][k]], ExchangeVec, N, Thr);

        for (int t = N-2; t>=0 ; t--){
        vHat[t][1][visitedStates[t][k]] = BestCurrentContribution[t][k] + vHat[t+1][1][visitedStates[t+1][k]];
        vBar[t][1][visitedStates[t][k]] = (double) (Alpha[k]*vHat[t][1][visitedStates[t][k]] + (1.0 - Alpha[k]) * vBar[t][0][visitedStates[t][k]]);
        vBar[t][0][visitedStates[t][k]] = (double) vBar[t][1][visitedStates[t][k]];
        }
         */
        int h0 = (double) round(indexReturn(MBatteries, 1.0, Thr, CapacityIncrement, MBatteries));
        cout<< k<<" "<<vBar[0][1][h0]<<" "<<endl;

        k++;
        
    }// End iteration loop
    
    
    return vBar[0][1][allStates.size()-1];
}


#endif /* TREADPs_h */

//    if (Random > 0.001){

//    }
 /*
    //RandomAction
    else {
        do  {
            (visitedActions[t][k]  = (double) mtrand1.randInt(allActions.size()-1));
        } while(CheckPossibleAction(allStates[visitedStates[t][k]], allActions[visitedActions[t][k]], MBatteries, Phi, Thr) ==false);

        int jFound = -1;

        FutureCharge = (double) allStates[visitedStates[t][k]].getNumberFull() + allActions[visitedActions[t][k]].getNumReplace()+ max(allActions[visitedActions[t][k] ].getChargeDischarge(),0) - abs(min(allActions[visitedActions[t][k] ].getChargeDischarge(),0))- min((double)allStates[visitedStates[t][k]].getNumberFull()-abs(min(allActions[visitedActions[t][k] ].getChargeDischarge(),0)), W[t][k]);
        
        FutureCapacity = (double) CapacityTransition(allStates[visitedStates[t][k]].getNumberFull(), allStates[visitedStates[t][k]].getAvgCap(), allActions[visitedActions[t][k] ].getChargeDischarge(), allActions[visitedActions[t][k]].getNumReplace(), FutureCharge, deltaC, deltaE, MBatteries, Thr);
        
        jFound = (double) round(indexReturn(FutureCharge, FutureCapacity, Thr, CapacityIncrement, MBatteries));
        ExpectedContribution[t][k] = vBar[t+1][0][jFound];
        
        CurrentContribution[t][k] = (double) (- CostReplace * allActions[visitedActions[t][k] ].getNumReplace()
                                              - ChargeVec[t] * max(0,allActions[visitedActions[t][k] ].getChargeDischarge())
                                              + DischargeVec[t] * abs(min(0,allActions[visitedActions[t][k] ].getChargeDischarge()))
                                              + ExchangeVec* (1.0 +(allStates[visitedStates[t][k]].getAvgCap()-Thr)/(1.0-Thr)) *
                                              (min( (double) allStates[visitedStates[t][k]].getNumberFull()
                                               - abs(min(0,allActions[visitedActions[t][k] ].getChargeDischarge())),W[t][k])));
        reward = (double) (CurrentContribution[t][k] + ExpectedContribution[t][k]);
        
        BestRewardSoFar[t][k] = (double) reward;
        BestCurrentContribution[t][k] = (double) CurrentContribution[t][k];
        jSelected= (double) jFound;
    }
  */
