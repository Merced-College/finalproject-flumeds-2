#include "httplib.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "profitCalculator.h"
#include "customer.h"
#include "quote.h"
#include "ExpenseTracker.h"
#include "Expense.h"
#include "invoices.h"
#include "Employee.h" 

using json = nlohmann::json;

//Globals
ExpenseTracker expTracker;



// HI hi again
std::vector<customer> customers;
std::vector<quote>    quotes;
std::vector<invoices> invoiceList;
std::vector<Employee> employees;
int nextCustomerId = 1;
int nextQuoteId    = 1;
int nextInvoiceId = 1;
int nextEmployeeId = 1;

/* Add near top with other globals
employeeManager empManager;
 invoiceGenerator invGenerator;
dataManager dm;
ExpenseTracker expTracker;
*/

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void loadCustomersFromCSV() {
    std::ifstream file("customer.csv");
    if (!file.is_open()) {
        std::cout << "No customer.csv found\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int id;
        std::string name, phone, email, address;
        std::getline(ss, token, ','); id      = std::stoi(token);
        std::getline(ss, token, ','); name    = token;
        std::getline(ss, token, ','); phone   = token;
        std::getline(ss, token, ','); email   = token;
        std::getline(ss, token, ','); address = token;
        customers.push_back(customer(id, name, phone, email, address));
        if (id >= nextCustomerId) nextCustomerId = id + 1;
    }
    std::cout << "Loaded " << customers.size() << " customers\n";
}


void saveCustomersToCSV() {
    std::ofstream file("customer.csv");
    for (auto& c : customers) {
        file << c.getId()      << ","
             << c.getName()    << ","
             << c.getPhone()   << ","
             << c.getEmail()   << ","
             << c.getAddress() << "\n";
    }
}

void loadInvoicesFromCSV() {
    std::ifstream file("invoices.csv");
    if (!file.is_open()) {
        std::cout << "No invoices.csv found\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int id, cId;
        std::string name, date;
        double amount;
        bool paid;

        std::getline(ss, token, ','); id = std::stoi(token);
        std::getline(ss, token, ','); cId = std::stoi(token);
        std::getline(ss, token, ','); name = token;
        std::getline(ss, token, ','); date = token;
        std::getline(ss, token, ','); amount = std::stod(token);
        std::getline(ss, token, ','); paid = (token == "1");

        invoiceList.push_back(invoices(id, cId, name, date, amount, paid));
        if (id >= nextInvoiceId) nextInvoiceId = id + 1;
    }
    std::cout << "Loaded " << invoiceList.size() << " invoices\n";
}

void saveInvoicesToCSV() {
    std::ofstream file("invoices.csv");
    for (const auto& inv : invoiceList) {
        file << inv.id << ","
             << inv.customerId << ","
             << inv.customerName << ","
             << inv.date << ","
             << inv.amount << ","
             << (inv.isPaid ? "1" : "0") << "\n";
    }
}

// ── employee CSV functions ────────────────────────────
void loadEmployeesFromCSV() {
    std::ifstream file("employees.csv");
    if (!file.is_open()) {
        std::cout << "No employees.csv found\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int    id;
        std::string name, jobType, phone, email;
        double hourlyRate, hoursWorked;
        std::getline(ss, token, ','); id          = std::stoi(token);
        std::getline(ss, token, ','); name        = token;
        std::getline(ss, token, ','); jobType     = token;
        std::getline(ss, token, ','); hourlyRate  = std::stod(token);
        std::getline(ss, token, ','); hoursWorked = std::stod(token);
        std::getline(ss, token, ','); phone       = token;
        std::getline(ss, token, ','); email       = token;
        Employee e(id, name, jobType, hourlyRate, phone, email);
        e.setHoursWorked(hoursWorked);
        employees.push_back(e);
        if (id >= nextEmployeeId) nextEmployeeId = id + 1;
    }
    std::cout << "Loaded " << employees.size() << " employees\n";
}

void saveEmployeesToCSV() {
    std::ofstream file("employees.csv");
    for (auto& e : employees) {
        file << e.getId()          << ","
             << e.getName()        << ","
             << e.getJobType()     << ","
             << e.getHourlyRate()  << ","
             << e.getHoursWorked() << ","
             << e.getPhone()       << ","
             << e.getEmail()       << "\n";
    }
}

int main() {
    httplib::Server svr;

    loadCustomersFromCSV();
    expTracker.loadFromCSV("expenses.csv");
    loadInvoicesFromCSV();
    loadEmployeesFromCSV();

    svr.set_default_headers({
        {"Access-Control-Allow-Origin",  "*"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    // ── serve pages ───────────────────────────────
    svr.Get("/", [](auto& req, auto& res) {
        res.set_content(readFile("index.html"), "text/html");
    });

  /*  svr.Get("/profit", [](auto& req, auto& res) {
        res.set_content(readFile("profit.html"), "text/html");
    });

    svr.Get("/quotes", [](auto& req, auto& res) {
        res.set_content(readFile("quotes.html"), "text/html");
    });

    // ── serve expense page ────────────────────────
    svr.Get("/expenses", [](auto& req, auto& res) {
        res.set_content(readFile("index.html"), "text/html");
    }); 
  */
    // ── serve CSS and JS ──────────────────────────
    svr.Get("/style.css", [](auto& req, auto& res) {
        res.set_content(readFile("style.css"), "text/css");
    });

    /* svr.Get("/app.js", [](auto& req, auto& res) {
        res.set_content(readFile("app.js"), "text/javascript");
    });

    svr.Get("/profit.js", [](auto& req, auto& res) {
        res.set_content(readFile("profit.js"), "text/javascript");
    });

    svr.Get("/quotes.js", [](auto& req, auto& res) {
        res.set_content(readFile("quotes.js"), "text/javascript");
    }); */
    

    svr.Get("/api/expenses", [&](auto& req, auto& res) {
        json result = json::array();
        for (auto& e : expTracker.getAllExpenses()) {
            result.push_back(e.toJson());
        }
        res.set_content(result.dump(), "application/json");
    });

    // ── customers ─────────────────────────────────
    svr.Get("/api/customers", [&](auto& req, auto& res) {
    json result = json::array();
    for (auto& c : customers) {
        result.push_back(c.toJson());
    }
    res.set_content(result.dump(), "application/json");
    });

    svr.Post("/api/customers", [&](auto& req, auto& res) {
        auto body = json::parse(req.body);
        customer c(
            nextCustomerId++,
            body["name"],
            body["phone"],
            body["email"],
            body["address"]
        );
        customers.push_back(c);
        saveCustomersToCSV();
        res.set_content("{\"success\": true}", "application/json");
    });

    svr.Delete("/api/customers/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        customers.erase(
            std::remove_if(customers.begin(), customers.end(),
                [id](customer& c) { return c.getId() == id; }),
            customers.end()
        );
        saveCustomersToCSV();
        res.set_content("{\"success\": true}", "application/json");
    });

    // ── profit calculator ─────────────────────────
    svr.Post("/api/profit", [](auto& req, auto& res) {
        auto body = json::parse(req.body);
        profitCalculator calc;
        calc.amountCharged = body["amountCharged"];
        calc.supplyCost    = body["supplyCost"];
        calc.laborHours    = body["laborHours"];
        calc.laborRate     = body["laborRate"];

        json result;
        result["laborCost"]     = calc.calcLaborCost();
        result["profit"]        = calc.calculateProfit();
        result["wasProfitable"] = calc.wasProfitable();

        res.set_content(result.dump(), "application/json");
    });

    // ── quotes ────────────────────────────────────
    svr.Get("/api/quotes", [&](auto& req, auto& res) {
        json result = json::array();
        for (auto& q : quotes) {
            result.push_back({
                {"id",              q.id},
                {"customerName",    q.customerName},
                {"serviceType",     q.serviceType},
                {"vehicleDesc",     q.vehicleDesc},
                {"date",            q.date},
                {"amountCharged",   q.amountCharged},
                {"estimatedProfit", q.estimatedProfit()},
                {"notes",           q.notes}
            });
        }
        res.set_content(result.dump(), "application/json");
    });

    svr.Post("/api/quotes", [&](auto& req, auto& res) {
        auto body = json::parse(req.body);
        quote q;
        q.id            = nextQuoteId++;
        q.customerName  = body["customerName"];
        q.customerPhone = body["customerPhone"];
        q.serviceType   = body["serviceType"];
        q.vehicleDesc   = body["vehicleDesc"];
        q.date          = body["date"];
        q.supplyCost    = body["supplyCost"];
        q.laborHours    = body["laborHours"];
        q.laborRate     = body["laborRate"];
        q.amountCharged = body["amountCharged"];
        q.notes         = body["notes"];
        quotes.push_back(q);
        json result;
        result["success"]         = true;
        result["id"]              = q.id;
        result["estimatedProfit"] = q.estimatedProfit();
        res.set_content(result.dump(), "application/json");
    });
    // ── Get All Invoices ───────────────────────────
    svr.Get("/api/invoices", [&](auto& req, auto& res) {
        json result = json::array();
        for (auto& inv : invoiceList) {
            result.push_back(inv.toJson());
        }
        res.set_content(result.dump(), "application/json");
    });

    // ── Create New Invoice ─────────────────────────
    svr.Post("/api/invoices", [&](auto& req, auto& res) {
        auto body = json::parse(req.body);
        
        invoices newInv(
            nextInvoiceId++,
            body["customerId"],
            body["customerName"],
            body["date"],
            body["amount"],
            body.value("isPaid", false) // Defaults to false if not provided
        );
        
        invoiceList.push_back(newInv);
        saveInvoicesToCSV();
        
        res.set_content("{\"success\": true}", "application/json");
    });

    // ── Mark Invoice as Paid ───────────────────────
    svr.Put("/api/invoices/pay/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        for (auto& inv : invoiceList) {
            if (inv.id == id) {
                inv.isPaid = true;
                saveInvoicesToCSV();
                return res.set_content("{\"success\": true}", "application/json");
            }
        }
        res.status = 404;
        res.set_content("{\"error\": \"Invoice not found\"}", "application/json");
    });

    // ── GET total by category ─────────────────────
    svr.Get("/api/expenses/category/(\\w+)", [&](auto& req, auto& res) {
        std::string category = req.matches[1];
        json result;
        result["category"] = category;
        result["total"]    = expTracker.getTotalByCategory(category);
        res.set_content(result.dump(), "application/json");
    });

// ── GET total by month ────────────────────────
    svr.Get("/api/expenses/month/(\\d{4}-\\d{2})", [&](auto& req, auto& res) {
        std::string yearMonth = req.matches[1];
        json result;
        result["month"] = yearMonth;
        result["total"] = expTracker.getTotalExpensesByMonth(yearMonth);
        res.set_content(result.dump(), "application/json");
    });

    // ── POST add expense ──────────────────────────
    svr.Post("/api/expenses", [&](auto& req, auto& res) {
        auto body = json::parse(req.body);

        expTracker.addExpense(
            body["amount"],
            body["category"],
            body["date"],
            body["description"]
        );

        expTracker.saveToCSV("expenses.csv");

        res.set_content("{\"success\": true}", "application/json");
    });
    
    // ── employees ─────────────────────────────────
    svr.Get("/api/employees", [&](auto& req, auto& res) {
        json result = json::array();
        for (auto& e : employees)
            result.push_back(e.toJson());
        res.set_content(result.dump(), "application/json");
    });

    svr.Post("/api/employees", [&](auto& req, auto& res) {
        auto body = json::parse(req.body);
        Employee e(nextEmployeeId++, body["name"], body["jobType"],
                   body["hourlyRate"], body["phone"], body["email"]);
        employees.push_back(e);
        saveEmployeesToCSV();
        res.set_content("{\"success\": true}", "application/json");
    });

    svr.Delete("/api/employees/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        employees.erase(
            std::remove_if(employees.begin(), employees.end(),
                [id](Employee& e) { return e.getId() == id; }),
            employees.end());
        saveEmployeesToCSV();
        res.set_content("{\"success\": true}", "application/json");
    });

    // ── employee search by name ───────────────────
    svr.Get("/api/employees/search/(.*)", [&](auto& req, auto& res) {
        std::string query = req.matches[1];
        Employee* found = Employee::searchByName(employees, query);
        if (found) {
            res.set_content(found->toJson().dump(), "application/json");
        } else {
            res.set_content("{\"found\": false}", "application/json");
        }
    });

    // ── update employee hours ─────────────────────
    svr.Put("/api/employees/hours/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        auto body = json::parse(req.body);
        for (auto& e : employees) {
            if (e.getId() == id) {
                e.addHours(body["hours"]);
                saveEmployeesToCSV();
                json result;
                result["success"]    = true;
                result["totalPay"]   = e.calculatePay();
                result["hoursWorked"]= e.getHoursWorked();
                return res.set_content(result.dump(), "application/json");
            }
        }
        res.status = 404;
        res.set_content("{\"error\": \"Employee not found\"}", "application/json");
    });


    std::cout << "Server running at http://localhost:8080\n";
    svr.listen("localhost", 8080);
    return 0;
}
