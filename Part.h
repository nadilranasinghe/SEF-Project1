/*
* FILE          : Part.h
* PROJECT       : PWH Warehouse Management System
* PROGRAMMERS    : Najaf Ali, Che-Ping Chien, Nadil Devnath Ranasinghe, Xinming Xu
* FIRST VERSION : 2025-08-01
* DESCRIPTION   :
*      Header file for parts inventory management including:
*      - Part data structure definition
*      - Inventory size constants
*      - Function prototypes for part operations
*/

#ifndef PART_H
#define PART_H

#define MAXIMUMLENGTH 51    // Maximum length for part name/number/location
#define MAXPARTSIZE 100     // Maximum parts in inventory
#define MAXLINE 100         // Maximum line length for file input
#define LOCATELINE 10       // Length for location components

// Part inventory structure
typedef struct {
    char PartName[MAXIMUMLENGTH];  // Descriptive name of part
    char PartNumber[MAXIMUMLENGTH]; // Manufacturer part number
    char PartLocate[MAXIMUMLENGTH]; // Warehouse location (A###-S###-L###-B###)
    float PartCost;                 // Unit cost
    int QuantityOnHand;             // Current inventory count
    int PartStatus;                 // Inventory status (0=plenty, 99=low, -X=backordered)
    int PartID;                     // Unique part identifier
} Parts;

// Function prototypes
void ListallParts(Parts* part, int* size);             // List all parts in inventory
void SearchforPart(Parts* part, int* size);            // Search for specific part
int AddPart(Parts* part, int* size);                   // Add new part to inventory
void UpdateInventoryforPart(Parts part[], int* size);  // Update part quantity
void SaveToFile(const char* filename, Parts part[], int* size); // Save parts to file
void loadfromfile(const char* filename, Parts part[], int* size); // Load parts from file
void handlePartsMenu(Parts parts[], int* partsCount);  // Main parts menu

#endif