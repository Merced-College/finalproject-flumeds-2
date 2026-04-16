#include "Expense.h"
#include <stdexcept>

Expense::Expense(int id, double amount, std::string cat, std::string date, std::string desc)
    : id(id), amount(amount), category(cat), date(date), description(desc) {
    
    if (amount < 0) {
        this->amount = 0; 
    }
}

// was suggested to add this function to extract the year and month from the date string for easier grouping in the UI.
// Add this definition here
std::string Expense::getYearMonth() const {
    if (date.length() >= 7) {
        return date.substr(0, 7); // Extracts "YYYY-MM"
    }
    return date;
}

nlohmann::json Expense::toJson() const {
    return nlohmann::json{
        {"id", id},
        {"amount", amount},
        {"category", category},
        {"date", date},
        {"description", description},
        {"yearMonth", getYearMonth()} 
    };
}