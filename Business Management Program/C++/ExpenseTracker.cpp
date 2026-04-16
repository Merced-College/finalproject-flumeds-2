#include "ExpenseTracker.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

// Logic to add a new expense and assign it a unique ID
void ExpenseTracker::addExpense(double amount, std::string category, std::string date, std::string desc) {
    Expense newExp(nextId++, amount, category, date, desc);
    expenses.push_back(newExp);
}

void ExpenseTracker::loadFromCSV(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No " << filename << " found\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int    id;
        double amount;
        std::string category, date, desc;
        std::getline(ss, token, ','); id       = std::stoi(token);
        std::getline(ss, token, ','); amount   = std::stod(token);
        std::getline(ss, token, ','); category = token;
        std::getline(ss, token, ','); date     = token;
        std::getline(ss, token, ','); desc     = token;
        expenses.push_back(Expense(id, amount, category, date, desc));
        if (id >= nextId) nextId = id + 1;
    }
    std::cout << "Loaded " << expenses.size() << " expenses\n";
}

const std::vector<Expense>& ExpenseTracker::getAllExpenses() const {
    return expenses;
}

// This is the "Engine" for your Profit Trend algorithm
// yearMonth format should be "YYYY-MM" (e.g., "2026-04")
double ExpenseTracker::getTotalExpensesByMonth(std::string yearMonth) const {
    double total = 0.0;
    for (const auto& exp : expenses) {
        // Check if the expense date starts with the year-month string
        if (exp.getDate().find(yearMonth) == 0) {
            total += exp.getAmount();
        }
    }
    return total;
}

// Logic to see where most of the money is going
double ExpenseTracker::getTotalByCategory(std::string category) const {
    double total = 0.0;
    for (const auto& exp : expenses) {
        if (exp.getCategory() == category) {
            total += exp.getAmount();
        }
    }
    return total;
}

// Persistence: Saving data so it's there the next time the app opens
void ExpenseTracker::saveToCSV(std::string filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& exp : expenses) {
            file << exp.getId() << "," 
                 << exp.getAmount() << "," 
                 << exp.getCategory() << "," 
                 << exp.getDate() << "," 
                 << exp.getDescription() << "\n";
        }
        file.close();
    }
}