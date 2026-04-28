/*
Author: Keiry & Brian
Class: Employee
Description: Represents an employee with their personal details, job type,
hourly rate, and hours worked. Includes methods for calculating pay,
checking full-time status, and searching the employee list.
*/
#include <iostream>
#include "Employee.h"

#include "Employee.h"
#include <iostream>

Employee::Employee(int id, std::string name, std::string jobType, double hourlyRate, std::string phone, std::string email)
    : id(id), name(name), jobType(jobType), hourlyRate(hourlyRate), hoursWorked(0), phone(phone), email(email) {}

// pay calculations

double Employee::calculatePay() const {
    return hoursWorked * hourlyRate;
}

double Employee::calculateWeeklyPay(double weeklyHours) const {
    return weeklyHours * hourlyRate;
}

bool Employee::isFullTime() const {
    return hoursWorked >= 40;
}

// JSON for the frontend part
nlohmann::json Employee::toJson() const {
    return nlohmann::json{
        {"id",          id},
        {"name",        name},
        {"jobType",     jobType},
        {"hourlyRate",  hourlyRate},
        {"hoursWorked", hoursWorked},
        {"totalPay",    calculatePay()},
        {"phone",       phone},
        {"email",       email},
        {"isFullTime",  isFullTime()}
    };
}

// linear search algorithm by ID
/*
Author: Keiry
Algorithm: Linear Search
This function searches through the employee list to find a match by ID.
Time Complexity: O(n)
*/
// Big-O: O(n) — checks each employee one by one
Employee* Employee::searchById(std::vector<Employee>& employees, int id) {
    for (auto& e : employees) {       // loop through every employee
        if (e.getId() == id) {        // check if ID matches
            return &e;                // found — return it
        }
    }
    return nullptr;                   // not found
}

// linear search algorithm by name
/*
Author: Keiry
Algorithm: Linear Search
This function searches through the employee list to find a match by Name.
Time Complexity: O(n)
If the list were to be longer, something like a hash map 
or binary search tree could be used for faster lookups bc it would have O(1) or O(log n) time complexity
*/
// Big-O: O(n) — checks each employee one by one
Employee* Employee::searchByName(std::vector<Employee>& employees,
                                  std::string name) {
    for (auto& e : employees) {       // loop through every employee
        if (e.getName() == name) {    // check if name matches
            return &e;                // found — return it
        }
    }
    return nullptr;                   // not found
}

// linear search algorithm by job type
/*
Author: Keiry
Algorithm: Linear Search (Filtering)
This function iterates through all employees and collects those matching the Job Type.
Time Complexity: O(n)
*/
// returns ALL employees with that job type
// Big-O: O(n) — must check every employee
std::vector<Employee> Employee::searchByJobType(std::vector<Employee>& employees,
                                                 std::string jobType) {
    std::vector<Employee> results;

    for (auto& e : employees) {           // loop through every employee
        if (e.getJobType() == jobType) {  // check if job type matches
            results.push_back(e);         // add to results list
        }
    }

    return results;                       // return all matches
}
