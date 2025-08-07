/*
* FILE          : Order.h
* PROJECT       : PWH Warehouse Management System
* PROGRAMMERS    : Najaf Ali, Che-Ping Chien, Nadil Devnath Ranasinghe, Xinming Xu
* FIRST VERSION : 2025-08-01
* DESCRIPTION   :
*      Header file for order management functionality including:
*      - Order data structures
*      - Order status constants
*      - Function prototypes for order operations
*/

#ifndef ORDER_H
#define ORDER_H

#include "Customer.h"
#include "Part.h"

#define MAX_PARTS_PER_ORDER 50  // Maximum distinct parts per order
#define MAX_ORDERS 100          // Maximum orders in system
#define LENGTH_OF_DATE 11       // Length of date string (YYYY-MM-DD + null)

// Order status constants
#define STATUS_PLACED 0                     // Order placed but not processed
#define STATUS_FULFILLED 1                  // Order successfully fulfilled
#define STATUS_INSUFFICIENT_PARTS 99        // Order failed - not enough inventory
#define STATUS_CREDIT_LIMIT_EXCEEDED 500    // Order failed - credit limit exceeded

// Structure for order line items
typedef struct {
    int PartID;             // ID of ordered part
    int NumberOfParts;      // Quantity ordered
} OrderItem;

// Main order structure
typedef struct {
    long OrderID;                       // Unique order identifier
    char OrderDate[LENGTH_OF_DATE];     // Date order was placed
    int OrderStatus;                    // Current status of order
    int CustomerID;                     // ID of ordering customer
    float OrderTotal;                   // Total value of order
    int DistinctParts;                  // Number of different parts in order
    int TotalParts;                     // Total quantity of all parts
    OrderItem Items[MAX_PARTS_PER_ORDER]; // Array of order items
} Order;

// Function prototypes
long generateOrderID();     // Generate unique order ID
bool validateDate(const char* date);    // Validate date format
void createNewOrder(Order orders[], int* orderCount, Customer customers[], int customerCount, Parts parts[], int partsCount);
void displayOrderDetails(long orderID, Order orders[], int orderCount);
void updateOrderStatus(long orderID, int newStatus, Order orders[], int orderCount);
void listAllOrders(Order orders[], int orderCount);
void processEndOfDayOrders(Order orders[], int* orderCount, Customer customers[], int customerCount, Parts parts[], int partsCount);
void loadOrderFromFile(Order orders[], int* orderCount);
void saveOrderToFile(Order orders[], int orderCount);
void handleOrdersMenu(Order orders[], int* orderCount, Customer customers[], int customerCount, Parts parts[], int partsCount);

#endif