#pragma once
#include <string>

class quote {
public:
    int    id;
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
};