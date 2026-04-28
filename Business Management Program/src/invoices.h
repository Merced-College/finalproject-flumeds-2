/*
Author: Keiry & Brian
Class: invoices
Description: Represents a customer invoice with payment status.
*/
#pragma once
#include <string>
#include "json.hpp"
using json = nlohmann::json;


class invoices {
public:
    int id;
    int customerId;
    std::string customerName;
    std::string date;
    std::string notes;

    double amount;
    bool isPaid;

    // Constructor
    invoices(int id, int cId, std::string name, std::string date, double amt, bool paid = false);

    // Functions
    void printInvoice();
    json toJson() const;
};