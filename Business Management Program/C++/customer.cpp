#include "customer.h"
#include <fstream>
#include <sstream>
#include <iostream>


customer::customer(int id, std::string name, std::string phone, std::string email, std::string address)
    : id(id), name(name), phone(phone), email(email), address(address) {}

nlohmann::json customer::toJson() const {
    return nlohmann::json{
        {"id", id},
        {"name", name},
        {"phone", phone},
        {"email", email},
        {"address", address}
    };
}