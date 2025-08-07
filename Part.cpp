/*
* FILE          : Part.cpp
* PROJECT       : PWH Warehouse Management System
* PROGRAMMERS   : Najaf Ali, Che-Ping Chien, Nadil Devnath Ranasinghe, Xinming Xu
* FIRST VERSION : 2025-08-01
* DESCRIPTION   :
*      Implementation of parts inventory functions including:
*      - Part addition and searching
*      - Inventory level management
*      - File I/O for parts records
*      - Location format validation
*/

#include "Part.h"
#include "System.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//
// FUNCTION    : ListallParts
// DESCRIPTION : Lists all parts in inventory with full details
// PARAMETERS  :
//      Parts* part : Array of part records
//      int* size   : Pointer to current inventory count
// RETURNS     : void
//
void ListallParts(Parts* part, int* size) {
    if (*size == 0) {
        printf("No parts in inventory.\n");
        return;
    }

    for (int i = 0; i < *size; i++) {
        printf("-------------------------------------\n");
        printf("Part ID: %d\n", part[i].PartID);
        printf("Name: %s\n", part[i].PartName);
        printf("Number: %s\n", part[i].PartNumber);
        printf("Location: %s\n", part[i].PartLocate);
        printf("Cost: $%.2f\n", part[i].PartCost);
        printf("Quantity: %d\n", part[i].QuantityOnHand);
        printf("Status: %d\n", part[i].PartStatus);
        printf("-------------------------------------\n");
    }
}

//
// FUNCTION    : SearchforPart
// DESCRIPTION : Searches for part by ID and displays details
// PARAMETERS  :
//      Parts* part : Array of part records
//      int* size   : Pointer to current inventory count
// RETURNS     : void
//
void SearchforPart(Parts* part, int* size) {
    if (*size == 0) {
        printf("No parts in inventory.\n");
        return;
    }

    int id;
    char buffer[100];

    printf("Enter Part ID to search: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error reading input.\n");
        return;
    }

    if (sscanf_s(buffer, "%d", &id) != 1) {
        printf("Invalid input. Please enter a number.\n");
        return;
    }

    for (int i = 0; i < *size; i++) {
        if (part[i].PartID == id) {
            printf("-------------------------------------\n");
            printf("Part ID: %d\n", part[i].PartID);
            printf("Name: %s\n", part[i].PartName);
            printf("Number: %s\n", part[i].PartNumber);
            printf("Location: %s\n", part[i].PartLocate);
            printf("Cost: $%.2f\n", part[i].PartCost);
            printf("Quantity: %d\n", part[i].QuantityOnHand);
            printf("Status: %d\n", part[i].PartStatus);
            printf("-------------------------------------\n");
            return;
        }
    }

    printf("Part with ID %d not found.\n", id);
}

//
// FUNCTION    : AddPart
// DESCRIPTION : Adds new part to inventory with validated input
// PARAMETERS  :
//      Parts* parts : Array of part records
//      int* size    : Pointer to current inventory count
// RETURNS     : int - Updated inventory count
//
int AddPart(Parts* parts, int* size) {
    if (*size >= MAXPARTSIZE) {
        printf("Inventory full.\n");
        return *size;
    }

    Parts newPart;
    char buffer[MAXIMUMLENGTH];
    char aisle[LOCATELINE], shelf[LOCATELINE], level[LOCATELINE], bin[LOCATELINE];

    // Get part name
    printf("Enter Part Name: ");
    fgets(buffer, MAXIMUMLENGTH, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0 || strlen(buffer) > 50) {
        printf("Invalid part name.\n");
        return *size;
    }
    strcpy_s(newPart.PartName, sizeof(newPart.PartName), buffer);

    // Get part number
    printf("Enter Part Number: ");
    fgets(buffer, MAXIMUMLENGTH, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) == 0 || strlen(buffer) > 50) {
        printf("Invalid part number.\n");
        return *size;
    }
    strcpy_s(newPart.PartNumber, sizeof(newPart.PartNumber), buffer);

    // Get part location with validation
    while (1) {
        printf("Enter Part Location (A###-S###-L###-B###): ");
        fgets(buffer, MAXIMUMLENGTH, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        int parsed = sscanf_s(buffer, "%9[^-]-%9[^-]-%9[^-]-%9s",
            aisle, LOCATELINE,
            shelf, LOCATELINE,
            level, LOCATELINE,
            bin, LOCATELINE);

        if (parsed == 4) {
            strcpy_s(newPart.PartLocate, sizeof(newPart.PartLocate), buffer);
            break;
        }
        printf("Invalid format. Please use A###-S###-L###-B### format.\n");
    }

    // Get part cost
    printf("Enter Part Cost: ");
    if (scanf_s("%f", &newPart.PartCost) != 1 || newPart.PartCost <= 0.0f) {
        printf("Invalid cost.\n");
        while (getchar() != '\n');
        return *size;
    }
    while (getchar() != '\n');

    // Get quantity
    printf("Enter Quantity On Hand: ");
    if (scanf_s("%d", &newPart.QuantityOnHand) != 1 || newPart.QuantityOnHand < 0) {
        printf("Invalid quantity.\n");
        while (getchar() != '\n');
        return *size;
    }
    while (getchar() != '\n');

    // Set status based on quantity
    if (newPart.QuantityOnHand > 100) {
        newPart.PartStatus = 0;
    }
    else {
        newPart.PartStatus = 99;
    }

    // Get part ID
    printf("Enter Part ID: ");
    if (scanf_s("%d", &newPart.PartID) != 1 || newPart.PartID <= 0) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return *size;
    }
    while (getchar() != '\n');

    // Check for duplicate ID
    for (int i = 0; i < *size; i++) {
        if (parts[i].PartID == newPart.PartID) {
            printf("Part with ID %d already exists.\n", newPart.PartID);
            return *size;
        }
    }

    // Add new part to inventory
    parts[*size] = newPart;
    (*size)++;
    printf("Part added successfully with ID %d\n", newPart.PartID);

    char logMsg[256];
    sprintf_s(logMsg, sizeof(logMsg), "New part added: ID %d, Name %s", newPart.PartID, newPart.PartName);
    logMessage(logMsg);

    return *size;
}

//
// FUNCTION    : UpdateInventoryforPart
// DESCRIPTION : Updates inventory quantity for specified part
// PARAMETERS  :
//      Parts part[] : Array of part records
//      int* size    : Pointer to current inventory count
// RETURNS     : void
//
void UpdateInventoryforPart(Parts part[], int* size) {
    if (*size == 0) {
        printf("No parts in inventory.\n");
        return;
    }

    int id;
    char buffer[100];

    printf("Enter Part ID to update: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error reading input.\n");
        return;
    }

    if (sscanf_s(buffer, "%d", &id) != 1) {
        printf("Invalid Part ID.\n");
        return;
    }

    int found = -1;
    for (int i = 0; i < *size; i++) {
        if (part[i].PartID == id) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Part with ID %d not found.\n", id);
        return;
    }

    printf("Current quantity: %d\n", part[found].QuantityOnHand);
    printf("Enter new quantity (blank to skip): ");

    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error reading input.\n");
        return;
    }

    if (buffer[0] == '\n') {
        printf("Update skipped.\n");
        return;
    }

    int quantity;
    if (sscanf_s(buffer, "%d", &quantity) != 1 || quantity < 0) {
        printf("Invalid quantity. Must be 0 or greater.\n");
        return;
    }

    part[found].QuantityOnHand = quantity;

    // Update status based on new quantity
    if (quantity > 100) {
        part[found].PartStatus = 0;
    }
    else if (quantity > 0) {
        part[found].PartStatus = 99;
    }
    else {
        part[found].PartStatus = -quantity;
    }

    printf("Inventory updated successfully.\n");

    char logMsg[256];
    sprintf_s(logMsg, sizeof(logMsg), "Part %d inventory updated to %d", id, quantity);
    logMessage(logMsg);
}

//
// FUNCTION    : SaveToFile
// DESCRIPTION : Saves all parts to file in pipe-delimited format
// PARAMETERS  :
//      const char* filename : Name of file to save to
//      Parts part[]         : Array of part records
//      int* size            : Pointer to current inventory count
// RETURNS     : void
//
void SaveToFile(const char* filename, Parts part[], int* size) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "w");
    if (err != 0 || file == NULL) {
        printf("Error saving parts\n");
        return;
    }

    for (int i = 0; i < *size; i++) {
        fprintf(file, "%s|%s|%s|%.2f|%d|%d|%d\n",
            part[i].PartName,
            part[i].PartNumber,
            part[i].PartLocate,
            part[i].PartCost,
            part[i].QuantityOnHand,
            part[i].PartStatus,
            part[i].PartID);
    }

    fclose(file);
    printf("Saved %d parts to %s\n", *size, filename);
    logMessage("Parts database saved");
}

//
// FUNCTION    : loadfromfile
// DESCRIPTION : Loads parts from file with validation
// PARAMETERS  :
//      const char* filename : Name of file to load from
//      Parts part[]         : Array to store loaded parts
//      int* size            : Pointer to current inventory count
// RETURNS     : void
//
void loadfromfile(const char* filename, Parts part[], int* size) {
    FILE* file;
    errno_t err = fopen_s(&file, filename, "r");
    if (err != 0 || file == NULL) {
        printf("Error loading parts\n");
        return;
    }

    char line[MAXLINE];
    *size = 0;

    while (fgets(line, MAXLINE, file) != NULL && *size < MAXPARTSIZE) {
        line[strcspn(line, "\n")] = '\0';

        Parts p;
        char* context = NULL;
        char* fields[7] = { NULL };
        int fieldCount = 0;

        char* token = strtok_s(line, "|", &context);
        while (token && fieldCount < 7) {
            fields[fieldCount++] = token;
            token = strtok_s(NULL, "|", &context);
        }

        if (fieldCount != 7) continue;

        if (strlen(fields[0]) == 0 || strlen(fields[0]) > 50) continue;
        strcpy_s(p.PartName, sizeof(p.PartName), fields[0]);

        if (strlen(fields[1]) == 0 || strlen(fields[1]) > 50) continue;
        strcpy_s(p.PartNumber, sizeof(p.PartNumber), fields[1]);

        if (strlen(fields[2]) == 0 || strlen(fields[2]) > 50) continue;
        strcpy_s(p.PartLocate, sizeof(p.PartLocate), fields[2]);

        if (sscanf_s(fields[3], "%f", &p.PartCost) != 1 || p.PartCost <= 0.0f) continue;

        if (sscanf_s(fields[4], "%d", &p.QuantityOnHand) != 1 || p.QuantityOnHand < 0) continue;

        if (sscanf_s(fields[5], "%d", &p.PartStatus) != 1) continue;

        if (sscanf_s(fields[6], "%d", &p.PartID) != 1 || p.PartID <= 0) continue;

        part[*size] = p;
        (*size)++;
    }

    fclose(file);
    printf("Loaded %d parts from %s\n", *size, filename);
    logMessage("Parts database loaded");
}

//
// FUNCTION    : handlePartsMenu
// DESCRIPTION : Main parts management menu interface
// PARAMETERS  :
//      Parts parts[]    : Array of part records
//      int* partsCount  : Pointer to current inventory count
// RETURNS     : void
//
void handlePartsMenu(Parts parts[], int* partsCount) {
    int choice;
    char buffer[100];

    do {
        partsSubMenu();

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Error reading input.\n");
            continue;
        }

        if (sscanf_s(buffer, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
        case 1: ListallParts(parts, partsCount); break;
        case 2: SearchforPart(parts, partsCount); break;
        case 3: AddPart(parts, partsCount); break;
        case 4: UpdateInventoryforPart(parts, partsCount); break;
        case 5: loadfromfile("parts.db", parts, partsCount); break;
        case 6: SaveToFile("parts.db", parts, partsCount); break;
        case 7: return;
        default: printf("Invalid option. Try again.\n");
        }
    } while (1);
}