/*
* FILE          : Customer.cpp
* PROJECT       : PROG1175 ¨C Assignment #4
* PROGRAMMER    : Xinming Xu (Simmy)
* FIRST VERSION : 2025-07-31
* DESCRIPTION   : This program manages customer records including loading, saving,
*                 displaying, searching, and editing customer information, with
*                 strict validation and logging.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Log Level
#define LOG_INFO    "INFO"
#define LOG_WARNING "WARNING"
#define LOG_ERROR   "ERROR"

#define MAX_CUSTOMERS 50        // Maximum number of customers
#define MAX_LINE_LENGTH 512     // Maximum read line length
#define MAX_INPUT_LENGTH 100    // Maximum input buffer length

// STRUCT : Customer
typedef struct {
    int customerID;         // Unique customer ID
    char name[51];          // lastName, firstName OR business name
    char address[101];
    char city[101];
    char province[3];       // Province Code / Format: ON (2-letter code) 
    char postalCode[7];     // postal code / Format: A1A1A1 
    char phone[13];         // phone Format: 123-456-7890
    char email[51];
    float creditLimit;      // >= 0.01, default 500.00
    float accountBalance;   // >= 0.00
    char joinDate[11];      // Format: YYYY-MM-DD
    char lastPayment[11];   // May be empty; same format
} Customer;

// Function prototype (log)
void logMessage(const char* level, const char* message);
void logFieldError(const char* fieldName, int line, const char* value);

// Function prototype (Menu)
void customersMenu(Customer customers[], int* count);

void listAllCustomers(Customer customers[], int count);
void searchCustomer(Customer customers[], int count);
void addCustomer(Customer customers[], int* count);
void updateCustomerInfo(Customer customers[], int count);
void listBadCreditCustomers(Customer customers[], int count);
int loadCustomers(Customer customers[], int max);
void saveCustomers(Customer customers[], int count);

// Function prototype (Valid)
int isLetter(char c);
int isDigit(char c);
int isValidProvince(const char* code);
int isValidPostalCode(const char* code);
int isValidPhone(const char* phone);
int isValidDateFormat(const char* date);

// Function prototype (listAllCustomers)
void Display_AllCustomers_Horizontal(Customer customers[], int count);
void Display_AllCustomers_Vertical(Customer customers[], int count);


int main() {

    // Dynamically allocate memory
    Customer* customers = (Customer*)malloc(sizeof(Customer) * MAX_CUSTOMERS);
    // Check for memory allocation failure
    if (customers == NULL) {
        printf("Error: Unable to allocate memory for customers.\n");
        // Write to log
        logMessage(LOG_ERROR, "Memory allocation failed for Customer array.");
        return 1;
    }

    // When starting the program, load the data from the previous day (Documentation requirements Page 8 - Question 1)
    // (1) Loading file customer data
    printf("Loading customer information ...\n");
    int customerCount = loadCustomers(customers, MAX_CUSTOMERS);
    printf("%d customers loaded.\n", customerCount);
    // (2) Loading file parts data

    // (3) Loading file orders data


    // Store user's menu choice
    int choice = 0;
    char buffer[MAX_INPUT_LENGTH];

    // Start main menu loop
    do {
        printf("\n----- PWH System Main Menu -----\n");
        printf("1. Customers\n");
        printf("2. Parts\n");
        printf("3. Orders\n");
        printf("4. Exit Program\n");
        printf("Select option: ");

        // Read input and Parse number
        if (!fgets(buffer, MAX_INPUT_LENGTH, stdin)) continue;
        if (sscanf_s(buffer, "%d", &choice) != 1) {
            printf("Invalid input.\n");
            // Write to log
            logMessage(LOG_WARNING, "Main menu: Invalid input format for menu selection.");
            continue;
        }


        switch (choice) {
        case 1: customersMenu(customers, &customerCount); break;
        case 2:
            // Parts menu (add other students' code here)
            printf("Parts menu (add other people's code here)\n");
            break;
        case 3:
            // Order menu (add other students' code here) 
            printf("Order menu (add other people's code here)\n");
            break;
        case 4:
            // Save data before exiting
            saveCustomers(customers, customerCount);
            printf("Customer data saved. Goodbye!\n");
            break;
        default:
            // Invalid menu selection
            printf("Invalid option. Try again.\n");
            // Write to log
            logMessage(LOG_WARNING, "Main menu: Invalid menu choice.");
        }
    } while (choice != 4);

    // Free dynamically allocated memory before exiting
    free(customers);
    return 0;
}

/*
* FUNCTION     : customersMenu
* DESCRIPTION  : Displays the customer menu and handles user choices
* PARAMETERS   : Customer customers[], int* count
* RETURNS      : void
*/
void customersMenu(Customer customers[], int* count) {

    int choice;
    char buffer[MAX_INPUT_LENGTH];

    do {
        printf("\n----- Customers Menu -----\n");
        printf("1. List all Customers\n");
        printf("2. Search for Customer\n");
        printf("3. Enter New Customer\n");
        printf("4. Update Customer Information\n");
        printf("5. Customer in Bad Credit Standing\n");
        printf("6. Load Customers\n");
        printf("7. Save Customers\n");
        printf("8. Return to Main Menu\n");
        printf("Select: ");

        // Read input and Parse number
        if (!fgets(buffer, MAX_INPUT_LENGTH, stdin)) continue;
        if (sscanf_s(buffer, "%d", &choice) != 1) {
            printf("Invalid input.\n");
            // Write to log
            logMessage(LOG_WARNING, "Customers menu: Invalid input format.");
            continue;
        }

        switch (choice) {
        case 1: listAllCustomers(customers, *count);                break;
        case 2: searchCustomer(customers, *count);                  break;
        case 3: addCustomer(customers, count);                      break;
        case 4: updateCustomerInfo(customers, *count);              break;
        case 5: listBadCreditCustomers(customers, *count);          break;
        case 6: *count = loadCustomers(customers, MAX_CUSTOMERS);   break;
        case 7: saveCustomers(customers, *count);                   break;
        case 8:                                                     return;
        default:
            // Write to log
            printf("Invalid choice.\n");
            logMessage(LOG_WARNING, "Customers menu: Invalid menu choice.");
        }
    } while (1);
}

/*
* FUNCTION     : listAllCustomers
* DESCRIPTION  : Displays a menu to choose customer list format (horizontal or vertical).
* PARAMETERS   : customers[] - array of customers
*                count       - number of customers
* RETURNS      : void
*/
void listAllCustomers(Customer customers[], int count) {

    int choice;
    char buffer[MAX_INPUT_LENGTH];

    do {
        printf("\n----- Customer Display Options -----\n");
        printf("1. Horizontal (pipe-separated ¡®|¡¯)\n");
        printf("2. Vertical (record-style)\n");
        printf("3. Return to Customers Menu\n");
        printf("Select an option: ");

        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        if (sscanf_s(buffer, "%d", &choice) != 1) {
            printf("Invalid input.\n");
            // Write to log
            logMessage(LOG_WARNING, "Customer display menu: Invalid input format.");
            continue;
        }

        switch (choice) {
        case 1:
            printf("\n----- All Customers (Horizontal) -----\n");
            Display_AllCustomers_Horizontal(customers, count);
            break;
        case 2:
            printf("\n----- All Customers (Vertical) -----\n");
            Display_AllCustomers_Vertical(customers, count);
            break;
        case 3:
            return;
        default:
            printf("Invalid option. Try again.\n");
            // Write to log
            logMessage(LOG_WARNING, "Customer display menu: Invalid menu choice.");
        }
    } while (1);
}

/*
* FUNCTION     : searchCustomer
* DESCRIPTION  : Searches all fields of all customers using keyword
* PARAMETERS   : customers[] - customer array
*                count       - number of customers
* RETURNS      : void
*/
void searchCustomer(Customer customers[], int count) {
    // Keywords entered by the user
    char keyword[51];
    int found = 0;

    // Handle empty customer list
    if (count == 0) {
        printf("There are no customers in the database.\n");
        // Write to log
        logMessage(LOG_INFO, "Search aborted: No customers loaded.");
        return;
    }

    // Prompt for keyword input
    printf("Enter keyword to search all fields: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    // If the user does not enter a keyword
    if (strlen(keyword) == 0) {
        printf("No keyword entered. Search cancelled.\n");
        logMessage(LOG_INFO, "Search aborted: Empty keyword.");
        return;
    }

    printf("\n----- Search Results -----\n");

    // Allocate memory dynamically for matched customers
    Customer* matched = (Customer*)malloc(sizeof(Customer) * count);
    if (!matched) {
        printf("Error: Unable to allocate memory for search results.\n");
        logMessage(LOG_ERROR, "Failed to allocate memory in searchCustomer.");
        return;
    }

    // Loop through all customers
    for (int i = 0; i < count; i++) {
        // Current customer
        Customer c = customers[i];

        // Prepare last payment display
        char lastPaymentDisplay[12];

        if (strlen(c.lastPayment) > 0) {
            // Copy if not empty
            strcpy_s(lastPaymentDisplay, sizeof(lastPaymentDisplay), c.lastPayment);
        }
        else {
            // Display (none)
            strcpy_s(lastPaymentDisplay, sizeof(lastPaymentDisplay), "(none)");
        }

        // Concatenate all fields for keyword search
        char recordLine[512];
        sprintf_s(recordLine, sizeof(recordLine), "%d|%s|%s|%s|%s|%s|%s|%s|%.2f|%.2f|%s|%s",
            c.customerID, c.name, c.address, c.city, c.province, c.postalCode,
            c.phone, c.email, c.creditLimit, c.accountBalance, c.joinDate, lastPaymentDisplay);

        // If keyword found, store match
        if (strstr(recordLine, keyword)) {
            matched[found++] = c;
        }
    }

    // No results found
    if (found == 0) {
        printf("No customer records matched keyword \"%s\".\n", keyword);
        // Write to log
        logMessage(LOG_INFO, "Search result: No match found.");
    }
    else {
        // Display matched customers
        Display_AllCustomers_Vertical(matched, found);
    }
    // Free the dynamic memory
    free(matched);
}

/*
* FUNCTION     : addCustomer
* DESCRIPTION  : Adds a new customer by prompting user input.
* PARAMETERS   : Customer customers[] - array to add into
*                int* count - pointer to number of customers
* RETURNS      : void
*/
void addCustomer(Customer customers[], int* count) {
    // Check if max limit is reached
    if (*count >= MAX_CUSTOMERS) {
        printf("Customer limit reached.\n");
        // Write to log
        logMessage(LOG_ERROR, "Add customer failed: Reached MAX_CUSTOMERS limit.");
        return;
    }

    // Temporary struct to hold new customer data
    Customer c;

    // Set the customer ID, 
    // the first customer * count is 1, 
    // the previous customer ID + 1
    c.customerID = (*count == 0) ? 1 : customers[*count - 1].customerID + 1;

    // Default credit limit
    c.creditLimit = 500.00f;
    // Initial account balance
    c.accountBalance = 0.00f;

    // Get current date as joinDate
    time_t t = time(NULL);
    struct tm tm_info;

    if (localtime_s(&tm_info, &t) == 0) {
        strftime(c.joinDate, sizeof(c.joinDate), "%Y-%m-%d", &tm_info);
    }
    else {
        printf("Error: Could not get local time.\n");
        strcpy_s(c.joinDate, sizeof(c.joinDate), "0000-00-00");
        // Write to log
        logMessage(LOG_WARNING, "Join date set to default due to localtime failure.");
    }

    // Initialize lastPayment to empty string
    strcpy_s(c.lastPayment, sizeof(c.lastPayment), "");

    char buffer[MAX_INPUT_LENGTH];

    // Name 
    while (1) {
        printf("Enter name: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0 && strlen(buffer) <= 50) {
            strcpy_s(c.name, sizeof(c.name), buffer);
            break;
        }
        printf("Invalid name. Try again.\n");
    }
    // address
    while (1) {
        printf("Enter address: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0 && strlen(buffer) <= 100) {
            strcpy_s(c.address, sizeof(c.address), buffer);
            break;
        }
        printf("Invalid address. Try again.\n");
    }
    // city
    while (1) {
        printf("Enter city: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0 && strlen(buffer) <= 100) {
            strcpy_s(c.city, sizeof(c.city), buffer);
            break;
        }
        printf("Invalid city. Try again.\n");
    }
    // province
    while (1) {
        printf("Enter province (e.g., ON): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (isValidProvince(buffer)) {
            strcpy_s(c.province, sizeof(c.province), buffer);
            break;
        }
        printf("Invalid province. Try again.\n");
    }
    // postalCode
    while (1) {
        printf("Enter postal code (A1A1A1): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (isValidPostalCode(buffer)) {
            strcpy_s(c.postalCode, sizeof(c.postalCode), buffer);
            break;
        }
        printf("Invalid postal code. Try again.\n");
    }
    // phone
    while (1) {
        printf("Enter phone (123-456-7890): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (isValidPhone(buffer)) {
            strcpy_s(c.phone, sizeof(c.phone), buffer);
            break;
        }
        printf("Invalid phone. Try again.\n");
    }
    // email
    while (1) {
        printf("Enter email: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0 && strlen(buffer) <= 50) {
            strcpy_s(c.email, sizeof(c.email), buffer);
            break;
        }
        printf("Invalid email. Try again.\n");
    }

    // Save customer to array
    customers[*count] = c;
    (*count)++;

    printf("Customer added with ID %d\n", c.customerID);
    // Write to log
    logMessage(LOG_INFO, "New customer successfully added.");
}

/*
* FUNCTION     : updateCustomerInfo
* DESCRIPTION  : Prompts user to update a customer's information by ID, with input validation.
* PARAMETERS   : Customer customers[], int count
* RETURNS      : void
*/
void updateCustomerInfo(Customer customers[], int count) {
    // Customer ID
    int id;
    char buffer[MAX_INPUT_LENGTH];

    printf("Enter customer ID to update: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    if (sscanf_s(buffer, "%d", &id) != 1) {
        printf("Invalid ID.\n");
        return;
    }

    // Index of found customer
    int index = -1;
    for (int i = 0; i < count; i++) {
        if (customers[i].customerID == id) {
            index = i;
            break;
        }
    }

    // Not found message
    if (index == -1) {
        printf("Customer with ID %d not found.\n", id);
        return;
    }

    // Pointer to target customer
    Customer* c = &customers[index];
    printf("Updating info for %s (ID %d)\n", c->name, c->customerID);

    // Email
    printf("Enter new email (blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (strlen(buffer) <= 50) {
            strcpy_s(c->email, sizeof(c->email), buffer);
        }
        else { printf("Invalid email length (max 50). Skipped.\n"); }
    }
    // Update phone
    printf("Enter new phone (123-456-7890, blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (isValidPhone(buffer)) {
            strcpy_s(c->phone, sizeof(c->phone), buffer);
        }
        else { printf("Invalid phone format. Skipped.\n"); }
    }
    // Update Address
    printf("Enter new address (blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (strlen(buffer) <= 100) {
            strcpy_s(c->address, sizeof(c->address), buffer);
        }
        else { printf("Invalid address length (max 100). Skipped.\n"); }
    }
    // Update City
    printf("Enter new city (blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (strlen(buffer) <= 100) {
            strcpy_s(c->city, sizeof(c->city), buffer);
        }
        else { printf("Invalid city length (max 100). Skipped.\n"); }
    }
    // Update Province
    printf("Enter new province (2 letters, blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (strlen(buffer) == 2 && isValidProvince(buffer)) {
            strcpy_s(c->province, sizeof(c->province), buffer);
        }
        else { printf("Invalid province code. Skipped.\n"); }
    }
    // Update Postal code
    printf("Enter new postal code (A1A1A1, blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (isValidPostalCode(buffer)) {
            strcpy_s(c->postalCode, sizeof(c->postalCode), buffer);
        }
        else { printf("Invalid postal code. Skipped.\n"); }
    }

    printf("Customer record updated.\n");
    // Write to log
    logMessage(LOG_INFO, "Customer record updated successfully.");
}

/*
* FUNCTION     : listBadCreditCustomers
* DESCRIPTION  : Lists all customers whose account balance exceeds their credit limit.
* PARAMETERS   : Customer customers[], int count
* RETURNS      : void
*/
void listBadCreditCustomers(Customer customers[], int count) {

    printf("\n--- Customers in Bad Credit Standing ---\n");

    // Flag to track if any bad credit customers found
    int found = 0;
    // Loop through all customers
    for (int i = 0; i < count; i++) {
        // Check if balance exceeds credit limit
        if (customers[i].accountBalance > customers[i].creditLimit) {
            printf("ID: %d | Name: %s | Email: %s | Balance: $%.2f | Limit: $%.2f\n",
                customers[i].customerID,
                customers[i].name,
                customers[i].email,
                customers[i].accountBalance,
                customers[i].creditLimit);
            // Set as Found
            found = 1;
        }
    }
    if (!found) {
        printf("No customers with bad credit.\n");
        // Write to log
        logMessage(LOG_INFO, "Checked for bad credit customers: none found.");
    }
}

/*
* FUNCTION     : loadCustomers
* DESCRIPTION  : Loads customer records from customers.db into an array with strict field validation.
*                Logs detailed error messages to runtime.log for any invalid field.
* PARAMETERS   : Customer customers[] - array to store loaded data
*                int max - max number of records
* RETURNS      : int - number of customers loaded
*/
int loadCustomers(Customer customers[], int max) {

    FILE* fp = NULL;
    // Safely open file in read mode
    errno_t err = fopen_s(&fp, "customers.db", "r");
    if (err != 0 || fp == NULL) {
        // Log error if file can't be opened
        logMessage(LOG_ERROR, "Failed to open customers.db.");
        return 0;
    }

    // Buffer for reading lines
    char line[MAX_LINE_LENGTH];
    // Count of successfully loaded customers
    int count = 0;
    // Line number tracker
    int lineNumber = 0;

    // Read line-by-line until EOF or max
    while (fgets(line, sizeof(line), fp) && count < max) {

        lineNumber++;
        line[strcspn(line, "\r\n")] = '\0';

        // Temporary customer structure
        Customer c;
        char* token;
        char* context = NULL;
        // Array to store split fields
        char* fields[12] = { NULL };

        int fieldCount = 0;
        token = strtok_s(line, "|", &context);
        while (token && fieldCount < 12) {
            // Store each token
            fields[fieldCount++] = token;
            token = strtok_s(NULL, "|", &context);
        }

        //Incomplete field check
        if (fieldCount < 11) {
            // Write to log
            logFieldError("Missing fields", lineNumber, line);
            continue;
        }

        // Field length and format verification + logging
        if (strlen(fields[0]) == 0 || strlen(fields[0]) > 50) {
            logFieldError("Name", lineNumber, fields[0]); continue;
        }
        strcpy_s(c.name, sizeof(c.name), fields[0]);

        if (strlen(fields[1]) == 0 || strlen(fields[1]) > 100) {
            logFieldError("Address", lineNumber, fields[1]); continue;
        }
        strcpy_s(c.address, sizeof(c.address), fields[1]);

        if (strlen(fields[2]) == 0 || strlen(fields[2]) > 100) {
            logFieldError("City", lineNumber, fields[2]); continue;
        }
        strcpy_s(c.city, sizeof(c.city), fields[2]);

        if (strlen(fields[3]) != 2 || !isValidProvince(fields[3])) {
            logFieldError("Province", lineNumber, fields[3]); continue;
        }
        strcpy_s(c.province, sizeof(c.province), fields[3]);

        if (strlen(fields[4]) != 6 || !isValidPostalCode(fields[4])) {
            logFieldError("PostalCode", lineNumber, fields[4]); continue;
        }
        strcpy_s(c.postalCode, sizeof(c.postalCode), fields[4]);

        if (strlen(fields[5]) != 12 || !isValidPhone(fields[5])) {
            logFieldError("Phone", lineNumber, fields[5]); continue;
        }
        strcpy_s(c.phone, sizeof(c.phone), fields[5]);

        if (strlen(fields[6]) == 0 || strlen(fields[6]) > 50) {
            logFieldError("Email", lineNumber, fields[6]); continue;
        }
        strcpy_s(c.email, sizeof(c.email), fields[6]);

        if (sscanf_s(fields[7], "%d", &c.customerID) != 1 || c.customerID <= 0) {
            logFieldError("CustomerID", lineNumber, fields[7]); continue;
        }

        if (sscanf_s(fields[8], "%f", &c.creditLimit) != 1 || c.creditLimit <= 0.0f) {
            logFieldError("CreditLimit", lineNumber, fields[8]); continue;
        }

        if (sscanf_s(fields[9], "%f", &c.accountBalance) != 1 || c.accountBalance < 0.0f) {
            logFieldError("AccountBalance", lineNumber, fields[9]); continue;
        }

        if (strlen(fields[10]) != 10 || !isValidDateFormat(fields[10])) {
            logFieldError("JoinDate", lineNumber, fields[10]); continue;
        }
        strcpy_s(c.joinDate, sizeof(c.joinDate), fields[10]);

        // Optional field LastPayment
        if (fieldCount == 12 && strlen(fields[11]) > 0) {
            if (strlen(fields[11]) > 10 || !isValidDateFormat(fields[11])) {
                logFieldError("LastPayment", lineNumber, fields[11]);
                continue;
            }
            strcpy_s(c.lastPayment, sizeof(c.lastPayment), fields[11]);
        }
        else {
            // LastPayment can be empty (log warning)
            logMessage(LOG_WARNING, "Line contains empty LastPayment (acceptable).");
            c.lastPayment[0] = '\0';
        }

        // All fields valid, add customer to array
        customers[count++] = c;
    }

    fclose(fp);

    // Log how many records loaded
    char msg[100];
    sprintf_s(msg, sizeof(msg), "Successfully loaded %d customers.", count);
    logMessage(LOG_INFO, msg);

    return count;
}

/*
* FUNCTION     : saveCustomers
* DESCRIPTION  : Writes all customer records to customers.db file.
* PARAMETERS   : Customer customers[] - array to save
*                int count - number of valid entries
* RETURNS      : void
*/
void saveCustomers(Customer customers[], int count) {

    FILE* fp = NULL;
    // Open the file safely (write mode)
    errno_t err = fopen_s(&fp, "customers.db", "w");
    if (err != 0 || fp == NULL) {
        printf("Error: Cannot write to customers.db (code %d)\n", err);
        return;
    }

    // Write each customer to file in pipe format
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%s|%d|%.2f|%.2f|%s|%s|\n",
            customers[i].name, customers[i].address,
            customers[i].city, customers[i].province,
            customers[i].postalCode, customers[i].phone,
            customers[i].email, customers[i].customerID,
            customers[i].creditLimit, customers[i].accountBalance,
            customers[i].joinDate, customers[i].lastPayment);
    }
    fclose(fp);

    // Log number of records saved
    char msg[100];
    sprintf_s(msg, sizeof(msg), "Saved %d customer records to customers.db.", count);
    logMessage(LOG_INFO, msg);
    printf("Customers saved successfully.\n");
}

/*
* FUNCTION     : isLetter
* DESCRIPTION  : Checks whether a character is an English alphabet letter (uppercase or lowercase).
* PARAMETERS   : char c - The character to check.
* RETURNS      : int - Returns 1 if the character is a letter; otherwise 0.
*/
int isLetter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/*
* FUNCTION     : isDigit
* DESCRIPTION  : Checks whether a character is a digit (0¨C9).
* PARAMETERS   : char c - The character to check.
* RETURNS      : int - Returns 1 if the character is a digit; otherwise 0.
*/
int isDigit(char c) {
    return c >= '0' && c <= '9';
}

/*
* FUNCTION     : isValidProvince
* DESCRIPTION  : Validates if the provided code is one of the recognized Canadian province codes.
* PARAMETERS   : const char* code - A 2-letter province code.
* RETURNS      : int - Returns 1 if the code is valid; otherwise 0.
*/
int isValidProvince(const char* code) {
    const char* provinces[] = {
        "ON", "QC", "NS", "NB", "MB", "BC", "PE", "SK",
        "AB", "NL", "NT", "YT", "NU"
    };
    for (int i = 0; i < sizeof(provinces) / sizeof(provinces[0]); i++) {
        if (strcmp(code, provinces[i]) == 0)
            return 1;
    }
    return 0;
}

/*
* FUNCTION     : isValidPostalCode
* DESCRIPTION  : Checks if the postal code is in the format A1A1A1.
* PARAMETERS   : const char* code - The postal code string.
* RETURNS      : int - Returns 1 if the format is valid; otherwise 0.
*/
int isValidPostalCode(const char* code) {
    if (strlen(code) != 6) return 0;
    for (int i = 0; i < 6; i++) {
        if (i % 2 == 0 && !isLetter(code[i])) return 0;
        if (i % 2 == 1 && !isDigit(code[i])) return 0;
    }
    return 1;
}

/*
* FUNCTION     : isValidPhone
* DESCRIPTION  : Validates phone number format as 123-456-7890.
* PARAMETERS   : const char* phone - The phone number string.
* RETURNS      : int - Returns 1 if the format is valid; otherwise 0.
*/
int isValidPhone(const char* phone) {
    if (strlen(phone) != 12) return 0;
    for (int i = 0; i < 12; i++) {
        if (i == 3 || i == 7) {
            if (phone[i] != '-') return 0;
        }
        else {
            if (!isDigit(phone[i])) return 0;
        }
    }
    return 1;
}

/*
* FUNCTION     : isValidDateFormat
* DESCRIPTION  : Validates if the string matches the YYYY-MM-DD date format.
* PARAMETERS   : const char* date - The date string.
* RETURNS      : int - Returns 1 if the format is valid; otherwise 0.
*/
int isValidDateFormat(const char* date) {

    if (strlen(date) != 10) return 0;

    if (date[4] != '-' || date[7] != '-') return 0;

    int digitPositions[] = { 0, 1, 2, 3, 5, 6, 8, 9 };

    for (int i = 0; i < 8; i++) {
        char ch = date[digitPositions[i]];
        if (isDigit(ch) == 0) {
            return 0;
        }
    }
    return 1;
}

/*
* FUNCTION     : logMessage
* DESCRIPTION  : Appends a timestamped log message to runtime.log using printf-style format
* PARAMETERS   : level   - log level (INFO, WARNING, ERROR)
*                format  - printf-style format string
*                ...     - additional format arguments
* RETURNS      : void
*/
void logMessage(const char* level, const char* message) {
    FILE* logfp = NULL;
    fopen_s(&logfp, "runtime.log", "a");
    if (!logfp) return;

    time_t now = time(NULL);
    struct tm t;
    localtime_s(&t, &now);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &t);
    fprintf(logfp, "[%s][%s]: %s\n", timestamp, level, message);

    fclose(logfp);
}

/*
* FUNCTION     : logFieldError
* DESCRIPTION  : Logs field-specific validation error
* PARAMETERS   : fieldName - field being validated
*                line      - line number in input file
*                value     - actual field content
* RETURNS      : void
*/
void logFieldError(const char* fieldName, int line, const char* value) {
    char msg[256];
    sprintf_s(msg, sizeof(msg), "Line %d, Invalid field [%s]: %s", line, fieldName, value);
    logMessage(LOG_ERROR, msg);
}

/*
* FUNCTION     : Display_AllCustomers_Vertical
* DESCRIPTION  : Displays all customers in vertical record style
* PARAMETERS   : customers[] - customer array
*                count       - number of customers
* RETURNS      : void
*/
void Display_AllCustomers_Horizontal(Customer customers[], int count) {

    printf("ID|Name|Address|City|Province|PostalCode|Phone|Email|CreditLimit|AccountBalance|JoinDate|LastPayment\n");
    printf("----------------------------------------------------------------------------------------------------------\n");

    if (count == 0) {
        printf("There are no customers in the database.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        Customer c = customers[i];
        printf("%d|%s|%s|%s|%s|%s|%s|%s|%.2f|%.2f|%s|%s\n",
            c.customerID, c.name, c.address,
            c.city, c.province, c.postalCode,
            c.phone, c.email, c.creditLimit,
            c.accountBalance, c.joinDate,
            strlen(c.lastPayment) > 0 ? c.lastPayment : "(none)");
    }
}

/*
* FUNCTION     : Display_AllCustomers_Vertical
* DESCRIPTION  : Displays all customers in vertical record style
* PARAMETERS   : customers[] - customer array
*                count       - number of customers
* RETURNS      : void
*/
void Display_AllCustomers_Vertical(Customer customers[], int count) {

    if (count == 0) {
        printf("There are no customers in the database.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        Customer c = customers[i];
        printf("------------------------------\n");
        printf("Customer ID       : %d\n", c.customerID);
        printf("Name              : %s\n", c.name);
        printf("Address           : %s\n", c.address);
        printf("City              : %s\n", c.city);
        printf("Province          : %s\n", c.province);
        printf("Postal Code       : %s\n", c.postalCode);
        printf("Phone             : %s\n", c.phone);
        printf("Email             : %s\n", c.email);
        printf("Credit Limit      : $%.2f\n", c.creditLimit);
        printf("Account Balance   : $%.2f\n", c.accountBalance);
        printf("Join Date         : %s\n", c.joinDate);
        printf("Last Payment      : %s\n", strlen(c.lastPayment) > 0 ? c.lastPayment : "(none)");
        printf("------------------------------\n");
    }
}
