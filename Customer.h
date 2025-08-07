/*
* FILE          : Customer.h
* PROJECT       : PWH Warehouse Management System
* PROGRAMMERS    : Najaf Ali, Che-Ping Chien, Nadil Devnath Ranasinghe, Xinming Xu
* FIRST VERSION : 2025-08-01
* DESCRIPTION   :
*      Header file for customer management functionality including:
*      - Customer data structure definition
*      - Constants for customer management
*      - Function prototypes for customer operations
*/

#ifndef CUSTOMER_H
#define CUSTOMER_H

#define MAX_CUSTOMERS 50        // Maximum number of customers in system
#define MAX_LINE_LENGTH 512     // Maximum length for file input lines
#define MAX_INPUT_LENGTH 100    // Maximum length for user input

// Customer data structure
typedef struct {
    int customerID;             // Unique customer identifier
    char name[51];              // Customer name (max 50 chars + null terminator)
    char address[101];          // Street address (max 100 chars)
    char city[101];             // City (max 100 chars)
    char province[3];           // 2-letter province code + null terminator
    char postalCode[7];         // 6-character postal code + null terminator
    char phone[13];             // Phone number (###-###-#### + null)
    char email[51];             // Email address (max 50 chars)
    float creditLimit;          // Maximum credit allowed
    float accountBalance;       // Current account balance
    char joinDate[11];          // Date joined (YYYY-MM-DD + null)
    char lastPayment[11];       // Date of last payment (YYYY-MM-DD + null)
} Customer;

// Function prototypes
void customersMenu(Customer customers[], int* count);            // Main customer menu
void listAllCustomers(Customer customers[], int count);          // List all customers
void searchCustomer(Customer customers[], int count);            // Search customers
void addCustomer(Customer customers[], int* count);              // Add new customer
void updateCustomerInfo(Customer customers[], int count);        // Update customer info
void listBadCreditCustomers(Customer customers[], int count);    // List customers with bad credit
int loadCustomers(Customer customers[], int max);                // Load customers from file
void saveCustomers(Customer customers[], int count);             // Save customers to file
int isValidProvince(const char* code);                          // Validate province code
int isValidPostalCode(const char* code);                        // Validate postal code
int isValidPhone(const char* phone);                            // Validate phone format
int isValidDateFormat(const char* date);                        // Validate date format

#endif