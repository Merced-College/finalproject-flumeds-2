/*
Author: Keiry & Brian
Class: invoices
Description: Implementation for the invoices class.
*/
#include "invoices.h"
#include <iostream>

// Implementation of the constructor
invoices::invoices(int id, int cId, std::string name, std::string date, double amt, bool paid)
    : id(id), customerId(cId), customerName(name), date(date), amount(amt), isPaid(paid) {}

void invoices::printInvoice() {
    std::cout << "================================\n";
    std::cout << "           INVOICE              \n";
    std::cout << "================================\n";
    std::cout << "Invoice ID:  " << id << "\n";
    std::cout << "Customer:    " << customerName << " (ID: " << customerId << ")\n";
    std::cout << "Date:        " << date << "\n";
    std::cout << "Status:      " << (isPaid ? "PAID" : "UNPAID") << "\n";
    std::cout << "--------------------------------\n";
    std::cout << "Total Due:   $" << amount << "\n";
    std::cout << "================================\n";
}

//Used AI to help us with the library
json invoices::toJson() const {
    return json{
        {"id", id},
        {"customerId", customerId},
        {"customerName", customerName},
        {"date", date},
        {"amount", amount},
        {"isPaid", isPaid}
    };
}