/*
Author: Keiry & Brian
Class: quote
Description: Implementation for the quote class logic.
*/
#include "quote.h"
#include <iostream>

double quote::calculateLaborCost() {
    return laborHours * laborRate;
}

double quote::calculateTotal() {
    return supplyCost + calculateLaborCost();
}

double quote::estimatedProfit() {
    return amountCharged - calculateTotal();
}

void quote::printQuote() {
    std::cout << "================================\n";
    std::cout << "         QUOTE ESTIMATE         \n";
    std::cout << "================================\n";
    std::cout << "Customer:  " << customerName  << "\n";
    std::cout << "Phone:     " << customerPhone << "\n";
    std::cout << "Service:   " << serviceType   << "\n";
    std::cout << "Vehicle:   " << vehicleDesc   << "\n";
    std::cout << "Date:      " << date          << "\n";
    std::cout << "--------------------------------\n";
    std::cout << "Supply Cost:   $" << supplyCost          << "\n";
    std::cout << "Labor Cost:    $" << calculateLaborCost() << "\n";
    std::cout << "--------------------------------\n";
    std::cout << "Total Quote:   $" << amountCharged        << "\n";
    std::cout << "Est. Profit:   $" << estimatedProfit()    << "\n";
    std::cout << "================================\n";
    if (notes != "") {
        std::cout << "Notes: " << notes << "\n";
    }
}

/*
Author: Brian
Algorithm: Selection Sort
This function sorts a list of quotes based on their estimated profit
from highest profit to lowest profit.
Time Complexity: O(n^2)
*/
void quote::sortQuotesByProfit(std::vector<quote>& quotes) {
    int n = quotes.size();
    for (int i = 0; i < n - 1; ++i) {
        int maxIndex = i;
        for (int j = i + 1; j < n; ++j) {
            // Compare estimated profit
            if (quotes[j].estimatedProfit() > quotes[maxIndex].estimatedProfit()) {
                maxIndex = j;
            }
        }
        // Swap if a higher profit is found
        if (maxIndex != i) {
            quote temp = quotes[i];
            quotes[i] = quotes[maxIndex];
            quotes[maxIndex] = temp;
        }
    }
}