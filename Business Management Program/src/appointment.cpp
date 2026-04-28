/*
Author: Keiry & Brian
Class: appointment
Description: Implementation of the appointment class.
*/
#include "appointment.h"
#include <iostream>
#include <algorithm>

appointment::appointment(int id, int customerId, std::string customerName, std::string date, std::string time, std::string serviceType, std::string notes)
    : id(id), customerId(customerId), customerName(customerName),
      date(date), time(time), serviceType(serviceType),
      notes(notes), isCompleted(false) {}

// ── conflict detection algorithm 
// checks if two appointments are on the same date and time
// Big-O: O(1) — just compares two values
bool appointment::conflictsWith(const appointment& other) const {
    return (date == other.date && time == other.time);
}

// get all appointments for a specific date
// Big-O: O(n) — loops through all appointments
std::vector<appointment> appointment::getByDate(std::vector<appointment>& appointments,std::string date) {
    std::vector<appointment> result;

    for (auto& a : appointments) {        // loop through all
        if (a.getDate() == date) {        // check if date matches
            result.push_back(a);          // add to results
        }
    }
    return result;
}


// get all appointments for a specific customer
// Big-O: O(n) — loops through all appointments
std::vector<appointment> appointment::getByCustomer(std::vector<appointment>& appointments,int customerId) {
    std::vector<appointment> result;

    for (auto& a : appointments) {
        if (a.getCustomerId() == customerId) {
        result.push_back(a);
        }
    }
    return result;
}


// conflict detection
// checks if ANY existing appointment conflicts with new date/time
// Big-O: O(n) — must check every appointment
bool appointment::hasConflict(std::vector<appointment>& appointments, std::string date, std::string time) {
    for (auto& a : appointments) {        // loop through every appointment
        if (a.getDate() == date && a.getTime() == time && !a.getIsCompleted()) {
            return true;
        }                       
    }
    return false;                         
}

/*
// Builds a queue of pending appointments by filtering out completed ones, 
// then sorting the remaining appointments by date and time (earliest first).
// Appointments on the same date are ordered by time to maintain schedule order.
// Time complexity: O(1) because it is constantly adding and removing appointments from the queue
*/
std::vector<appointment> appointment::getQueue(std::vector<appointment>& appointments) {
    std::vector<appointment> queue;
    
    // Filter out already completed appointments
    for (auto& a : appointments) {
        if (!a.getIsCompleted()) {
            queue.push_back(a);
        }
    }

    // Sort by date then by time (First In First Out based on schedule)
    std::sort(queue.begin(), queue.end(), [](const appointment& a, const appointment& b) {
        if (a.getDate() != b.getDate()) return a.getDate() < b.getDate();
        return a.getTime() < b.getTime();
    });
    return queue;
}

//Used AI to help us with the library
// JSON for frontend
nlohmann::json appointment::toJson() const {
    return nlohmann::json{
        {"id",           id},
        {"customerId",   customerId},
        {"customerName", customerName},
        {"date",         date},
        {"time",         time},
        {"serviceType",  serviceType},
        {"notes",        notes},
        {"isCompleted",  isCompleted}
    };
}