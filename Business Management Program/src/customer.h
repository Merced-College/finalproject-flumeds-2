/*
Author: Keiry & Brian
Class: customer
Description: Represents a business customer with their contact information.
*/
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "json.hpp"
//#include <nlohmann/json.hpp>

class customer {
//Priv classes to store customer info, and public functions to access that info 
private:
    int id;
    std::string name;
    std::string phone;
    std::string email;
    std::string address;

public:
//constructors
    customer(int id, std::string name, std::string phone, std::string email, std::string address);

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getAddress() const { return address; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }


    //Used AI to help us with the library
    // allows C++ backend to send a list of clients to HTML/JS frontend
    nlohmann::json toJson() const;
};

#endif