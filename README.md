Wallet (Incomes and Expenses) 💳
A full-fledged finance management software developed in C to help users track, calculate, and analyze their financial activities. This program provides a structured solution for individual fund management by organizing all transactions into a single database to facilitate easy analysis of spending habits and financial targets.

🚀 Features
Comprehensive Data Storage: Tracks income and expenditure with details including date (YYYY/MM/DD), type, category, and value.

Multi-Currency Support: Manages finances in various currencies, such as HUF and EUR.

Advanced Database Operations: Supports loading existing records into memory, adding new entries, editing previous inputs, and deleting obsolete records.

Financial Analysis: Provides a statement of the user's total balance and identifies the most expensive spending category.

Time Period Filtering: Allows users to set specific date ranges to view statistics, including total spent and highest expenditure per category within that timeframe.

Custom Categories: Users can expand the system by adding new categories that align with their specific financial behaviors.

🛠 Technical Implementation
This project was built with a focus on modularity and efficient memory management.

Data Structures
The program utilizes two primary struct types to organize data:

Transaction: Stores details for a single entry (date, type, category, amount).

Wallet: Manages the collection of transactions, tracking capacity and unique categories.

Memory Management

Dynamic Reallocation: To ensure scalability without wasting memory, the transactions array dynamically resizes using realloc as the database grows.

Dynamic Strings: Categories are stored as dynamically allocated strings within a categories array.

Memory Safety: A dedicated freeWallet function is implemented to deallocate memory for both transactions and category strings, preventing memory leaks.

Logic

Date Filtering: Transactions within a specific range are identified by comparing strings lexicographically, made possible by the YYYY/MM/DD storage format.

Duplicate Prevention: Category existence is verified using strcmp to ensure no duplicate categories are created.

💻 Getting Started
Prerequisites
A standard C compiler (e.g., GCC).

Compilation & Execution
Compile the program:

Bash
gcc Wallet.c -o Wallet
Run the application:

Bash
./Wallet
📖 Usage Guide
The program operates via a command-line menu with the following options:


Load Database: Reads transaction data from transactions.txt.

Save Database: Writes all current records in memory back to transactions.txt.

Add Entry: Create a new record. Dates must follow the YYYY/MM/DD format.

Edit/Delete Entry: Modify or remove existing records using their transaction index.

Display Totals: View total income, expenses, current balance, and the highest expenditure category.

Display Statistics: View income and expenses filtered by a user-defined date range.

List Categories: See all unique categories currently in use.

👨‍💻 Author
Name: Ali Efe Kaçal

Role: University Student

GitHub: aliefekacal

This project was developed for academic purposes as part of a C programming course.
