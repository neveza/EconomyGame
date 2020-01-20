#ifndef LINE_H
#define DATE_H

const  int MAX_X = 25;
const  int MAX_Y = 20;
const double FE_GDP = 180;

//Economy data, may change into a proper table
struct Economy
{
    double marketPrice = 10; //y
    double priceIndex = 100;
    double realGDP = FE_GDP; //x
    char aggGraph[MAX_X][MAX_Y];
};
 
//generates lines based on the linear equation y=mx+c
class Line
{
    const int X = 0;
    const int Y = 1;
    double slope;
    double yIntercept;
    double lineMid[2];
    double xMin;
    double xMax;
    //double yMin;
    //double yMin;
    char symbol;
public:
    //Line();
    Line(double, double, double, char);
    void updateLine(Economy&, Line&, double);
    void updateCurve(Economy&);
    double calcY(double);
    double calcX(double);
};


#endif