/*
Author: Keiry & Brian
Class: quote
Description: Represents a service quote given to a customer, including
labor hours, supply costs, and estimated profit.
*/


#pragma once
#include <string>
#include <vector>

class quote {
public:
    int id;
    std::string customerName;
    std::string customerPhone;
    std::string serviceType;
    std::string vehicleDesc;
    std::string date;
    std::string notes;

    double laborHours;
    double laborRate;
    double supplyCost;
    double amountCharged;

    // functions
    double calculateTotal();
    double calculateLaborCost();
    double estimatedProfit();
    void   printQuote();

    // sorting algorithms
    static void sortQuotesByProfit(std::vector<quote>& quotes);
};