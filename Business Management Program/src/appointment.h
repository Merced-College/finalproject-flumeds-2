/*
Author: Keiry & Brian
Class: appointment
Description: Represents a scheduled appointment for a customer, including
date, time, service type, and completion status.
*/
#pragma once
#include <string>
#include <vector>
#include "json.hpp"

class appointment {
private:
    int id;
    int customerId;
    std::string customerName;
    std::string date;        // format: "YYYY-MM-DD"
    std::string time;        // format: "HH:MM"
    std::string serviceType;
    std::string notes;
    bool isCompleted;

public:
    // constructor
    appointment(int id, int customerId, std::string customerName, std::string date, std::string time, std::string serviceType, std::string notes = "");
    
    // getters
    int getId() const { return id; }
    int getCustomerId() const { return customerId; }
    std::string getCustomerName() const { return customerName; }
    std::string getDate() const { return date; }
    std::string getTime() const { return time; }
    std::string getServiceType() const { return serviceType; }
    std::string getNotes() const { return notes; }
    bool getIsCompleted() const { return isCompleted; }

    // setters
    void markCompleted()                     { isCompleted = true; }
    void setNotes(std::string n)             { notes = n; }
    void setDate(std::string d)              { date = d; }
    void setTime(std::string t)              { time = t; }

    // conflict detection algorithm 
    // checks if this appointment conflicts with another
    bool conflictsWith(const appointment& other) const;

    // static search functions
    //Vector Data Structure: O(n) for search, but simple and efficient for small datasets
    static std::vector<appointment> getByDate(std::vector<appointment>& appointments, std::string date);
    static std::vector<appointment> getByCustomer(std::vector<appointment>& appointments, int customerId);
    //ADDED
    static std::vector<appointment> getQueue(std::vector<appointment>& appointments);
    static bool hasConflict(std::vector<appointment>& appointments, std::string date, std::string time);

    //Used AI to help us with the library
    nlohmann::json toJson() const;
};