/*
Author: Keiry & Brian
Class: Employee
Description: 
*/

#pragma once
#include <string>
#include <vector>
#include "json.hpp"

class Employee {
private:
    int id;
    std::string name;
    std::string jobType;
    double hourlyRate;
    double hoursWorked;
    std::string phone;
    std::string email;

public:
    // constructor
    Employee(int id, std::string name, std::string jobType,
             double hourlyRate, std::string phone, std::string email);

    // getters
    int getId() const { return id; }
    std::string getName()const { return name; }
    std::string getJobType()    const { return jobType; }
    double getHourlyRate() const { return hourlyRate; }
    double getHoursWorked() const { return hoursWorked; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }

    // setters
    void setHoursWorked(double hours) { hoursWorked = hours; }
    void setHourlyRate(double rate)   { hourlyRate  = rate; }
    void addHours(double hours)       { hoursWorked += hours; }

    // functions
    double         calculatePay()    const;  // hoursWorked x hourlyRate
    double         calculateWeeklyPay(double weeklyHours) const;
    bool           isFullTime()      const;  // works 40+ hrs
    nlohmann::json toJson()          const;

    // linear search algorithms
    static Employee* searchById(std::vector<Employee>& employees, int id);
    static Employee* searchByName(std::vector<Employee>& employees, std::string name);
    static std::vector<Employee> searchByJobType(std::vector<Employee>& employees, std::string jobType);
};

