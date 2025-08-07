/*
* FILE          : System.cpp
* PROJECT       : PWH Warehouse Management System
* PROGRAMMERS   : Najaf Ali, Che-Ping Chien, Nadil Devnath Ranasinghe, Xinming Xu
* FIRST VERSION : 2025-08-01
* DESCRIPTION   :
*      Implementation of system-level functions including:
*      - Menu display functions
*      - System logging functionality
*      - User interface helpers
*/

#include "System.h"
#include <string.h> 
#include <stdio.h>
#include <time.h>

//
// FUNCTION    : mainMenu
// DESCRIPTION : Displays the main system menu options
// PARAMETERS  : None
// RETURNS     : void
//
void mainMenu() {
    printf("\n===== PWH Warehouse System =====\n");
    printf("1. Parts Management\n");
    printf("2. Customer Management\n");
    printf("3. Order Management\n");
    printf("4. Exit System\n");
    printf("===============================\n");
    printf("Select option: ");
}

//
// FUNCTION    : partsSubMenu
// DESCRIPTION : Displays the parts management submenu options
// PARAMETERS  : None
// RETURNS     : void
//
void partsSubMenu() {
    printf("\n----- Parts Management -----\n");
    printf("1. List All Parts\n");
    printf("2. Search for Part\n");
    printf("3. Add New Part\n");
    printf("4. Update Part Inventory\n");
    printf("5. Load Parts Database\n");
    printf("6. Save Parts Database\n");
    printf("7. Return to Main Menu\n");
    printf("----------------------------\n");
    printf("Select option: ");
}

//
// FUNCTION    : customerSubMenu
// DESCRIPTION : Displays the customer management submenu options
// PARAMETERS  : None
// RETURNS     : void
//
void customerSubMenu() {
    printf("\n----- Customer Management -----\n");
    printf("1. List All Customers\n");
    printf("2. Search for Customer\n");
    printf("3. Add New Customer\n");
    printf("4. Update Customer Information\n");
    printf("5. List Bad Credit Customers\n");
    printf("6. Load Customer Database\n");
    printf("7. Save Customer Database\n");
    printf("8. Return to Main Menu\n");
    printf("-------------------------------\n");
    printf("Select option: ");
}

//
// FUNCTION    : orderSubMenu
// DESCRIPTION : Displays the order management submenu options
// PARAMETERS  : None
// RETURNS     : void
//
void orderSubMenu() {
    printf("\n----- Order Management -----\n");
    printf("1. List All Orders\n");
    printf("2. Search for Order\n");
    printf("3. Create New Order\n");
    printf("4. Process End-of-Day Orders\n");
    printf("5. Load Order Database\n");
    printf("6. Save Order Database\n");
    printf("7. Return to Main Menu\n");
    printf("---------------------------\n");
    printf("Select option: ");
}

//
// FUNCTION    : logMessage
// DESCRIPTION : Logs system messages to file with timestamp
// PARAMETERS  :
//      const char* message : Message to log
// RETURNS     : void
//
void logMessage(const char* message) {
    FILE* logFile;
    errno_t err = fopen_s(&logFile, "system.log", "a");
    if (err == 0 && logFile != NULL) {
        time_t now;
        time(&now);
        char timestamp[26];
        ctime_s(timestamp, sizeof(timestamp), &now);
        timestamp[strlen(timestamp) - 1] = '\0';

        fprintf(logFile, "[%s] %s\n", timestamp, message);
        fclose(logFile);
    }
}