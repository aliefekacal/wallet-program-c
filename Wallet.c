#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* This struct is the part where data of transaction stored. The date, income or expense type, category of this type,
and amounts entered by the user in the wallet program.
The transaction actions performed by the user in the wallet program are stored with the existence of this struct.*/
typedef struct {
    char date[11];
    char type[8]; /* "income" or "expense" */
    char category[20];
    double amount;
} Transaction;

/*This structure organizes any transaction data and makes it easily accessible,
allowing the user to track and manage their expenses.
usage of size_t in here is to prevent negative numbers.
**char is for opening array for string with the name of categories.*/
typedef struct {
    Transaction* transactions;
    size_t size;
    size_t capacity;
    char** categories;
    size_t categoryCount;
} Wallet;

/* This function is for the first usage of your wallet,
At the beginning of the program there should not be any data entered,
so your wallet must be empty and this function provides you with the initial version of your wallet.
You set your first version of your wallet to empty in here.*/
void initWallet(Wallet* wallet) {
    wallet->size = 0;
    wallet->capacity = 10;
    wallet->transactions = (Transaction*)malloc(wallet->capacity * sizeof(Transaction));
    wallet->categories = NULL;
    wallet->categoryCount = 0;
}

/* This function frees memory allocated for a Wallet structure,
preventing memory leaks that occur when memory is not returned to the system.
Properly deallocating resources is crucial for optimal memory usage and application performance.*/
void freeWallet(Wallet* wallet) {
    free(wallet->transactions);
    for (size_t i = 0; i < wallet->categoryCount; i++) {
        free(wallet->categories[i]);
    }
    free(wallet->categories);
}

/* This function checks if a specific category exists within a wallet's categories list.
It uses a Wallet structure and a string to search for categories,
compares them using strcmp, and returns true if a match is found.*/
bool isCategoryExists(const Wallet* wallet, const char* category) {
    for (size_t i = 0; i < wallet->categoryCount; i++) {
        if (strcmp(wallet->categories[i], category) == 0) {
            return true;
        }
    }
    return false;
}

/* This function adds a new category to a wallet's list, checking if it exists.
If not, it reallocates memory, allocates memory for the new category string,
copies it, and increments the category count. */
void addCategory(Wallet* wallet, const char* category) {
    if (!isCategoryExists(wallet, category)) {
        wallet->categories = (char**)realloc(wallet->categories, (wallet->categoryCount + 1) * sizeof(char*));
        wallet->categories[wallet->categoryCount] = (char*)malloc(strlen(category) + 1);
        strcpy(wallet->categories[wallet->categoryCount], category);
        wallet->categoryCount++;
    }
}
/*This function efficiently reads and stores transaction data from a file,
 maintains a current category list, and dynamically handles memory allocation.*/
void loadDatabase(Wallet* wallet, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        wallet->size = 0;
        while (fscanf(file, "%10s %7s %19s %lf", wallet->transactions[wallet->size].date, wallet->transactions[wallet->size].type, wallet->transactions[wallet->size].category, &wallet->transactions[wallet->size].amount) != EOF) {
            addCategory(wallet, wallet->transactions[wallet->size].category);
            wallet->size++;
            if (wallet->size >= wallet->capacity) {
                wallet->capacity *= 2;
                wallet->transactions = (Transaction*)realloc(wallet->transactions, wallet->capacity * sizeof(Transaction));
            }
        }
        fclose(file);
        printf("Database loaded successfully.\n");
    } else {
        printf("Failed to load database.\n");
    }
}
/* This function saves a Wallet structure's contents to a file,
repeating through transactions and writing their details in a structured format.
It uses the `fprintf` function to print the date, type, category, and amount of each transaction. */
void saveDatabase(const Wallet* wallet, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file) {
        for (size_t i = 0; i < wallet->size; i++) {
            fprintf(file, "%s %s %s %.2f\n", wallet->transactions[i].date, wallet->transactions[i].type, wallet->transactions[i].category, wallet->transactions[i].amount);
        }
        fclose(file);
        printf("Database saved successfully.\n");
    } else {
        printf("Failed to save database.\n");
    }
}
/*This function adds a new transaction entry to a wallet, checking and reallocating memory to accommodate more transactions.
 It copies the transaction, checks if the category exists, and prints a confirmation message.*/
void addEntry(Wallet* wallet, const char* date, const char* type, const char* category, double amount) {
    if (wallet->size >= wallet->capacity) {
        wallet->capacity *= 2;
        wallet->transactions = (Transaction*)realloc(wallet->transactions, wallet->capacity * sizeof(Transaction));
    }

    /* Add the transaction*/
    strcpy(wallet->transactions[wallet->size].date, date);
    strcpy(wallet->transactions[wallet->size].type, type);
    strcpy(wallet->transactions[wallet->size].category, category);
    wallet->transactions[wallet->size].amount = amount;
    wallet->size++;

    /* Add the category if it doesn't exist*/
    addCategory(wallet, category);
    printf("Entry added successfully.\n");
}

/* This function modifies an existing transaction entry in the wallet by checking
if the index is valid, updating the transaction, and printing a confirmation message.*/
void editEntry(Wallet* wallet, size_t index, const char* date, const char* type, const char* category, double amount) {
    if (index < wallet->size) {
        strcpy(wallet->transactions[index].date, date);
        strcpy(wallet->transactions[index].type, type);
        strcpy(wallet->transactions[index].category, category);
        wallet->transactions[index].amount = amount;
        printf("Entry edited successfully.\n");
    } else {
        printf("Invalid entry index.\n");
    }
}

/* This function removes a transaction from a wallet at a specified index,
checking if it's within the valid range, shifting transactions,
updating wallet size, and printing confirmation or error messages.*/
void deleteEntry(Wallet* wallet, size_t index) {
    if (index < wallet->size) {
        for (size_t i = index; i < wallet->size - 1; i++) {
            wallet->transactions[i] = wallet->transactions[i + 1];
        }
        wallet->size--;
        printf("Entry deleted successfully.\n");
    } else {
        printf("Invalid entry index.\n");
    }
}

/* This function in a wallet basically summarizes financial data, including income, expenses,
 balance, and category with the highest expense. It initializes variables,
 repeats through transactions, calculates the balance by subtracting expenses from income,
 and outputs the total income, expenses, balance, and most expensive category.    */
void displayTotals(const Wallet* wallet) {
    double totalIncome = 0.0;
    double totalExpenses = 0.0;
    char maxCategory[20] = "";
    double maxExpense = 0.0;

    for (size_t i = 0; i < wallet->size; i++) {
        if (strcmp(wallet->transactions[i].type, "income") == 0) {
            totalIncome += wallet->transactions[i].amount;
        } else if (strcmp(wallet->transactions[i].type, "expense") == 0) {
            totalExpenses += wallet->transactions[i].amount;
            if (wallet->transactions[i].amount > maxExpense) {
                maxExpense = wallet->transactions[i].amount;
                strcpy(maxCategory, wallet->transactions[i].category);
            }
        }
    }

    double balance = totalIncome - totalExpenses;
    printf("Total Income: %.2f\n", totalIncome);
    printf("Total Expenses: %.2f\n", totalExpenses);
    printf("Balance: %.2f\n", balance);
    printf("Most Expensive Category: %s (%.2f)\n", maxCategory, maxExpense);
}

/* This function is a wallet structure that calculates income and expenses
within a specified date range. It initializes variables, checks if transactions fall within the range,
calculates incomes and expenses, and outputs the totals.  */
void displayStats(const Wallet* wallet, const char* startDate, const char* endDate) {
    double totalIncome = 0.0;
    double totalExpenses = 0.0;

    printf("Statistics from %s to %s:\n", startDate, endDate);
    for (size_t i = 0; i < wallet->size; i++) {
        if (strcmp(wallet->transactions[i].date, startDate) >= 0 && strcmp(wallet->transactions[i].date, endDate) <= 0) {
            if (strcmp(wallet->transactions[i].type, "income") == 0) {
                totalIncome += wallet->transactions[i].amount;
            } else if (strcmp(wallet->transactions[i].type, "expense") == 0) {
                totalExpenses += wallet->transactions[i].amount;
            }
        }
    }

    printf("Total Income: %.2f\n", totalIncome);
    printf("Total Expenses: %.2f\n", totalExpenses);
}

/*This function checks for categories in a wallet, prints "Categories:" if available,
and iterates through the categories array to provide a summary of transaction organization. */
void listCategories(const Wallet* wallet) {
    if (wallet->categoryCount > 0) {
        printf("Categories:\n");
        for (size_t i = 0; i < wallet->categoryCount; i++) {
            printf("- %s\n", wallet->categories[i]);
        }
    } else {
        printf("No categories available.\n");
    }
}

/*This function creates a user-friendly menu for the wallet program,
allowing users to interact with various features, improve its usability. */
void displayMenu() {
    printf("\nWallet Program Menu:\n");
    printf("1. Load Database\n");
    printf("2. Save Database\n");
    printf("3. Add Entry\n");
    printf("4. Edit Entry\n");
    printf("5. Delete Entry\n");
    printf("6. Display Totals\n");
    printf("7. Display Statistics\n");
    printf("8. List Categories\n");
    printf("9. Exit\n");
}

int main() {
    Wallet myWallet;
    initWallet(&myWallet);
    int choice;

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        char date[11], type[8], category[20];
        double amount;
        size_t index;

        switch (choice) {
            case 1:
                loadDatabase(&myWallet, "transactions.txt");
                break;
            case 2:
                saveDatabase(&myWallet, "transactions.txt");
                break;
            case 3:
                printf("Enter date (YYYY/MM/DD): ");
                scanf("%s", date);
                printf("Enter type (income/expense): ");
                scanf("%s", type);
                printf("Enter category: ");
                scanf("%s", category);
                printf("Enter amount: ");
                scanf("%lf", &amount);
                addEntry(&myWallet, date, type, category, amount);
                break;
            case 4:
                printf("Enter index to edit: ");
                scanf("%d", &index);
                printf("Enter new date (YYYY/MM/DD): ");
                scanf("%s", date);
                printf("Enter new type (income/expense): ");
                scanf("%s", type);
                printf("Enter new category: ");
                scanf("%s", category);
                printf("Enter new amount: ");
                scanf("%lf", &amount);
                editEntry(&myWallet, index, date, type, category, amount);
                break;
            case 5:
                printf("Enter index to delete: ");
                scanf("%d", &index);
                deleteEntry(&myWallet, index);
                break;
            case 6:
                displayTotals(&myWallet);
                break;
            case 7:
                printf("Enter start date (YYYY/MM/DD): ");
                scanf("%s", date);
                char endDate[11];
                printf("Enter end date (YYYY/MM/DD): ");
                scanf("%s", endDate);
                displayStats(&myWallet, date, endDate);
                break;
            case 8:
                listCategories(&myWallet);
                break;
            case 9:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);

    freeWallet(&myWallet);
    return 0;
}
