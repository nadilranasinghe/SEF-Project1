/*
* FILE          : Order.cpp
* PROJECT       : PWH Warehouse Management System
* PROGRAMMERS   : Najaf Ali, Che-Ping Chien, Nadil Devnath Ranasinghe, Xinming Xu
* FIRST VERSION : 2025-08-01
* DESCRIPTION   :
*      Implementation of order management functions including:
*      - Order creation and processing
*      - Order status management
*      - End-of-day batch processing
*      - File I/O for order records
*/

#include "Order.h"
#include "System.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

//
// FUNCTION    : getCurrentDate
// DESCRIPTION : Gets current date in YYYY-MM-DD format
// PARAMETERS  :
//      char* dateStr : Buffer to store date string (must be at least LENGTH_OF_DATE)
// RETURNS     : void
//
void getCurrentDate(char* dateStr) {
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    snprintf(dateStr, LENGTH_OF_DATE, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

//
// FUNCTION    : generateOrderID
// DESCRIPTION : Generates unique order ID based on date and sequence
// PARAMETERS  : None
// RETURNS     : long - Generated order ID
//
long generateOrderID() {
    char currentDate[LENGTH_OF_DATE];
    getCurrentDate(currentDate);
    static int dailySequence = 0;
    static char lastDate[LENGTH_OF_DATE] = "";

    if (strcmp(currentDate, lastDate) != 0) {
        dailySequence = 0;
        strcpy_s(lastDate, sizeof(lastDate), currentDate);
    }

    dailySequence++;

    int year, month, day;
    sscanf_s(currentDate, "%d-%d-%d", &year, &month, &day);

    return (long)year * 1000000 + month * 10000 + day * 100 + dailySequence;
}

//
// FUNCTION    : validateDate
// DESCRIPTION : Validates date string format (YYYY-MM-DD)
// PARAMETERS  :
//      const char* date : Date string to validate
// RETURNS     : bool - true if valid, false otherwise
//
bool validateDate(const char* date) {
    if (strlen(date) != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    int year, month, day;
    if (sscanf_s(date, "%d-%d-%d", &year, &month, &day) != 3)
        return false;

    if (year < 2000 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    return true;
}

//
// FUNCTION    : validateCustomer
// DESCRIPTION : Validates that customer ID exists in system
// PARAMETERS  :
//      int customerID      : Customer ID to validate
//      Customer customers[]: Array of customer records
//      int customerCount   : Number of customers
// RETURNS     : bool - true if valid, false otherwise
//
bool validateCustomer(int customerID, Customer customers[], int customerCount) {
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].customerID == customerID) {
            return true;
        }
    }
    return false;
}

//
// FUNCTION    : validatePart
// DESCRIPTION : Validates that part ID exists in inventory
// PARAMETERS  :
//      int partID      : Part ID to validate
//      Parts parts[]   : Array of part records
//      int partsCount  : Number of parts in inventory
// RETURNS     : bool - true if valid, false otherwise
//
bool validatePart(int partID, Parts parts[], int partsCount) {
    for (int i = 0; i < partsCount; i++) {
        if (parts[i].PartID == partID) {
            return true;
        }
    }
    return false;
}

//
// FUNCTION    : getPartPrice
// DESCRIPTION : Retrieves price for specified part ID
// PARAMETERS  :
//      int partID      : Part ID to look up
//      Parts parts[]   : Array of part records
//      int partsCount  : Number of parts in inventory
// RETURNS     : float - Price of part, 0.0 if not found
//
float getPartPrice(int partID, Parts parts[], int partsCount) {
    for (int i = 0; i < partsCount; i++) {
        if (parts[i].PartID == partID) {
            return parts[i].PartCost;
        }
    }
    return 0.0f;
}

//
// FUNCTION    : createNewOrder
// DESCRIPTION : Creates new order with user input and validation
// PARAMETERS  :
//      Order orders[]      : Array of order records
//      int* orderCount     : Pointer to current order count
//      Customer customers[]: Array of customer records
//      int customerCount   : Number of customers
//      Parts parts[]       : Array of part records
//      int partsCount      : Number of parts in inventory
// RETURNS     : void
//
void createNewOrder(Order orders[], int* orderCount, Customer customers[], int customerCount, Parts parts[], int partsCount) {
    if (*orderCount >= MAX_ORDERS) {
        printf("Maximum order capacity reached.\n");
        return;
    }

    Order newOrder;
    newOrder.OrderID = generateOrderID();
    getCurrentDate(newOrder.OrderDate);
    newOrder.OrderStatus = STATUS_PLACED;

    printf("Enter Customer ID: ");
    if (scanf_s("%d", &newOrder.CustomerID) != 1) {
        printf("Invalid customer ID.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (!validateCustomer(newOrder.CustomerID, customers, customerCount)) {
        printf("Customer not found.\n");
        return;
    }

    printf("Enter number of distinct parts (1-%d): ", MAX_PARTS_PER_ORDER);
    if (scanf_s("%d", &newOrder.DistinctParts) != 1 ||
        newOrder.DistinctParts < 1 || newOrder.DistinctParts > MAX_PARTS_PER_ORDER) {
        printf("Invalid number of parts.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    newOrder.OrderTotal = 0.0f;
    newOrder.TotalParts = 0;

    for (int i = 0; i < newOrder.DistinctParts; i++) {
        printf("\nPart #%d:\n", i + 1);

        printf("Enter Part ID: ");
        if (scanf_s("%d", &newOrder.Items[i].PartID) != 1) {
            printf("Invalid part ID.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');

        if (!validatePart(newOrder.Items[i].PartID, parts, partsCount)) {
            printf("Part not found.\n");
            return;
        }

        printf("Enter Quantity: ");
        if (scanf_s("%d", &newOrder.Items[i].NumberOfParts) != 1 ||
            newOrder.Items[i].NumberOfParts < 1) {
            printf("Invalid quantity.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');

        float partPrice = getPartPrice(newOrder.Items[i].PartID, parts, partsCount);
        newOrder.OrderTotal += partPrice * newOrder.Items[i].NumberOfParts;
        newOrder.TotalParts += newOrder.Items[i].NumberOfParts;
    }

    orders[*orderCount] = newOrder;
    (*orderCount)++;
    printf("\nOrder created successfully!\n");
    printf("Order ID: %ld\n", newOrder.OrderID);
    printf("Order Total: $%.2f\n", newOrder.OrderTotal);

    char logMsg[256];
    sprintf_s(logMsg, sizeof(logMsg), "New order created: ID %ld, Customer %d, Total $%.2f",
        newOrder.OrderID, newOrder.CustomerID, newOrder.OrderTotal);
    logMessage(logMsg);
}

//
// FUNCTION    : displayOrderDetails
// DESCRIPTION : Displays detailed information for specific order
// PARAMETERS  :
//      long orderID    : ID of order to display
//      Order orders[]  : Array of order records
//      int orderCount  : Number of orders
// RETURNS     : void
//
void displayOrderDetails(long orderID, Order orders[], int orderCount) {
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].OrderID == orderID) {
            printf("\nOrder Details\n");
            printf("----------------------------\n");
            printf("Order ID: %ld\n", orders[i].OrderID);
            printf("Date: %s\n", orders[i].OrderDate);

            printf("Status: ");
            switch (orders[i].OrderStatus) {
            case STATUS_PLACED: printf("Placed"); break;
            case STATUS_FULFILLED: printf("Fulfilled"); break;
            case STATUS_INSUFFICIENT_PARTS: printf("Insufficient Parts"); break;
            case STATUS_CREDIT_LIMIT_EXCEEDED: printf("Credit Limit Exceeded"); break;
            default: printf("Unknown");
            }
            printf("\n");

            printf("Customer ID: %d\n", orders[i].CustomerID);
            printf("Total Parts: %d\n", orders[i].TotalParts);
            printf("Distinct Parts: %d\n", orders[i].DistinctParts);
            printf("Order Total: $%.2f\n", orders[i].OrderTotal);

            printf("\nOrder Items:\n");
            for (int j = 0; j < orders[i].DistinctParts; j++) {
                printf("  Part ID: %d, Quantity: %d\n",
                    orders[i].Items[j].PartID,
                    orders[i].Items[j].NumberOfParts);
            }

            return;
        }
    }

    printf("Order not found.\n");
}

//
// FUNCTION    : updateOrderStatus
// DESCRIPTION : Updates status of specified order
// PARAMETERS  :
//      long orderID    : ID of order to update
//      int newStatus   : New status code
//      Order orders[]  : Array of order records
//      int orderCount  : Number of orders
// RETURNS     : void
//
void updateOrderStatus(long orderID, int newStatus, Order orders[], int orderCount) {
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].OrderID == orderID) {
            orders[i].OrderStatus = newStatus;
            printf("Order status updated.\n");

            char logMsg[256];
            sprintf_s(logMsg, sizeof(logMsg), "Order %ld status updated to %d", orderID, newStatus);
            logMessage(logMsg);

            return;
        }
    }

    printf("Order not found.\n");
}

//
// FUNCTION    : listAllOrders
// DESCRIPTION : Lists all orders in system with basic information
// PARAMETERS  :
//      Order orders[]  : Array of order records
//      int orderCount  : Number of orders
// RETURNS     : void
//
void listAllOrders(Order orders[], int orderCount) {
    if (orderCount == 0) {
        printf("No orders in system.\n");
        return;
    }

    printf("\nAll Orders\n");
    printf("----------------------------\n");

    for (int i = 0; i < orderCount; i++) {
        printf("Order ID: %ld\n", orders[i].OrderID);
        printf("Date: %s\n", orders[i].OrderDate);
        printf("Customer ID: %d\n", orders[i].CustomerID);

        printf("Status: ");
        switch (orders[i].OrderStatus) {
        case STATUS_PLACED: printf("Placed"); break;
        case STATUS_FULFILLED: printf("Fulfilled"); break;
        case STATUS_INSUFFICIENT_PARTS: printf("Insufficient Parts"); break;
        case STATUS_CREDIT_LIMIT_EXCEEDED: printf("Credit Limit Exceeded"); break;
        default: printf("Unknown");
        }
        printf("\n");

        printf("Total: $%.2f\n", orders[i].OrderTotal);
        printf("----------------------------\n");
    }
}

//
// FUNCTION    : processEndOfDayOrders
// DESCRIPTION : Processes all pending orders with validation checks
// PARAMETERS  :
//      Order orders[]      : Array of order records
//      int* orderCount     : Pointer to current order count
//      Customer customers[]: Array of customer records
//      int customerCount   : Number of customers
//      Parts parts[]       : Array of part records
//      int partsCount      : Number of parts in inventory
// RETURNS     : void
//
void processEndOfDayOrders(Order orders[], int* orderCount, Customer customers[], int customerCount, Parts parts[], int partsCount) {
    if (*orderCount == 0) {
        printf("No orders to process.\n");
        return;
    }

    printf("\nProcessing orders...\n");
    int processed = 0;
    char logMsg[256];

    // Sort orders by customer join date (oldest customers first)
    for (int i = 0; i < *orderCount - 1; i++) {
        for (int j = 0; j < *orderCount - i - 1; j++) {
            Customer c1, c2;
            for (int k = 0; k < customerCount; k++) {
                if (customers[k].customerID == orders[j].CustomerID) c1 = customers[k];
                if (customers[k].customerID == orders[j + 1].CustomerID) c2 = customers[k];
            }

            if (strcmp(c1.joinDate, c2.joinDate) > 0) {
                Order temp = orders[j];
                orders[j] = orders[j + 1];
                orders[j + 1] = temp;
            }
        }
    }

    // Process each order
    for (int i = 0; i < *orderCount; i++) {
        if (orders[i].OrderStatus != STATUS_PLACED) continue;

        Customer* customer = NULL;
        for (int j = 0; j < customerCount; j++) {
            if (customers[j].customerID == orders[i].CustomerID) {
                customer = &customers[j];
                break;
            }
        }
        if (!customer) continue;

        // Check credit limit
        if (customer->accountBalance + orders[i].OrderTotal > customer->creditLimit) {
            orders[i].OrderStatus = STATUS_CREDIT_LIMIT_EXCEEDED;
            sprintf_s(logMsg, sizeof(logMsg),
                "Order %ld: Credit limit exceeded (Customer %d: Balance $%.2f + Order $%.2f > Limit $%.2f)",
                orders[i].OrderID, customer->customerID,
                customer->accountBalance, orders[i].OrderTotal, customer->creditLimit);
            logMessage(logMsg);
            continue;
        }

        // Check inventory availability
        bool canFulfill = true;
        for (int j = 0; j < orders[i].DistinctParts; j++) {
            OrderItem item = orders[i].Items[j];
            Parts* part = NULL;

            for (int k = 0; k < partsCount; k++) {
                if (parts[k].PartID == item.PartID) {
                    part = &parts[k];
                    break;
                }
            }

            if (!part || part->QuantityOnHand < item.NumberOfParts) {
                canFulfill = false;
                if (part) {
                    int deficit = item.NumberOfParts - part->QuantityOnHand;
                    part->PartStatus = -deficit;

                    sprintf_s(logMsg, sizeof(logMsg),
                        "Insufficient inventory - Part %d: Need %d, Have %d (Deficit %d)",
                        item.PartID, item.NumberOfParts, part->QuantityOnHand, deficit);
                    logMessage(logMsg);
                }
            }
        }

        if (!canFulfill) {
            orders[i].OrderStatus = STATUS_INSUFFICIENT_PARTS;
            sprintf_s(logMsg, sizeof(logMsg),
                "Order %ld: Insufficient parts", orders[i].OrderID);
            logMessage(logMsg);
            continue;
        }

        // Fulfill order
        for (int j = 0; j < orders[i].DistinctParts; j++) {
            OrderItem item = orders[i].Items[j];
            for (int k = 0; k < partsCount; k++) {
                if (parts[k].PartID == item.PartID) {
                    parts[k].QuantityOnHand -= item.NumberOfParts;

                    // Update part status based on new quantity
                    if (parts[k].QuantityOnHand > 100) {
                        parts[k].PartStatus = 0;
                    }
                    else if (parts[k].QuantityOnHand > 0) {
                        parts[k].PartStatus = 99;
                    }
                    else {
                        parts[k].PartStatus = -parts[k].QuantityOnHand;
                    }
                    break;
                }
            }
        }

        // Update customer balance
        customer->accountBalance += orders[i].OrderTotal;
        orders[i].OrderStatus = STATUS_FULFILLED;
        processed++;

        sprintf_s(logMsg, sizeof(logMsg),
            "Order %ld fulfilled - Customer %d, Total $%.2f",
            orders[i].OrderID, customer->customerID, orders[i].OrderTotal);
        logMessage(logMsg);
    }

    printf("\nProcessing complete. %d orders processed.\n", processed);
}

//
// FUNCTION    : loadOrderFromFile
// DESCRIPTION : Loads orders from file with validation
// PARAMETERS  :
//      Order orders[]  : Array to store loaded orders
//      int* orderCount : Pointer to current order count
// RETURNS     : void
//
void loadOrderFromFile(Order orders[], int* orderCount) {
    FILE* file;
    errno_t err = fopen_s(&file, "orders.db", "r");
    if (err != 0 || file == NULL) {
        printf("Error loading orders\n");
        return;
    }

    char line[512];
    *orderCount = 0;

    while (fgets(line, sizeof(line), file) != NULL && *orderCount < MAX_ORDERS) {
        line[strcspn(line, "\r\n")] = '\0';

        Order o;
        char* context = NULL;
        char* fields[6 + MAX_PARTS_PER_ORDER * 2] = { NULL };
        int fieldCount = 0;

        char* token = strtok_s(line, "|", &context);
        while (token && fieldCount < 6 + MAX_PARTS_PER_ORDER * 2) {
            fields[fieldCount++] = token;
            token = strtok_s(NULL, "|", &context);
        }

        if (fieldCount < 6) continue;

        if (sscanf_s(fields[0], "%ld", &o.OrderID) != 1) continue;

        if (strlen(fields[1]) != 10 || !validateDate(fields[1])) continue;
        strcpy_s(o.OrderDate, sizeof(o.OrderDate), fields[1]);

        if (sscanf_s(fields[2], "%d", &o.OrderStatus) != 1) continue;

        if (sscanf_s(fields[3], "%d", &o.CustomerID) != 1 || o.CustomerID <= 0) continue;

        if (sscanf_s(fields[4], "%f", &o.OrderTotal) != 1 || o.OrderTotal <= 0.0f) continue;

        if (sscanf_s(fields[5], "%d", &o.DistinctParts) != 1 || o.DistinctParts < 1) continue;

        o.TotalParts = 0;
        int itemIndex = 0;
        for (int i = 6; i < fieldCount && itemIndex < o.DistinctParts; i += 2) {
            if (sscanf_s(fields[i], "%d", &o.Items[itemIndex].PartID) != 1 || o.Items[itemIndex].PartID <= 0) break;
            if (sscanf_s(fields[i + 1], "%d", &o.Items[itemIndex].NumberOfParts) != 1 || o.Items[itemIndex].NumberOfParts < 1) break;

            o.TotalParts += o.Items[itemIndex].NumberOfParts;
            itemIndex++;
        }

        if (itemIndex != o.DistinctParts) continue;

        orders[*orderCount] = o;
        (*orderCount)++;
    }

    fclose(file);
    printf("Loaded %d orders from orders.db\n", *orderCount);
    logMessage("Order database loaded");
}

//
// FUNCTION    : saveOrderToFile
// DESCRIPTION : Saves all orders to file in pipe-delimited format
// PARAMETERS  :
//      Order orders[]  : Array of order records to save
//      int orderCount  : Number of orders to save
// RETURNS     : void
//
void saveOrderToFile(Order orders[], int orderCount) {
    FILE* file;
    errno_t err = fopen_s(&file, "orders.db", "w");
    if (err != 0 || file == NULL) {
        printf("Error saving orders.\n");
        return;
    }

    for (int i = 0; i < orderCount; i++) {
        fprintf(file, "%ld|%s|%d|%d|%.2f|%d",
            orders[i].OrderID,
            orders[i].OrderDate,
            orders[i].OrderStatus,
            orders[i].CustomerID,
            orders[i].OrderTotal,
            orders[i].DistinctParts);

        for (int j = 0; j < orders[i].DistinctParts; j++) {
            fprintf(file, "|%d|%d",
                orders[i].Items[j].PartID,
                orders[i].Items[j].NumberOfParts);
        }

        fprintf(file, "\n");
    }

    fclose(file);
    printf("Saved %d orders records to orders.db.\n", orderCount);
    logMessage("Order database saved");
}

//
// FUNCTION    : handleOrdersMenu
// DESCRIPTION : Main order management menu interface
// PARAMETERS  :
//      Order orders[]      : Array of order records
//      int* orderCount     : Pointer to current order count
//      Customer customers[]: Array of customer records
//      int customerCount   : Number of customers
//      Parts parts[]       : Array of part records
//      int partsCount      : Number of parts in inventory
// RETURNS     : void
//
void handleOrdersMenu(Order orders[], int* orderCount, Customer customers[], int customerCount, Parts parts[], int partsCount) {
    int choice;
    char buffer[100];

    do {
        orderSubMenu();

        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        if (sscanf_s(buffer, "%d", &choice) != 1) {
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
        case 1: listAllOrders(orders, *orderCount); break;
        case 2: {
            long orderID;
            printf("Enter Order ID: ");
            if (scanf_s("%ld", &orderID) == 1) {
                displayOrderDetails(orderID, orders, *orderCount);
            }
            while (getchar() != '\n');
            break;
        }
        case 3: createNewOrder(orders, orderCount, customers, customerCount, parts, partsCount); break;
        case 4: processEndOfDayOrders(orders, orderCount, customers, customerCount, parts, partsCount); break;
        case 5: loadOrderFromFile(orders, orderCount); break;
        case 6: saveOrderToFile(orders, *orderCount); break;
        case 7: return;
        default: printf("Invalid option.\n");
        }
    } while (1);
}