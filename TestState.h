/*--------------------------------------------------------------------------*/
/*------------------------------ File State.h ------------------------------*/
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
#ifndef TestState_h
#define TestState_h
/* self-identification: #endif at the end of the file */

/*--------------------------------------------------------------------------*/
/*------------------------------ INCLUDES ----------------------------------*/
/*--------------------------------------------------------------------------*/
#include <vector>

/*--------------------------------------------------------------------------*/
/*------------------------------ NAMESPACE ---------------------------------*/
/*--------------------------------------------------------------------------*/

using namespace std;

/*--------------------------------------------------------------------------*/
/*------------------------------ CLASS State -------------------------------*/
/*--------------------------------------------------------------------------*/
/*----------------------------- GENERAL NOTES ------------------------------*/
/*--------------------------------------------------------------------------*/

/// Implementation of the State concept for the ReAl problem
class multi_array;

/** The State class contains all the information related to the State and
 * Action in the Real problem, and it implements the needed basic functions.*/

class State{

public:



/**@} ----------------------------------------------------------------------*/
/*--------------- METHODS FOR READING THE DATA OF THE State ----------------*/
/*--------------------------------------------------------------------------*/
/** @name Reading the data of the State
 *
 * These methods allow to read data that must be common to (in principle) all
 * the problem
 * @{ */

    /// Returns the number of fuel stations
//  int get_number_fuel_station() const { return NumGasStation; }

/*--------------------------------------------------------------------------*/

/**@} ----------------------------------------------------------------------*/
/*-------------- METHODS FOR READING THE Variable OF THE State -------------*/
/*--------------------------------------------------------------------------*/
/** @name Reading the Variable of the State
 *
 * These methods allow to read the Variable of the problem.
 *  @{ */

    /// returns the vector of fuel level level variables
    int get_fuel_level() const {
        return FuLevel;
    }

/*--------------------------------------------------------------------------*/
    /// returns the vector of cumulative driving time
    int get_cumulative_driving_time() const {
        return DriveTime;
    }

/**@} ----------------------------------------------------------------------*/
/*-------------------- METHODS FOR INITIALIZING THE State ------------------*/
/*--------------------------------------------------------------------------*/

/** @name Handling the data of the problem
    @{ */

    void load( std::istream & input ) const {
        throw ( std::logic_error( "State::load() not "
                                  "implemented yet") );
    };

/**@} ----------------------------------------------------------------------*/
/*---------------------------------data-------------------------------------*/
/*--------------------------------------------------------------------------*/

    /// The number of fuel station on each route
    //int NumGasStation = 5;

/*-----------------------------variables------------------------------------*/
    /// The fuel level of each tank
    int  FuLevel;
    int LB = 100;
    int UB = 800;
    int index;

    /// The cumulative driving time of the truck.
    int  MaxPerDay = 540;
    int  DriveTime = 0;
    int  PresentDay = 0;


}; // end( class( State ) )

/*--------------------------------------------------------------------------*/
/*----------------------------- CLASS Action -------------------------------*/
/*--------------------------------------------------------------------------*/
/*----------------------------- GENERAL NOTES ------------------------------*/
/*--------------------------------------------------------------------------*/

class Action{
public:

/**@} ----------------------------------------------------------------------*/
/*--------------- METHODS FOR READING THE DATA OF THE Action ---------------*/
/*--------------------------------------------------------------------------*/
/** @name Reading the data of the Action
 *
 * These methods allow to read data that must be common to (in principle) all
 * the problem
 * @{ */

/** @name Handling the data of the Action
    @{ */

    void load( std::istream & input ) const {
        throw ( std::logic_error( "Action::load() not "
                                  "implemented yet") );
    };

/**@} ----------------------------------------------------------------------*/
/*---------------------------------data-------------------------------------*/
/*--------------------------------------------------------------------------*/
    /// The lower bound of each tank
    int MinimumAmountRefuel = 200;

    int MaximumAmountRefuel = 800;

    /// The amount of increment
    int Increment = 10;
/*-----------------------------variables------------------------------------*/

    /// The amount of refueling of each tank
    double Refueling;

}; // end( class( Action ) )

vector<Action> createAllActions()
{
    vector<Action> allActions;
    Action a;
    a.Refueling = 0.0;
    allActions.push_back(a);
    for( int i = a.MinimumAmountRefuel/a.Increment; i <= a.MaximumAmountRefuel/a.Increment; i++ )
    {

        a.Refueling = i*a.Increment;
        allActions.push_back(a);

    }
    return allActions;
}


vector<State> createAllStates()
{
    vector<State> allStates;
    State s;
    for( int i = s.LB; i <= s.UB; i++ )
    {
        s.FuLevel = i;
        s.index = i - s.LB ;
        allStates.push_back(s);
    }
    return allStates;
}

/*
vector<State> createAllStates()
{
    vector<State> allStates;
    State s;

    int counter = 0;
    for( int i = s.LB; i <= s.UB; i++ )
    {
        s.FuLevel = i;
        //  s.index = i - s.LB;
        for (int j = 0 ; j < s.MaxDriveTime; j++) {

            s.DriveTime = j;
            for (int k = 0 ; k < s.MaxDay; k++) {
                s.PresentDay =k;
                s.index = counter;
                counter++;
                allStates.push_back(s);
            }
        }
    }
    return allStates;
}*/


#endif /* TestState_h */

/*--------------------------------------------------------------------------*/
/*---------------------------- End File State.h ----------------------------*/
/*--------------------------------------------------------------------------*/