#

## Description
This is a C++ application designed to help an auto detailing business manage its day-to-day operations. It provides a complete backend system to handle customers, employees, quotes, invoices, appointments, and business expenses. 

## Features
- **Customer Management**: Add, search, and delete customer information.
- **Quote Generation**: Create quotes for services including supply and labor costs, and estimate profit.
- **Invoice Tracking**: Keep track of paid and unpaid invoices for completed jobs.
- **Employee Management**: Manage staff details, job types, hourly rates, and calculate weekly pay.
- **Expense Tracking**: Track business expenses and calculate grouped analytics by category or month.
- **Appointment Scheduling**: Book appointments and check for time slot conflicts.
- **Profit Calculator**: A built-in calculator to evaluate if a job will be profitable based on labor and supply costs.
- **Web Frontend Integration**: Uses `httplib` to serve a frontend web interface and provide a REST API for the frontend to interact with the C++ backend.

## How to Compile and Run
This project uses the `cpp-httplib` library and requires a compiler that supports C++14 or higher (C++17 recommended).

To compile the project from the root directory:
```bash
cd src
g++ -std=c++17 -o businessapp main.cpp dataManager.cpp Employee.cpp Expense.cpp ExpenseTracker.cpp appointment.cpp customer.cpp invoices.cpp profitCalculator.cpp quote.cpp
```

To run the compiled program:
```bash
./businessapp
```
Once the server is running, open your web browser and navigate to `http://localhost:8080` to access the application interface.

## Data Structures Used
1. **`std::vector`**: Used extensively throughout the application to store lists of entities (e.g., `vector<customer>`, `vector<quote>`, `vector<Employee>`).
2. **`std::string`**: Used extensively for storing textual data like names, emails, and phone numbers.
3. **`std::map`**: Used in the `ExpenseTracker` class to automatically group and sum up expenses by category.
4. **`std::queue<Type>`** Used in `Appointment` class to build a queue of appointments on the same date ordered by time to maintain schedule.

## Algorithms Used
1. **Linear Search (O(N))**: Used in `Employee.cpp` to search for an employee by ID, Name, or Job Type. It iterates through the vector sequentially until a match is found.
2. **Selection Sort (O(N^2))**: Used in `quote.cpp` (`sortQuotesByProfit`) to sort the quotes vector by estimated profit in descending order.
3. **Calculation Algorithm (O(1))**: Used in `profitCalculator.cpp` to determine the net profit using a constant-time calculation algorithm: `Charged - Supplies - Labor`.

## Contributors
- Keiry & Brian
