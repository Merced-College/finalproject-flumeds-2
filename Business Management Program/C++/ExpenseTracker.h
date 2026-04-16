#ifndef EXPENSETRACKER_H
#define EXPENSETRACKER_H

#include <vector>
#include <string>
#include "Expense.h"

class ExpenseTracker {
private:
    std::vector<Expense> expenses;
    int nextId = 1; // Auto-incrementing ID for new expenses

public:
    // Core Management
    void addExpense(double amount, std::string category, std::string date, std::string desc);
    const std::vector<Expense>& getAllExpenses() const;

    // Analytics (Used by ProfitTrend class)
    double getTotalExpensesByMonth(std::string yearMonth) const; // Input: "2026-04"
    double getTotalByCategory(std::string category) const;

    // Persistence
    void loadFromCSV(std::string filename);
    void saveToCSV(std::string filename);
};

#endif