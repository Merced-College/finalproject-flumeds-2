/*
Author: Keiry & Brian
Class: customer
Description: Implementation for the customer class.
*/
#include "customer.h"
#include <fstream>
#include <sstream>
#include <iostream>


//Constructors for the customer class
customer::customer(int id, std::string name, std::string phone, std::string email, std::string address)
    : id(id), name(name), phone(phone), email(email), address(address) {}


//Used AI to help us with the library
nlohmann::json customer::toJson() const {
    return nlohmann::json{
        {"id", id},
        {"name", name},
        {"phone", phone},
        {"email", email},
        {"address", address}
    };
}