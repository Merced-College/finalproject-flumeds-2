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