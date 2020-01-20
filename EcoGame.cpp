/*
    POST THOUGHT: May rework to have a table as the driving data force than the graph/linear equation.
    Not accurate enough due to gaps of understanding regarding how these curves are formulated, and the data necessary

    On further testing, might be okay enough. Need further play throughs.
    
    Description: The program sets up a scenario where the supply and demand are intersecting outside of the
    full employment(FE) GDP. The user must shift the the GDP by manipulating supply and/or demand until at FE.

    Current issues:
        Out of bound crash: Occasionally, the y-axis goes out of bound crashing the software.
                            - Not entirely sure *where* it's going out of bounds at the moment.
        
*/


#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Line.h"

using namespace std;

struct Tax{
    double incomeTax = 0;
    double businessTax = 0;
};


void printAggGraph(Economy& econ, Line& demand, Line& supply);
double genGDP(int option);

double MAX_GDP = 250;
double MAX_PRICE = 20;
const Economy BASE_YEAR;

const bool SUCCESS = true;

int main()
{
    Economy econ;
    Tax tax;
    //double m = econ.marketPrice/econ.realGDP;
    double m = MAX_PRICE/MAX_GDP;
    Line Supply(m, econ.realGDP, econ.marketPrice, 's');
    Line Demand(-m, econ.realGDP, econ.marketPrice, 'd');
    //Supply.updateLine(econ, Demand, 0);
    //Demand.updateLine(econ, Supply, 0);
    Supply.updateCurve(econ);
    Demand.updateCurve(econ);
    srand(time(0));

    int s = rand() % 2;
    Supply.updateLine(econ, Demand, genGDP(s));
    Demand.updateLine(econ, Supply, genGDP(s));

    int attempt = 0;
    bool isWin = false;

    while(attempt < 6 && isWin != true)
    {
        cout << "Full Employment: " << FE_GDP << endl;
        cout << "realGDP: " << econ.realGDP
             << " SupplyGDP: " << Supply.calcX(econ.marketPrice)
             << " DemandGDP: " << Demand.calcX(econ.marketPrice) << endl
             << "marketPrice: " << econ.marketPrice
             << ", " << Demand.calcY(econ.realGDP)
             << ", " << Supply.calcY(econ.realGDP) << endl;    
        printAggGraph(econ, Demand, Supply);
        cout << "\nPlease enter income tax in x.xx form: ";
        cin  >> tax.incomeTax;

        cout << "\nPlease enter business tax in x.xx form: ";
        cin  >> tax.businessTax;

        Supply.updateLine(econ, Demand, tax.businessTax);
        Demand.updateLine(econ, Supply, tax.incomeTax);
        if (econ.realGDP == FE_GDP)
        {
            isWin = SUCCESS;
            cout << "Full Employment: " << FE_GDP << endl;
            cout << "realGDP: " << econ.realGDP
                 << " SupplyGDP: " << Supply.calcX(econ.marketPrice)
                 << " DemandGDP: " << Demand.calcX(econ.marketPrice) << endl
                 << "marketPrice: " << econ.marketPrice << endl;
            printAggGraph(econ, Demand, Supply);
            cout << "\nYou have made the GDP To Full Employment!" << endl;
        }
        attempt++;
    }

    if (attempt >= 6)
    {
        cout << "Full Employment: " << FE_GDP << endl;
        cout << "realGDP: " << econ.realGDP
             << " SupplyGDP: " << Supply.calcX(econ.marketPrice)
             << " DemandGDP: " << Demand.calcX(econ.marketPrice) << endl
             << "marketPrice: " << econ.marketPrice << endl;
        printAggGraph(econ, Demand, Supply);
        cout << "\nYou have destroyed the economy, good job." << endl;
    }

    return 0;
}

/*
   Generates GDP change, used primarily for initial start up.
   Parameter: option: option number for which set of values to return
   option 0: returns between negative 10 and (FE_GDP - 80)
   option 1: returns between positive 10 and 49;
*/
double genGDP(int option)
{
    if (option == 1)
    {
        return rand() % 50 + 10;
    }
    else
    {
        int r = FE_GDP - 80;
        return (rand() % r + 10) * -1;
    }
    
}

/*
   Prints the aggregate graph.
*/
void printAggGraph(Economy& econ, Line& demand, Line& supply)
{
    //cleans the array into blanks
    for (int i = 0; i < 25; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            econ.aggGraph[i][j] = ' ';
        }
    }

    //populates the Full Employment line
    int i = FE_GDP/10;
    for (int j = 19; j >= 0; --j)
    {
        econ.aggGraph[i][j] = 'F';
    }

    cout << "Updating curves.." << endl;
    supply.updateCurve(econ);
    demand.updateCurve(econ);

    cout << "printing graph..." << endl;

    for (int i = 19; i >= 0; i--)
    {
        cout.width(2);
        cout << i; //print y-scale
        for(int j = 0; j < 25; j++)
        {
            cout << " " << econ.aggGraph[j][i];
            //cout << " " << j;
        }
        cout << endl;
    }
    cout << "  ";
    //prints the x-scale
    for (int i = 0; i < 25; i++)
    {
        cout << " " << i;
    }

}