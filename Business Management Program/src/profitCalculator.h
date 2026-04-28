/*
Author: Keiry & Brian
Class: profitCalculator
Description: 
*/



#pragma once
#include <string>

class profitCalculator {
public :
    double amountCharged;
    double laborRate;
    double laborHours;
    double supplyCost;

    //function to caluculate the values

    double calculateProfit();
    double calcLaborCost();
    bool wasProfitable();
    void printResults();
};

