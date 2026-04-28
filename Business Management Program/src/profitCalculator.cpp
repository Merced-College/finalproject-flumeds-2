/*
Author: Keiry & Brian
Class: profitCalculator
Description: A utility class for calculating profit based on service charges, 
supply costs, and labor hours/rates.
*/
#include <iostream>
#include "profitCalculator.h"

// hours worked × pay rate
double profitCalculator::calcLaborCost() {
    return laborHours * laborRate;
}

/*
Author: Keiry
Algorithm: Calculation algorithm 
This function calculates the net profit using the formula: Charged - Supplies - Labor
Time Complexity: O(1)
*/
// charged - supplies - labor = profit
double profitCalculator::calculateProfit() {
    return amountCharged - supplyCost - calcLaborCost();
}

// was it Profitable
bool profitCalculator::wasProfitable() {
    return calculateProfit() > 0;
}

// print a full breakdown
void profitCalculator::printResults() {
    std::cout << "Amount Charged: $" << amountCharged    << "\n";
    std::cout << "Supply Cost:   -$" << supplyCost       << "\n";
    std::cout << "Labor Cost:    -$" << calcLaborCost() << "\n";
    std::cout << "─────────────────────\n";
    std::cout << "Net Profit:     $" << calculateProfit() << "\n";

    if (wasProfitable()) {
        std::cout << "This job was profitable!\n";
    } else {
        std::cout << "Warning: This job lost money.\n";
    }
}