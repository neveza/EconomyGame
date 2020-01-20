/*
    Line class: used for creating supply and demand curves,
    but many functions can be used for general linear expression.
*/

#include "Line.h"
#include <iostream>
/*
    assigns required property for the line.
    paramaters:
        m: slope,
        gdp: starting x position,
        price: starting y position,
        symbol: character symbol to print (currently required, but isn't used)
*/
Line::Line(double m, double gdp, double price, char sym)
{
    slope = m;
    lineMid[0] = gdp;
    lineMid[1] = price;
    yIntercept = price - slope * gdp;
    symbol = sym;
}

/*
    updates the linear equation by shifting along the inverse line
    Parameters:
        econ: Economy structure
        inverseLine: Line class of the inverse line.
        tax: the change of GDP via Tax
    post-condition: yIntercept is altered changing the linear equation
    Issues:
        The line has to behave like a curve, but the current solution
        doesn't work well, and still has moments of derailing or 
        going out of bounds of the array
*/
void Line::updateLine(Economy& econ, Line& inverseLine, double tax)
{
    //double tempGDP = econ.realGDP + tax;
    //cout << "MAX X: " << inverseLine.calcX(20) << " MIN X: " << inverseLine.calcX(1) << endl;
    if (tax != 0)
    {
        double tempGDP = econ.realGDP + tax;
        double tempPrice = inverseLine.calcY(econ.realGDP);
        //std::cout << "tempGDP: before " << tempGDP;
        //std::cout << "MAX X: " << inverseLine.xMax << " MIN X: " << inverseLine.xMin << std::endl;
        /*
            Current solution to 'mimic' a curve by using xMax/xMin and yMax/yMin
            as limits to the linaer shift. The x and y max/min are assigned through updateCurve.
            Rather find a better solution.
        */
        if (tempGDP > inverseLine.xMax)
        {
            tempGDP = inverseLine.xMax;
        }
        else if (tempGDP < inverseLine.xMin)
        {
            tempGDP = inverseLine.xMin;
        }
        //not ideal, but it keeps the program from crashing (sort of)
        if (tempPrice < 0)
        {
            tempPrice = 0;
        }
        if (tempPrice > 20)
        {
            tempPrice = 20;
        }

        //std::cout << "tempGDP:  after " << tempGDP << std::endl;
         econ.realGDP = tempGDP;
         econ.marketPrice = tempPrice;
         lineMid[0] = econ.realGDP;
         lineMid[1] = econ.marketPrice;
         yIntercept = econ.marketPrice - slope * (econ.realGDP);
    }
}

/*
    updates the curve on the aggregate graph array.
    parameter:
        econ: Economy structure
    Post-condition: econ.aggGraph is altered,
                    xMin, xMax, yMin, yMax are altered.
    issues: Current solutions for populating the graph is not ideal,
        the commented out solution works, but not as well as the crude version.
*/
void Line::updateCurve(Economy& econ)
{ 
    //cout << "assigning x..";
    int x = lineMid[X]/10;
    //cout << "  assigning y..." << endl;
    int y = lineMid[Y];
    econ.aggGraph[x][y] = symbol;
    //cout << "x: " << x << " y: " << y << endl;
    int ySub = y;
    int aMod = 1;
    if (slope < 0)
    {
        for(int i = x-1; i > 0; i--)
        {
            ySub += aMod;
            aMod += 2;
            if (ySub < 20 && ySub > 0)
            {
                econ.aggGraph[i][ySub] = 'd';
                //cout << "x: " << i
                //<< "y: " << ySub << " " << endl; //debug
                xMin = i * 10;
            }
        }

        ySub = y;
        aMod = 1;
        for(int i = x+1; i < 25; i++)
        {
            ySub -= aMod;
            ++aMod;
            for (int n = 0; n < 5; n++)
            {
                if (ySub < MAX_Y && ySub > 0 &&
                    i < MAX_X)
                {
                    econ.aggGraph[i][ySub] = 'd';
                    //cout << "x: " << i
                     //<< "y: " << ySub << endl; //debug
                     xMax = i * 10;
                }
                ++i;
            }
        }
    }
    else
    {
        ySub = y;
        aMod = 1;
        for(int i = x+1; i < MAX_X; i++)
        {
            ySub += aMod;
            aMod += 2;
            if (ySub < 20 && ySub > 0)
            {
                econ.aggGraph[i][ySub] = 's';
                //cout << "x: " << i
                    //<< "y: " << ySub << " " << endl; //debug
                xMax = i * 10;
            }
        }

        ySub = y;
        aMod = 1;
        for(int i = x-1; i > 0; i--)
        {
            ySub -= aMod;
            ++aMod;
            for (int n = 0; n < 5; n++)
            {
                if (ySub < MAX_Y && ySub > 0 &&
                    i > 0)
                {
                    econ.aggGraph[i][ySub] = 's';
                    //cout << "x: " << i
                        //<< "y: " << ySub << endl; //debug
                    xMin = i * 10;
                }
                --i;
            }
        }
    }
//has potential, but just never looked right.
/*    int signChange = 1;
    int loop = x;
    int ySub = 0;
    int aMod = 1;
    if (slope < 0)
    {
        loop = 25-x;
        ++x;
        signChange = -1;
        ySub = y;
    }
    else
    {
        x = lineMid[X]/10;
        ySub = y;
    }
    for (int i = 0; i < loop; i++)
    {
        ySub -= aMod;
        aMod++;
        for (int n = 0; n < 5; n++)
        {
            //cout << " x: " << x << " i:" << i;
            if (i != loop && x < MAX_X)
            {
                econ.aggGraph[x][ySub] = symbol;
            }
            x += -1*signChange;
            i++;
        }
    }

    ySub = y;
    aMod = 1;
    x = lineMid[0]/10;
    x += 1*signChange;
    while(x > 0 && x < MAX_X)
    {
        ySub += aMod;
        aMod += 2;
        if (ySub < MAX_Y && ySub > 0)
        {
            econ.aggGraph[x][ySub] = symbol;
            //cout << "x: " << i
            //<< "y: " << ySub1 << " " << endl; //debug
        }
        x += 1*signChange;
    } */
}

double Line::calcY(double x){
    return slope * (x) + yIntercept;
}

double Line::calcX(double y)
{
    return (y - yIntercept)/slope;
}