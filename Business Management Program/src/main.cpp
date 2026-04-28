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
#include "appointment.h"
#include "dataManager.h"          

//Used AI to help us with the library
using json = nlohmann::json;

//Globals allow data to be accessed and modified from any part of the application 
dataManager      dm;               
ExpenseTracker   expTracker;

std::vector<customer>    customers;
std::vector<quote>       quotes;
std::vector<invoices>    invoiceList;
std::vector<Employee>    employees;
std::vector<appointment> appointments;

// keep track of the next ID to assign for each type of data, giving every new object a unique ID
int nextCustomerId    = 1;
int nextQuoteId       = 1;
int nextInvoiceId     = 1;
int nextEmployeeId    = 1;
int nextAppointmentId = 1;

//helps read files and serves them to the web server
std::string readFile(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

//set next IDs from loaded data
void setNextIds() {
    for (auto& c: customers)
        if (c.getId() >= nextCustomerId)
            nextCustomerId = c.getId() + 1;

    for (auto& q: quotes)
        if (q.id >= nextQuoteId)
            nextQuoteId = q.id + 1;

    for (auto& inv:  invoiceList)
        if (inv.id >= nextInvoiceId)
            nextInvoiceId = inv.id + 1;

    for (auto& e: employees)
        if (e.getId() >= nextEmployeeId)
            nextEmployeeId = e.getId() + 1;

    for (auto& a: appointments)
        if (a.getId() >= nextAppointmentId)
            nextAppointmentId = a.getId() + 1;
}

int main() {
    httplib::Server svr;

    //load all data on startup from CSV files using dataManager
    customers    = dm.loadCustomers("customer.csv");
    quotes       = dm.loadQuotes("quote.csv");
    invoiceList  = dm.loadInvoices("invoices.csv");
    employees    = dm.loadEmployees("employees.csv");
    appointments = dm.loadAppointments("appointments.csv");
    dm.loadExpenses("expenses.csv", expTracker);

    // set correct next IDs
    setNextIds();

    svr.set_default_headers({
        {"Access-Control-Allow-Origin",  "*"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    //serve pages //Maps URL paths (e.g., /customers) to physical files in the ../FrontEnd folder
    //Index2 page
    svr.Get("/", [](auto& req, auto& res) {
        res.set_content(readFile("../FrontEnd/index2.html"), "text/html");
    });

    svr.Get("/index2.html", [](auto& req, auto& res) {
        res.set_content(readFile("../FrontEnd/index2.html"), "text/html");
    });

    //Employees page
    svr.Get("/employees.html", [](auto& req, auto& res) {
        res.set_content(readFile("../FrontEnd/employees.html"), "text/html");
    });

    svr.Get("/employees", [](auto& req, auto& res) {
        res.set_content(readFile("../FrontEnd/employees.html"), "text/html");
    });

    //Customers page
    svr.Get("/customers", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/customers.html"), "text/html");
    });

    svr.Get("/customers.html", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/customers.html"), "text/html");
    });

    svr.Get("/customers.css", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/customers.css"), "text/css");
    });

     svr.Get("/customers.js", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/customers.js"), "text/javascript");
    });

    //Appointment page
    svr.Get("/appointment", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/appointment.html"), "text/html");
    });

    svr.Get("/appointment.html", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/appointment.html"), "text/html");
    });

    svr.Get("/appointment.css", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/appointment.css"), "text/css");
    });

    svr.Get("/appointment.js", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/appointment.js"), "text/javascript");
    });

    //Invoices page
     svr.Get("/invoices", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/invoices.html"), "text/html");
    });

    svr.Get("/invoices.html", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/invoices.html"), "text/html");
    });

    svr.Get("/invoices.css", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/invoices.css"), "text/css");
    });

    svr.Get("/invoices.js", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/invoices.js"), "text/javascript");
    });

    //Quote Page
    svr.Get("/quote", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/quote.html"), "text/html");
    });

    svr.Get("/quote.html", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/quote.html"), "text/html");
    });

    svr.Get("/quote.css", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/quote.css"), "text/css");
    });

    svr.Get("/quote.js", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/quote.js"), "text/javascript");
    });

    //Expense Page
     svr.Get("/expense", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/expense.html"), "text/html");
    });

    svr.Get("/expense.html", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/expense.html"), "text/html");
    });

    svr.Get("/expense.css", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/expense.css"), "text/css");
    });

    svr.Get("/expense.js", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/expense.js"), "text/javascript");
    });

    //profitCalculator Page
    svr.Get("/profitCalculator", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/profitCalculator.html"), "text/html");
    });

    svr.Get("/profitCalculator.html", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/profitCalculator.html"), "text/html");
    });

    svr.Get("/profitCalculator.css", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/profitCalculator.css"), "text/css");
    });

    svr.Get("/profitCalculator.js", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/profitCalculator.js"), "text/javascript");
    });


    //

    svr.Get("/style.css", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/style.css"), "text/css");
    });

    svr.Get("/script.js", [](auto& req, auto& res) {
    res.set_content(readFile("../FrontEnd/script.js"), "text/javascript");
    });

    /*
    This is a expense route that handles all expenses info when the frontend asks
    svr Get, regusters a GET route at the given url, when javascript fetches the url it starts running. Its able to
    access global variables and request & send
    json creates a empty array to store all expenses 
    The for loop, goes through all expenses in tracker, allExpenses() returns list of expenses, e.toJson() converts
    all expense objects into a JSON format
    res sends the JSON array back into the browser
    */    
    // expenses
    svr.Get("/api/expenses", [&](auto& req, auto& res) {   //svr.Get 
        json result = json::array();
        for (auto& e : expTracker.getAllExpenses())
            result.push_back(e.toJson());
        res.set_content(result.dump(), "application/json");
    });

    /*
    This post route handles when the frontend adds expenses
    svr.Post registers a post route in the url, whuch runs when it fetches expenses
    auto body reads data sent from browser
    exp Tracker, pulls all the fields out of the json when its sent to the frontend, calls addExpense onto expense
    Tracker object. Which adds expenses to memory list
    dm saves all expenses to CSV file 
    res.set sends successful responses back to browser
    */
    svr.Post("/api/expenses", [&](auto& req, auto& res) {
        auto body = json::parse(req.body);
        expTracker.addExpense(
            body["amount"],
            body["category"],
            body["date"],
            body["description"]
        );
        dm.saveExpenses("expenses.csv", expTracker);  // ← dataManager
        res.set_content("{\"success\": true}", "application/json");
    });

    /*
    This route handles frontend requests when asked for specific catergories 
    svr.Get registers a route in the given url
    std::string captures whatever is in the url while req.matches gets the 1st captured group
    json result creates a json object, which stores the category name / total
    getTotalByCategory adds up all the expenses in the given category
    res.set sends the JSON result back to the browser
    */
    svr.Get("/api/expenses/category/(\\w+)", [&](auto& req, auto& res) {
        std::string category = req.matches[1];
        json result;
        result["category"] = category;
        result["total"]    = expTracker.getTotalByCategory(category);
        res.set_content(result.dump(), "application/json");
    });

    /*
    Same as the catery route but instead captures the year and month
    */
    svr.Get("/api/expenses/month/(\\d{4}-\\d{2})", [&](auto& req, auto& res) {
        std::string yearMonth = req.matches[1];
        json result;
        result["month"] = yearMonth;
        result["total"] = expTracker.getTotalExpensesByMonth(yearMonth);
        res.set_content(result.dump(), "application/json");
    });

    /*
    This route handles frontend requests when asked for all customers
    svr.Get registers a GET route at the url which runs when JS does `fetch('/api/customers')`
    json result creates an empty JSON array [] to hold all customers
    for (auto& c : customers) loops through every customer in customers vector and 
    c.toJson() converts each one to JSON with their given info
    result.push adds each converted customer into the result array
    res.set sends the full array back to the browser as a JSON response
    */
    // customers
    svr.Get("/api/customers", [&](auto& req, auto& res) {
        json result = json::array();
        for (auto& c : customers)
            result.push_back(c.toJson());
        res.set_content(result.dump(), "application/json");
    });

    /*
    This route handles frontend requests when adding customers(new)
    svr.Post registers a post route in the the given url
    auto body = json reads and parse the data sent from the browser to be able to access
    customer c creates a new customer object using data from the browser
    nextCustomerId++ gives the new customer a inquique id and increments for the next customer
    customer.push adds the newly created customer to customers vector(in the memory) 
    dm.save saves all customer info into the CSV file 
    res.set sends a a successful response back to the browser. So this way JS knows customers was successfully added
    */
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
        dm.saveCustomers("customer.csv", customers);  // ← dataManager
        res.set_content("{\"success\": true}", "application/json");
    });

    /*
    This route handles the frontend requests, when deleting specific custoemrs
    svr.Delete registers a delete route in the given url
    int id converts the number from the url and converts it from string to integer 
    std::remove scans through all customers in the vector and marks customers whose getId() matches the id removal one
    [id][customer] is a lambda function which runs for each customer and returns true if customer ID matches. It tells remove_if to remove it
    customers.erase removes the marked customers froom the vector, it physicallt removes them while remoive_if marks them
    dm.save saves the updated customer list to CSV file
    res.set sends a successful response to js, so frontend 
    */
    svr.Delete("/api/customers/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        customers.erase(
            std::remove_if(customers.begin(), customers.end(),
                [id](customer& c) { return c.getId() == id; }),
            customers.end());
        dm.saveCustomers("customer.csv", customers);  // ← dataManager
        res.set_content("{\"success\": true}", "application/json");
    });
    
    /*
    svr.Get registers a GET route in the url 
    std::string query captures whatever the user typed in the box url
    json result makes a empty JSON arry to hold the matching customers
    for (auto&c ) loops through all the customers in the vector to see if they match the seacrh query 
    c.getName() checks if the customers name has the search query and returns npos if nothing was found
    c.getPhone() does the same as getName but for phone
    result.push adds the matching customer to the result array
    res.set sends only the matching customers to frontend so browser can display it
    */
    svr.Get("/api/customers/search/(.*)", [&](auto& req, auto& res) {
        std::string query = req.matches[1];
        json result = json::array();
        for (auto& c : customers) {
            if (c.getName().find(query)  != std::string::npos ||
                c.getPhone().find(query) != std::string::npos) {
                result.push_back(c.toJson());
            }
        }
        res.set_content(result.dump(), "application/json");
    });

    /*
    This route handles the frontend requests when calculating profit for a job
    svr.Post regiseters a post route at the given url
    auto body reads and parses data sent from browser so fields like body["amountCharged"] and body["supplyCost"] can be accessed
    profitCalculator creates a object which mirros profitCalculator.h 
    calc.amountCharged and the 3 lines below it, set the four values onto the calclator object directly (amountCharged, supplyCost, laborHours)
    calc.wasProfitable() returns true or false which depenonds on the net profit being greater than 0
    json Result, builds the responses object with all values calculated so it can be sent to browser. That was the js can be displayed
    on screen
    */
    // profit calculator
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

    /*
    This route handles frontend requests, when asked for all quotes
    json result creates an empty JSON array to hold all quotes
    for(auto& q) loops through all quotes in the vector(quotes)
    result.push builds the JSON object field by field. Gives full control over what is sent to the frontend(browser)
    All the fields inside result.push are pulled directly from the quote object 
    q.estimatedProfit() calls methods from the quote class(quote class calculates amount charge - supply cost - (labor hours * rate)) so the 
    frontend doesnt calculate it 
    res.set sends the full array of quotes into the browser, that was Js can display them on the table
    */
    // quotes
    svr.Get("/api/quotes", [&](auto& req, auto& res) {
        json result = json::array();
        for (auto& q : quotes) {
            result.push_back({
                {"id",              q.id},
                {"customerName",    q.customerName},
                {"customerPhone",   q.customerPhone},
                {"serviceType",     q.serviceType},
                {"vehicleDesc",     q.vehicleDesc},
                {"date",            q.date},
                {"supplyCost",      q.supplyCost},
                {"laborHours",      q.laborHours},
                {"laborRate",       q.laborRate},
                {"amountCharged",   q.amountCharged},
                {"estimatedProfit", q.estimatedProfit()},
                {"notes",           q.notes}
            });
        }
        res.set_content(result.dump(), "application/json");
    });

    /*
    This will help create a new quote by taking the data sent from the frontend
    assinging it a new ID and saving it to the CSV file.
    This also calculates the estimated profit on the backend and sends it back to the frontend 
    
    */
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
        dm.saveQuotes("quote.csv", quotes);          // ← dataManager
        json result;
        result["success"]         = true;
        result["id"]              = q.id;
        result["estimatedProfit"] = q.estimatedProfit();
        res.set_content(result.dump(), "application/json");
    });

    //invoices
    /*
     * This will iterate through the global invoice vector and convert 
     * each object to a JSON format to send to the frontend.
     */
    svr.Get("/api/invoices", [&](auto& req, auto& res) {
        json result = json::array();
        for (auto& inv : invoiceList)
            result.push_back(inv.toJson());
        res.set_content(result.dump(), "application/json");
    });
    /*
    This will create new invoices based on the data sent from the frontend 
    it creates a new invoice object with the current nextInvoiceId 
    then it saves the new invoice to the CSV file
    */
    svr.Post("/api/invoices", [&](auto& req, auto& res) {
        auto body = json::parse(req.body);
        invoices newInv(
            nextInvoiceId++,
            body["customerId"],
            body["customerName"],
            body["date"],
            body["amount"],
            body.value("isPaid", false)
        );
        invoiceList.push_back(newInv);
        dm.saveInvoices("invoices.csv", invoiceList); // ← dataManager
        res.set_content("{\"success\": true}", "application/json");
    });
    /*
     This will search for an invoice by ID to allow user 
     to mark invoice as paid and saves
     list to the CSV file
     */
    svr.Put("/api/invoices/pay/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        for (auto& inv : invoiceList) {
            if (inv.id == id) {
                inv.isPaid = true;
                dm.saveInvoices("invoices.csv", invoiceList); // ← dataManager
                return res.set_content(
                    "{\"success\": true}", "application/json");
            }
        }
        res.status = 404;
        res.set_content(
            "{\"error\": \"Invoice not found\"}", "application/json");
    });

    // employees
    /*
     * This will iterate through the global employee vector and convert 
     * each object to a JSON format to send to the frontend.
     */
    svr.Get("/api/employees", [&](auto& req, auto& res) {
        json result = json::array();
        for (auto& e : employees)
            result.push_back(e.toJson());
        res.set_content(result.dump(), "application/json");
    });
    /*
    This will recieve new employee details from the frontend
    creating a new employee ID and adding it to the list in the CSV file
    */
    svr.Post("/api/employees", [&](auto& req, auto& res) {
        auto body = json::parse(req.body);
        Employee e(
            nextEmployeeId++,
            body["name"],
            body["jobType"],
            body["hourlyRate"],
            body["phone"],
            body["email"]
        );
        employees.push_back(e);
        dm.saveEmployees("employees.csv", employees); // ← dataManager
        res.set_content("{\"success\": true}", "application/json");
    });

    /*
      Gets the ID from the URL and uses the erase function
      to remove the matching employee from the vector 
     */
    svr.Delete("/api/employees/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        employees.erase(
            std::remove_if(employees.begin(), employees.end(),
                [id](Employee& e) { return e.getId() == id; }),
            employees.end());
        dm.saveEmployees("employees.csv", employees); // ← dataManager
        res.set_content("{\"success\": true}", "application/json");
    });

    /*
     * Uses a static search helper in the Employee class to find a match 
     * by name
     */

    svr.Get("/api/employees/search/(.*)", [&](auto& req, auto& res) {
        std::string query = req.matches[1];
        Employee* found = Employee::searchByName(employees, query);
        if (found) {
            res.set_content(found->toJson().dump(), "application/json");
        } else {
            res.set_content("{\"found\": false}", "application/json");
        }
    });
    
    /*
     * Finds an employee by ID and updates their work hours. It then 
     * triggers the internal payroll calculation and returns the updated 
     * total pay and total hours worked to the frontend.
     */
    svr.Put("/api/employees/hours/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        auto body = json::parse(req.body);
        for (auto& e : employees) {
            if (e.getId() == id) {
                e.addHours(body["hours"]);
                dm.saveEmployees("employees.csv", employees); // ← dataManager
                json result;
                result["success"]     = true;
                result["totalPay"]    = e.calculatePay();
                result["hoursWorked"] = e.getHoursWorked();
                return res.set_content(result.dump(), "application/json");
            }
        }
        res.status = 404;
        res.set_content(
            "{\"error\": \"Employee not found\"}", "application/json");
    });

    //appointments
    /*
     * This will iterate through the global appointments vector and convert 
     * each object to a JSON format to send to the frontend.
     */
    svr.Get("/api/appointments", [&](auto& req, auto& res) {
        json result = json::array();
        for (auto& a : appointments)
            result.push_back(a.toJson());
        res.set_content(result.dump(), "application/json");
    });
    /*
    Gets a date from the website url and find all the appointments for that date.
     */
    svr.Get("/api/appointments/date/(.*)", [&](auto& req, auto& res) {
        std::string date = req.matches[1];
        auto filtered = appointment::getByDate(appointments, date);
        json result = json::array();
        for (auto& a : filtered)
            result.push_back(a.toJson());
        res.set_content(result.dump(), "application/json");
    });
    /*
     * This helps make new appointments and checks 
     * if the requested time slot is already taken before saving.
     */
    svr.Post("/api/appointments", [&](auto& req, auto& res) {
    auto body = json::parse(req.body);

    std::string date = body["date"];   // ← get date
    std::string time = body["time"];   // ← get time

    // conflict detection that prevents double booking at the same time or date
    if (appointment::hasConflict(appointments, date, time)) {
        res.status = 409;
        res.set_content(
            "{\"error\": \"Time slot already booked\"}",
            "application/json");
        return;
    }
    //creates new appointment object with the current ID and increments the ID for the next one
        appointment a(
        nextAppointmentId++,
        body["customerId"],
        body["customerName"],
        date,                          // ← date first 
        time,                          // ← time second 
        body["serviceType"],
        body.value("notes", "")
    );

    appointments.push_back(a);
    dm.saveAppointments("appointments.csv", appointments);

    json result;
    result["success"] = true;
    result["id"]      = a.getId();
    res.set_content(result.dump(), "application/json");
    });
    /*
    This will search for an appointment by ID, 
    if it finds it it will update the status and saves
    list to the CSV file
    */
    svr.Put("/api/appointments/complete/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        for (auto& a : appointments) {
            if (a.getId() == id) {
                a.markCompleted();
                dm.saveAppointments("appointments.csv", appointments); // ← dataManager
                return res.set_content(
                    "{\"success\": true}", "application/json");
            }
        }
        res.status = 404;
        res.set_content("{\"error\": \"Not found\"}", "application/json");
    });

    /*
    This will delete appointments based on the id sent from the frontend, 
    it uses std::remove_if to find the appointment with the matching id and remove it from the vector
    then saves the updated list to the CSV file
    svr.delete register a delete route in the url inside it 
    int id = std captures appointment id 

     */
    svr.Delete("/api/appointments/(\\d+)", [&](auto& req, auto& res) {
        int id = std::stoi(req.matches[1]);
        appointments.erase(
            std::remove_if(appointments.begin(), appointments.end(),
                [id](appointment& a) { return a.getId() == id; }),
            appointments.end());
        dm.saveAppointments("appointments.csv", appointments); // ← dataManager
        res.set_content("{\"success\": true}", "application/json");
    });
    

//Get the full queue in order
svr.Get("/api/appointments/queue", [&](auto& req, auto& res) {
    auto queue = appointment::getQueue(appointments);
    json result = json::array();
    for (auto& a : queue) result.push_back(a.toJson());
    res.set_content(result.dump(), "application/json");
});

//Get ONLY the next person in line
svr.Get("/api/appointments/next", [&](auto& req, auto& res) {
    auto queue = appointment::getQueue(appointments);
    if (queue.empty()) {
        res.set_content("{\"message\": \"No upcoming appointments\"}", "application/json");
    } else {
        res.set_content(queue[0].toJson().dump(), "application/json");
    }
});

//Complete the next person in line (The "Pop" operation)
svr.Put("/api/appointments/next/complete", [&](auto& req, auto& res) {
    auto queue = appointment::getQueue(appointments);
    if (!queue.empty()) {
        int nextId = queue[0].getId();
        for (auto& a : appointments) {
            if (a.getId() == nextId) {
                a.markCompleted();
                dm.saveAppointments("appointments.csv", appointments);
                return res.set_content("{\"success\": true}", "application/json");
            }
        }
    }
    res.status = 404;
    res.set_content("{\"error\": \"Queue empty\"}", "application/json");
});

    std::cout << "Server running at http://localhost:8080\n";
    svr.listen("localhost", 8080);
    return 0;
}
