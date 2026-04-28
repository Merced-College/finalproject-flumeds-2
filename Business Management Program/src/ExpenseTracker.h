/*
Author: Keiry & Brian
Class: ExpenseTracker
Description: Manages a collection of business expenses. Provides methods
for adding expenses, calculating totals, and loading/saving to a file.
Uses a vector for storage and a map for grouped analytics.
*/
#ifndef EXPENSETRACKER_H
#define EXPENSETRACKER_H

#include <vector>
#include <string>
#include <map>
#include "Expense.h"

class ExpenseTracker {
private:
    std::vector<Expense> expenses;
    int nextId = 1; // Auto-incrementing ID for new expenses

public:
    // Core Management
    void addExpense(double amount, std::string category, std::string date, std::string desc);
    const std::vector<Expense>& getAllExpenses() const;

    // Analytics 
    double getTotalExpensesByMonth(std::string yearMonth) const; // Input: "2026-04"
    double getTotalByCategory(std::string category) const;
    std::map<std::string, double> getExpensesGroupedByCategory() const;

    // Persistence
    void loadFromCSV(std::string filename);
    void saveToCSV(std::string filename);
};

#endif