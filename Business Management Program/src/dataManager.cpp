/*
Author: Keiry & Brian
Class: dataManager
Description: Implementation for the dataManager class.
*/
#include "dataManager.h"

//Data structures and file I/O for customers, appointments, quotes, invoices, employees, and expenses
std::vector<customer> dataManager::loadCustomers(const std::string& filename) {
    std::vector<customer> customers;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No " << filename << " found\n";
        return customers;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int id;
        std::string name, phone, email, address;

        std::getline(ss, token, ','); id      = std::stoi(token);
        std::getline(ss, token, ','); name    = token;
        std::getline(ss, token, ','); phone   = token;
        std::getline(ss, token, ','); email   = token;
        std::getline(ss, token, ','); address = token;

        customers.push_back(customer(id, name, phone, email, address));
    }

    std::cout << "Loaded " << customers.size() << " customers\n";
    return customers;
}

void dataManager::saveCustomers(const std::string& filename,
                                 std::vector<customer>& customers) {
    std::ofstream file(filename);
    for (auto& c : customers) {
        file << c.getId()      << ","
             << c.getName()    << ","
             << c.getPhone()   << ","
             << c.getEmail()   << ","
             << c.getAddress() << "\n";
    }
    std::cout << "Saved " << customers.size() << " customers\n";
}

// QUOTES
//Data structures and file I/O for customers, appointments, quotes, invoices, employees, and expenses
std::vector<quote> dataManager::loadQuotes(const std::string& filename) {
    std::vector<quote> quotes;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No " << filename << " found\n";
        return quotes;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        quote q;

        std::getline(ss, token, ','); q.id            = std::stoi(token);
        std::getline(ss, token, ','); q.customerName  = token;
        std::getline(ss, token, ','); q.customerPhone = token;
        std::getline(ss, token, ','); q.serviceType   = token;
        std::getline(ss, token, ','); q.vehicleDesc   = token;
        std::getline(ss, token, ','); q.date          = token;
        std::getline(ss, token, ','); q.supplyCost    = std::stod(token);
        std::getline(ss, token, ','); q.laborHours    = std::stod(token);
        std::getline(ss, token, ','); q.laborRate     = std::stod(token);
        std::getline(ss, token, ','); q.amountCharged = std::stod(token);
        std::getline(ss, token, ','); q.notes         = token;

        quotes.push_back(q);
    }

    std::cout << "Loaded " << quotes.size() << " quotes\n";
    return quotes;
}

void dataManager::saveQuotes(const std::string& filename,
                              std::vector<quote>& quotes) {
    std::ofstream file(filename);
    for (auto& q : quotes) {
        file << q.id            << ","
             << q.customerName  << ","
             << q.customerPhone << ","
             << q.serviceType   << ","
             << q.vehicleDesc   << ","
             << q.date          << ","
             << q.supplyCost    << ","
             << q.laborHours    << ","
             << q.laborRate     << ","
             << q.amountCharged << ","
             << q.notes         << "\n";
    }
    std::cout << "Saved " << quotes.size() << " quotes\n";
}

// INVOICES
//Data structures and file I/O for customers, appointments, quotes, invoices, employees, and expenses
std::vector<invoices> dataManager::loadInvoices(const std::string& filename) {
    std::vector<invoices> invoiceList;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No " << filename << " found\n";
        return invoiceList;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int    id, cId;
        std::string name, date;
        double amount;
        bool   paid;

        std::getline(ss, token, ','); id     = std::stoi(token);
        std::getline(ss, token, ','); cId    = std::stoi(token);
        std::getline(ss, token, ','); name   = token;
        std::getline(ss, token, ','); date   = token;
        std::getline(ss, token, ','); amount = std::stod(token);
        std::getline(ss, token, ','); paid   = (token == "1");

        invoiceList.push_back(invoices(id, cId, name, date, amount, paid));
    }

    std::cout << "Loaded " << invoiceList.size() << " invoices\n";
    return invoiceList;
}

void dataManager::saveInvoices(const std::string& filename,
                                std::vector<invoices>& invoiceList) {
    std::ofstream file(filename);
    for (auto& inv : invoiceList) {
        file << inv.id           << ","
             << inv.customerId   << ","
             << inv.customerName << ","
             << inv.date         << ","
             << inv.amount       << ","
             << (inv.isPaid ? "1" : "0") << "\n";
    }
    std::cout << "Saved " << invoiceList.size() << " invoices\n";
}

// EMPLOYEES
//Data structures and file I/O for customers, appointments, quotes, invoices, employees, and expenses
std::vector<Employee> dataManager::loadEmployees(const std::string& filename) {
    std::vector<Employee> employees;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No " << filename << " found\n";
        return employees;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int    id;
        std::string name, jobType, phone, email;
        double hourlyRate, hoursWorked;

        std::getline(ss, token, ','); id          = std::stoi(token);
        std::getline(ss, token, ','); name        = token;
        std::getline(ss, token, ','); jobType     = token;
        std::getline(ss, token, ','); hourlyRate  = std::stod(token);
        std::getline(ss, token, ','); hoursWorked = std::stod(token);
        std::getline(ss, token, ','); phone       = token;
        std::getline(ss, token, ','); email       = token;

        Employee e(id, name, jobType, hourlyRate, phone, email);
        e.setHoursWorked(hoursWorked);
        employees.push_back(e);
    }

    std::cout << "Loaded " << employees.size() << " employees\n";
    return employees;
}

void dataManager::saveEmployees(const std::string& filename,
                                 std::vector<Employee>& employees) {
    std::ofstream file(filename);
    for (auto& e : employees) {
        file << e.getId()          << ","
             << e.getName()        << ","
             << e.getJobType()     << ","
             << e.getHourlyRate()  << ","
             << e.getHoursWorked() << ","
             << e.getPhone()       << ","
             << e.getEmail()       << "\n";
    }
    std::cout << "Saved " << employees.size() << " employees\n";
}

// EXPENSES
//Data structures and file I/O for customers, appointments, quotes, invoices, employees, and expenses
void dataManager::loadExpenses(const std::string& filename,
                                ExpenseTracker& tracker) {
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
        double      amount;
        std::string category, date, desc;

        std::getline(ss, token, ',');            // skip id
        std::getline(ss, token, ','); amount   = std::stod(token);
        std::getline(ss, token, ','); category = token;
        std::getline(ss, token, ','); date     = token;
        std::getline(ss, token, ','); desc     = token;

        tracker.addExpense(amount, category, date, desc);
    }

    std::cout << "Loaded expenses from " << filename << "\n";
}

void dataManager::saveExpenses(const std::string& filename,
                                ExpenseTracker& tracker) {
    tracker.saveToCSV(filename);
    std::cout << "Saved expenses to " << filename << "\n";
}

//APPOINTMENTS
//Data structures and file I/O for customers, appointments, quotes, invoices, employees, and expenses
std::vector<appointment> dataManager::loadAppointments(
    const std::string& filename) {

    std::vector<appointment> appointments;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "No " << filename << " found\n";
        return appointments;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int    id, customerId;
        std::string customerName, date, time, serviceType, notes;
        bool   isCompleted;

        std::getline(ss, token, ','); id           = std::stoi(token);
        std::getline(ss, token, ','); customerId   = std::stoi(token);
        std::getline(ss, token, ','); customerName = token;
        std::getline(ss, token, ','); date         = token;
        std::getline(ss, token, ','); time         = token;
        std::getline(ss, token, ','); serviceType  = token;
        std::getline(ss, token, ','); notes        = token;
        std::getline(ss, token, ','); isCompleted  = (token == "1");

        appointment a(id, customerId, customerName,
                      date, time, serviceType, notes);
        if (isCompleted) a.markCompleted();
        appointments.push_back(a);
    }

    std::cout << "Loaded " << appointments.size() << " appointments\n";
    return appointments;
}

void dataManager::saveAppointments(const std::string& filename,
                                    std::vector<appointment>& appointments) {
    std::ofstream file(filename);
    for (auto& a : appointments) {
        file << a.getId()           << ","
             << a.getCustomerId()   << ","
             << a.getCustomerName() << ","
             << a.getDate()         << ","
             << a.getTime()         << ","
             << a.getServiceType()  << ","
             << a.getNotes()        << ","
             << (a.getIsCompleted() ? "1" : "0") << "\n";
    }
    std::cout << "Saved " << appointments.size() << " appointments\n";
}