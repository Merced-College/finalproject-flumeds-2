/*
Author: Keiry & Brian
Class: Expense
Description: Represents a single business expense.
*/
#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
#include "json.hpp"
//#include <nlohmann/json.hpp> // For JSON serialization

class Expense {
private:
    int id;
    double amount;
    std::string category;    // e.g., "Supplies", "Labor", "Gas"
    std::string date;        // Format: "YYYY-MM-DD" (crucial for trend logic)
    std::string description;

public:
    Expense(int id, double amount, std::string cat, std::string date, std::string desc);

    std::string getYearMonth() const;

    // Getters
    int getId() const { return id; }
    double getAmount() const { return amount; }
    std::string getCategory() const { return category; }
    std::string getDate() const { return date; }
    std::string getDescription() const { return description; }
    
    //Used AI to help us with the library
    // Helper for C++/Javascript bridge
    nlohmann::json toJson() const;
};

#endif