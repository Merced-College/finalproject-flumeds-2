#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "json.hpp"
//#include <nlohmann/json.hpp>

class customer {
private:
    int id;
    std::string name;
    std::string phone;
    std::string email;
    std::string address;

public:
    customer(int id, std::string name, std::string phone, std::string email, std::string address);

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getAddress() const { return address; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }

    // This allows your C++ backend to send a list of clients to your HTML/JS frontend
    nlohmann::json toJson() const;
};

#endif