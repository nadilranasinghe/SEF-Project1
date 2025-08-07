/*
* FILE          : Customer.cpp
* PROJECT       : PWH Warehouse Management System
* PROGRAMMERS    : Najaf Ali, Che-Ping Chien, Nadil Devnath Ranasinghe, Xinming Xu
* FIRST VERSION : 2025-08-01
* DESCRIPTION   :
*      Implementation of customer management functions including:
*      - Customer data validation
*      - File I/O for customer records
*      - Customer search and display functions
*      - Credit management
*/

#include "Customer.h"
#include "System.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

//
// FUNCTION    : Display_AllCustomers_Horizontal
// DESCRIPTION : Displays all customers in pipe-delimited horizontal format
// PARAMETERS  :
//      Customer customers[] : Array of customer records
//      int count           : Number of customers to display
// RETURNS     : void
//
void Display_AllCustomers_Horizontal(Customer customers[], int count) {
    printf("ID|Name|Address|City|Province|PostalCode|Phone|Email|CreditLimit|AccountBalance|JoinDate|LastPayment\n");
    printf("----------------------------------------------------------------------------------------------------------\n");

    if (count == 0) {
        printf("No customers in database.\n");
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

//
// FUNCTION    : Display_AllCustomers_Vertical
// DESCRIPTION : Displays all customers in formatted vertical layout
// PARAMETERS  :
//      Customer customers[] : Array of customer records
//      int count           : Number of customers to display
// RETURNS     : void
//
void Display_AllCustomers_Vertical(Customer customers[], int count) {
    if (count == 0) {
        printf("No customers in database.\n");
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

//
// FUNCTION    : isValidProvince
// DESCRIPTION : Validates Canadian province/territory codes
// PARAMETERS  :
//      const char* code : 2-letter province code to validate
// RETURNS     : int - 1 if valid, 0 if invalid
//
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

//
// FUNCTION    : isValidPostalCode
// DESCRIPTION : Validates Canadian postal code format (A1A1A1)
// PARAMETERS  :
//      const char* code : Postal code to validate
// RETURNS     : int - 1 if valid, 0 if invalid
//
int isValidPostalCode(const char* code) {
    if (strlen(code) != 6) return 0;
    for (int i = 0; i < 6; i++) {
        if (i % 2 == 0 && !isalpha(code[i])) return 0;
        if (i % 2 == 1 && !isdigit(code[i])) return 0;
    }
    return 1;
}

//
// FUNCTION    : isValidPhone
// DESCRIPTION : Validates phone number format (###-###-####)
// PARAMETERS  :
//      const char* phone : Phone number to validate
// RETURNS     : int - 1 if valid, 0 if invalid
//
int isValidPhone(const char* phone) {
    if (strlen(phone) != 12) return 0;
    for (int i = 0; i < 12; i++) {
        if (i == 3 || i == 7) {
            if (phone[i] != '-') return 0;
        }
        else if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

//
// FUNCTION    : isValidDateFormat
// DESCRIPTION : Validates date format (YYYY-MM-DD)
// PARAMETERS  :
//      const char* date : Date string to validate
// RETURNS     : int - 1 if valid, 0 if invalid
//
int isValidDateFormat(const char* date) {
    if (strlen(date) != 10) return 0;
    if (date[4] != '-' || date[7] != '-') return 0;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return 0;
    }

    int year, month, day;
    if (sscanf_s(date, "%d-%d-%d", &year, &month, &day) != 3)
        return 0;

    if (year < 2000 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;

    return 1;
}

//
// FUNCTION    : listAllCustomers
// DESCRIPTION : Provides menu for selecting customer display format
// PARAMETERS  :
//      Customer customers[] : Array of customer records
//      int count           : Number of customers
// RETURNS     : void
//
void listAllCustomers(Customer customers[], int count) {
    int choice;
    char buffer[MAX_INPUT_LENGTH];

    do {
        printf("\n----- Customer Display Options -----\n");
        printf("1. Horizontal (pipe-separated)\n");
        printf("2. Vertical (record-style)\n");
        printf("3. Return to Customers Menu\n");
        printf("Select an option: ");

        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        if (sscanf_s(buffer, "%d", &choice) != 1) {
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
        case 1: Display_AllCustomers_Horizontal(customers, count); break;
        case 2: Display_AllCustomers_Vertical(customers, count); break;
        case 3: return;
        default: printf("Invalid option. Try again.\n");
        }
    } while (1);
}

//
// FUNCTION    : searchCustomer
// DESCRIPTION : Searches customers by keyword across all fields
// PARAMETERS  :
//      Customer customers[] : Array of customer records
//      int count           : Number of customers
// RETURNS     : void
//
void searchCustomer(Customer customers[], int count) {
    char keyword[51];
    int found = 0;

    if (count == 0) {
        printf("No customers in database.\n");
        return;
    }

    printf("Enter keyword to search: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    if (strlen(keyword) == 0) {
        printf("No keyword entered.\n");
        return;
    }

    printf("\n----- Search Results -----\n");
    for (int i = 0; i < count; i++) {
        Customer c = customers[i];
        char recordLine[512];
        sprintf_s(recordLine, sizeof(recordLine), "%d|%s|%s|%s|%s|%s|%s|%s|%.2f|%.2f|%s|%s",
            c.customerID, c.name, c.address, c.city, c.province, c.postalCode,
            c.phone, c.email, c.creditLimit, c.accountBalance, c.joinDate,
            strlen(c.lastPayment) > 0 ? c.lastPayment : "(none)");

        if (strstr(recordLine, keyword)) {
            Display_AllCustomers_Vertical(&c, 1);
            found = 1;
        }
    }

    if (!found) {
        printf("No matches found.\n");
    }
}

//
// FUNCTION    : addCustomer
// DESCRIPTION : Adds a new customer with validated input
// PARAMETERS  :
//      Customer customers[] : Array of customer records
//      int* count          : Pointer to customer count (will be incremented)
// RETURNS     : void
//
void addCustomer(Customer customers[], int* count) {
    if (*count >= MAX_CUSTOMERS) {
        printf("Customer limit reached.\n");
        return;
    }

    Customer c;
    c.customerID = (*count == 0) ? 1 : customers[*count - 1].customerID + 1;
    c.creditLimit = 500.00f;
    c.accountBalance = 0.00f;
    strcpy_s(c.lastPayment, sizeof(c.lastPayment), "");

    time_t t = time(NULL);
    struct tm tm_info;
    if (localtime_s(&tm_info, &t) == 0) {
        strftime(c.joinDate, sizeof(c.joinDate), "%Y-%m-%d", &tm_info);
    }

    char buffer[MAX_INPUT_LENGTH];

    while (1) {
        printf("Enter name: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0 && strlen(buffer) <= 50) {
            strcpy_s(c.name, sizeof(c.name), buffer);
            break;
        }
        printf("Invalid name.\n");
    }

    while (1) {
        printf("Enter address: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0 && strlen(buffer) <= 100) {
            strcpy_s(c.address, sizeof(c.address), buffer);
            break;
        }
        printf("Invalid address.\n");
    }

    while (1) {
        printf("Enter city: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0 && strlen(buffer) <= 100) {
            strcpy_s(c.city, sizeof(c.city), buffer);
            break;
        }
        printf("Invalid city.\n");
    }

    while (1) {
        printf("Enter province (e.g., ON): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (isValidProvince(buffer)) {
            strcpy_s(c.province, sizeof(c.province), buffer);
            break;
        }
        printf("Invalid province.\n");
    }

    while (1) {
        printf("Enter postal code (A1A1A1): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (isValidPostalCode(buffer)) {
            strcpy_s(c.postalCode, sizeof(c.postalCode), buffer);
            break;
        }
        printf("Invalid postal code.\n");
    }

    while (1) {
        printf("Enter phone (123-456-7890): ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (isValidPhone(buffer)) {
            strcpy_s(c.phone, sizeof(c.phone), buffer);
            break;
        }
        printf("Invalid phone.\n");
    }

    while (1) {
        printf("Enter email: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0 && strlen(buffer) <= 50) {
            strcpy_s(c.email, sizeof(c.email), buffer);
            break;
        }
        printf("Invalid email.\n");
    }

    customers[*count] = c;
    (*count)++;
    printf("Customer added with ID %d\n", c.customerID);
    logMessage("New customer added");
}

//
// FUNCTION    : updateCustomerInfo
// DESCRIPTION : Updates existing customer information with validation
// PARAMETERS  :
//      Customer customers[] : Array of customer records
//      int count           : Number of customers
// RETURNS     : void
//
void updateCustomerInfo(Customer customers[], int count) {
    int id;
    char buffer[MAX_INPUT_LENGTH];

    printf("Enter customer ID to update: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) return;
    if (sscanf_s(buffer, "%d", &id) != 1) {
        printf("Invalid ID.\n");
        return;
    }

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (customers[i].customerID == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Customer not found.\n");
        return;
    }

    Customer* c = &customers[index];
    printf("Updating info for %s (ID %d)\n", c->name, c->customerID);

    printf("Enter new email (blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (strlen(buffer) <= 50) {
            strcpy_s(c->email, sizeof(c->email), buffer);
        }
        else {
            printf("Invalid email length.\n");
        }
    }

    printf("Enter new phone (123-456-7890, blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (isValidPhone(buffer)) {
            strcpy_s(c->phone, sizeof(c->phone), buffer);
        }
        else {
            printf("Invalid phone format.\n");
        }
    }

    printf("Enter new address (blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (strlen(buffer) <= 100) {
            strcpy_s(c->address, sizeof(c->address), buffer);
        }
        else {
            printf("Invalid address length.\n");
        }
    }

    printf("Enter new city (blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (strlen(buffer) <= 100) {
            strcpy_s(c->city, sizeof(c->city), buffer);
        }
        else {
            printf("Invalid city length.\n");
        }
    }

    printf("Enter new province (2 letters, blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (strlen(buffer) == 2 && isValidProvince(buffer)) {
            strcpy_s(c->province, sizeof(c->province), buffer);
        }
        else {
            printf("Invalid province code.\n");
        }
    }

    printf("Enter new postal code (A1A1A1, blank to skip): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        if (isValidPostalCode(buffer)) {
            strcpy_s(c->postalCode, sizeof(c->postalCode), buffer);
        }
        else {
            printf("Invalid postal code.\n");
        }
    }

    printf("Customer record updated.\n");
    logMessage("Customer information updated");
}

//
// FUNCTION    : listBadCreditCustomers
// DESCRIPTION : Lists customers who have exceeded their credit limit
// PARAMETERS  :
//      Customer customers[] : Array of customer records
//      int count           : Number of customers
// RETURNS     : void
//
void listBadCreditCustomers(Customer customers[], int count) {
    printf("\n--- Customers in Bad Credit Standing ---\n");
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (customers[i].accountBalance > customers[i].creditLimit) {
            printf("ID: %d | Name: %s | Email: %s | Balance: $%.2f | Limit: $%.2f\n",
                customers[i].customerID,
                customers[i].name,
                customers[i].email,
                customers[i].accountBalance,
                customers[i].creditLimit);
            found = 1;
        }
    }

    if (!found) {
        printf("No customers with bad credit.\n");
    }
}

//
// FUNCTION    : loadCustomers
// DESCRIPTION : Loads customers from file with validation
// PARAMETERS  :
//      Customer customers[] : Array to store loaded customers
//      int max             : Maximum number of customers to load
// RETURNS     : int - Number of customers successfully loaded
//
int loadCustomers(Customer customers[], int max) {
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, "customers.db", "r");
    if (err != 0 || fp == NULL) {
        printf("Error loading customers.\n");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), fp) && count < max) {
        line[strcspn(line, "\r\n")] = '\0';

        Customer c;
        char* context = NULL;
        char* fields[12] = { NULL };
        int fieldCount = 0;

        char* token = strtok_s(line, "|", &context);
        while (token && fieldCount < 12) {
            fields[fieldCount++] = token;
            token = strtok_s(NULL, "|", &context);
        }

        if (fieldCount < 11) continue;

        if (strlen(fields[0]) == 0 || strlen(fields[0]) > 50) continue;
        strcpy_s(c.name, sizeof(c.name), fields[0]);

        if (strlen(fields[1]) == 0 || strlen(fields[1]) > 100) continue;
        strcpy_s(c.address, sizeof(c.address), fields[1]);

        if (strlen(fields[2]) == 0 || strlen(fields[2]) > 100) continue;
        strcpy_s(c.city, sizeof(c.city), fields[2]);

        if (strlen(fields[3]) != 2 || !isValidProvince(fields[3])) continue;
        strcpy_s(c.province, sizeof(c.province), fields[3]);

        if (strlen(fields[4]) != 6 || !isValidPostalCode(fields[4])) continue;
        strcpy_s(c.postalCode, sizeof(c.postalCode), fields[4]);

        if (strlen(fields[5]) != 12 || !isValidPhone(fields[5])) continue;
        strcpy_s(c.phone, sizeof(c.phone), fields[5]);

        if (strlen(fields[6]) == 0 || strlen(fields[6]) > 50) continue;
        strcpy_s(c.email, sizeof(c.email), fields[6]);

        if (sscanf_s(fields[7], "%d", &c.customerID) != 1 || c.customerID <= 0) continue;

        if (sscanf_s(fields[8], "%f", &c.creditLimit) != 1 || c.creditLimit <= 0.0f) continue;

        if (sscanf_s(fields[9], "%f", &c.accountBalance) != 1 || c.accountBalance < 0.0f) continue;

        if (strlen(fields[10]) != 10 || !isValidDateFormat(fields[10])) continue;
        strcpy_s(c.joinDate, sizeof(c.joinDate), fields[10]);

        if (fieldCount == 12 && strlen(fields[11]) > 0) {
            if (strlen(fields[11]) > 10 || !isValidDateFormat(fields[11])) continue;
            strcpy_s(c.lastPayment, sizeof(c.lastPayment), fields[11]);
        }
        else {
            c.lastPayment[0] = '\0';
        }

        customers[count++] = c;
    }

    fclose(fp);
    printf("Loaded %d customers from customers.db\n", count);
    logMessage("Customer database loaded");
    return count;
}

//
// FUNCTION    : saveCustomers
// DESCRIPTION : Saves all customers to file in pipe-delimited format
// PARAMETERS  :
//      Customer customers[] : Array of customer records to save
//      int count           : Number of customers to save
// RETURNS     : void
//
void saveCustomers(Customer customers[], int count) {
    FILE* fp = NULL;
    errno_t err = fopen_s(&fp, "customers.db", "w");
    if (err != 0 || fp == NULL) {
        printf("Error saving customers.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%s|%d|%.2f|%.2f|%s|%s|\n",
            customers[i].name,
            customers[i].address,
            customers[i].city,
            customers[i].province,
            customers[i].postalCode,
            customers[i].phone,
            customers[i].email,
            customers[i].customerID,
            customers[i].creditLimit,
            customers[i].accountBalance,
            customers[i].joinDate,
            customers[i].lastPayment);
    }

    fclose(fp);
    printf("Saved %d customer records to customers.db.\n", count);
    logMessage("Customer database saved");
}

//
// FUNCTION    : customersMenu
// DESCRIPTION : Main customer management menu interface
// PARAMETERS  :
//      Customer customers[] : Array of customer records
//      int* count          : Pointer to customer count
// RETURNS     : void
//
void customersMenu(Customer customers[], int* count) {
    int choice;
    char buffer[MAX_INPUT_LENGTH];

    do {
        customerSubMenu();

        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        if (sscanf_s(buffer, "%d", &choice) != 1) {
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
        case 1: listAllCustomers(customers, *count); break;
        case 2: searchCustomer(customers, *count); break;
        case 3: addCustomer(customers, count); break;
        case 4: updateCustomerInfo(customers, *count); break;
        case 5: listBadCreditCustomers(customers, *count); break;
        case 6: *count = loadCustomers(customers, MAX_CUSTOMERS); break;
        case 7: saveCustomers(customers, *count); break;
        case 8: return;
        default: printf("Invalid choice.\n");
        }
    } while (1);
}