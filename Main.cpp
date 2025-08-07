/*
* FILE          : Main.cpp
* PROJECT       : PWH Warehouse Management System
* PROGRAMMERS   : Najaf Ali, Che-Ping Chien, Nadil Devnath Ranasinghe, Xinming Xu
* FIRST VERSION : 2025-08-01
* DESCRIPTION   :
*      Main program entry point with:
*      - System initialization
*      - Main program loop
*      - Data loading/saving
*      - Menu navigation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Customer.h"
#include "Part.h"
#include "Order.h"
#include "System.h"

//
// FUNCTION    : main
// DESCRIPTION : Program entry point, manages main system loop
// PARAMETERS  : void
// RETURNS     : int - Program exit status
//
int main(void) {
    // Initialize data structures
    Customer customers[MAX_CUSTOMERS];
    Parts parts[MAXPARTSIZE];
    Order orders[MAX_ORDERS];

    // Load initial data
    int customerCount = loadCustomers(customers, MAX_CUSTOMERS);
    int partsCount = 0;
    loadfromfile("parts.db", parts, &partsCount);
    int orderCount = 0;
    loadOrderFromFile(orders, &orderCount);

    int choice;
    char buffer[100];

    // Main program loop
    do {
        mainMenu();

        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        if (sscanf_s(buffer, "%d", &choice) != 1) {
            printf("Invalid input.\n");
            continue;
        }

        // Handle menu selection
        switch (choice) {
        case 1: handlePartsMenu(parts, &partsCount); break;
        case 2: customersMenu(customers, &customerCount); break;
        case 3: handleOrdersMenu(orders, &orderCount, customers, customerCount, parts, partsCount); break;
        case 4:
            printf("\nSaving data...\n");
            saveCustomers(customers, customerCount);
            SaveToFile("parts.db", parts, &partsCount);
            saveOrderToFile(orders, orderCount);
            printf("Data saved. Goodbye!\n");
            break;
        default:
            printf("Invalid option.\n");
        }
    } while (choice != 4);

    return 0;
}