/*
Author: Keiry & Brian 
Class: dataManager
Description: Handles all file I/O operations (saving and loading CSV files)
for the various business entities.
*/
#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "json.hpp"
#include "customer.h"
#include "quote.h"
#include "invoices.h"
#include "Employee.h"
#include "Expense.h"
#include "ExpenseTracker.h"
#include "appointment.h"

using json = nlohmann::json;

class dataManager {
public:
    // customers
    std::vector<customer> loadCustomers(const std::string& filename);
    void saveCustomers(const std::string& filename, std::vector<customer>& customers);

    // quotes
    std::vector<quote> loadQuotes(const std::string& filename);
    void saveQuotes(const std::string& filename, std::vector<quote>& quotes);

    // invoices
    std::vector<invoices> loadInvoices(const std::string& filename);
    void saveInvoices(const std::string& filename, std::vector<invoices>& invoiceList);

    // employees
    std::vector<Employee> loadEmployees(const std::string& filename);
    void saveEmployees(const std::string& filename, std::vector<Employee>& employees);

    // expenses
    void loadExpenses(const std::string& filename,
    ExpenseTracker& tracker);
    void saveExpenses(const std::string& filename,
    ExpenseTracker& tracker);

    // appointments
    std::vector<appointment>  loadAppointments(const std::string& filename);
    void saveAppointments(const std::string& filename, std::vector<appointment>& appointments);
};