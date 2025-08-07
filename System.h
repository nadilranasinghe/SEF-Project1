/*
* FILE          : System.h
* PROJECT       : PWH Warehouse Management System
* PROGRAMMERS    : Najaf Ali, Che-Ping Chien, Nadil Devnath Ranasinghe, Xinming Xu
* FIRST VERSION : 2025-08-01
* DESCRIPTION   :
*      Header file for system-level functions including:
*      - Main menu navigation
*      - Submenu displays
*      - System logging functionality
*/

#ifndef SYSTEM_H
#define SYSTEM_H

// Function prototypes
void mainMenu();             // Display main system menu
void partsSubMenu();         // Display parts management submenu
void customerSubMenu();      // Display customer management submenu
void orderSubMenu();         // Display order management submenu
void logMessage(const char* message);  // Log system messages to file

#endif