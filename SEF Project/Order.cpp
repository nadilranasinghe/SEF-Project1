#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdbool.h>
#include<time.h>
#include "System.cpp"

#define MAX_PARTS 50
#define MAX_CUSTOMERS 50
#define MAX_ORDERS 30
#define LENGTH_OF_DATE 11

#define STATUS_PLACED 0
#define STATUS_PROCESSING 1
#define STATUS_FULFILLED 2
#define STATUS_SHIPPED 3
#define STATUS_DELIVERED 4
#define STATUS_CANCELLED 5
#define STAUS_BACKORDERED 6

typedef struct {
	int PartID;
	int NumberOfParts;

}OrderItem;

typedef struct {
	int PartID;
	char PartName[50];
	float PartCost;
	int QuantityInStock;
}Part;


Order orders[1000];
int orderCount = 0;
int dailySequence = 0;
char lastOrderDate[LENGTH_OF_DATE] = "";



typedef struct {
	long OrderID;
	char OrderDate[LENGTH_OF_DATE];
	int OrderStatus;
	int CustomerID;
	float OrderTotal;
	int DistinctParts;
	int TotalParts;
	OrderItem Items[MAX_PARTS];
}Order;





long generateOrderID();
bool validateDate(const char* date);
void createNewOrder();
void displayOrderDetails(long orderID);
void updateOrderStatus(long orderID, int newStatus);
void listAllOrders();
void processEndOfDayOrders();
void loadOrderFromFile();
void saveOrderToFile();





void getCurrentDate(char* dateStr) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	snprintf(dateStr, LENGTH_OF_DATE, "%04d-&02d-%02d", tm.tm_year + 1990, tm.tm_mon + 1, tm.tm_mday);
}

long generateOrderID() {
	char currentDate[LENGTH_OF_DATE];
	getCurrentDate(currentDate);

	if (strcmp(lastOrderDate, currentDate) != 0) {
		dailySequence = 0;
		strcpy(lastOrderDate, currentDate);
	}

	dailySequence++;

	int year, month, day;
	sscanf(currentDate, "%d-%d-%d", &year, &month, &day);

	return (long)year * 1000000 + month * 1000000 + day * 10000 + dailySequence;

}
bool validateDate(const char* date) {
	if (strlen(date) != 10) return false;
	if (date[4] != '-' || date[7] != '-') return false;

	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 7) continue;
		if (date[i] < '0' || date[i] > '9') return false;
	}

	int year, month, day;
	sscanf(date, "%d-%d-%d", &year, &month, &day);

	if (year < 2000 || year > 2100) return false;
	if (month < 1 || month > 12) return false;
	if (day < 1 || day > 31) return false;

	return true;
}
void createNewOrder() {
	if (orderCount >= 1000) {
		printf("Maximum order capacity is reached\n");
		return;
	}
	Order newOrder;

	newOrder.OrderID = generateOrderID();


	getCurrentDate(newOrder.OrderDate);

	newOrder.OrderStatus = STATUS_PLACED;

	printf("Enter customer ID: ");
	scanf("%d", &newOrder.CustomerID);
	if (!validateCustomer(newOrder.CustomerID)) {
		printf("Invalid customer ID\n");
		return;
	}

	printf("Enter number of Distinct Parts (1-%d)", MAX_PARTS);
	scanf("%d", &newOrder.DistinctParts);

	if (newOrder.DistinctParts < 1 || newOrder.DistinctParts > MAX_PARTS) {
		printf("Invalid number of distinct Parts\n");
		return;
	}

	newOrder.OrderTotal = 0.0f;
	newOrder.TotalParts = 0;

	for (int i = 0; i < newOrder.DistinctParts; i++) {
		printf("\n Part #%d: \n", i + 1);
		printf("Enter Part ID: ");
		scanf("%d", &newOrder.Items[i].PartID);

		if (!validatePart(newOrder.Items[i].PartID)) {
			printf("Invalid Part ID\n");
			return;
		}
		printf("Enter Quantity: ");
		scanf("%d", &newOrder.Items[i].NumberOfParts);

		if (newOrder.Items[i].NumberOfParts < 1) {
			printf("Quantity must be more than 1\n");
			return;

		}

		newOrder.TotalParts += newOrder.Items[i].NumberOfParts;
		newOrder.OrderTotal += newOrder.Items[i].NumberOfParts * getPartPrice(newOrder.Items[i].PartID);
	}
	orders[orderCount++] = newOrder;

	printf("\nOrder Created Successfully!\n");
	printf("Order ID: %1d\n", newOrder.OrderID);
	printf("Order Total: $%.2f\n", newOrder.OrderTotal);

////}

void displayOrderDetails(long orderID) {
	for (int i = 0; i < orderCount; i++) {
		if (orders[i].OrderID == orderID) {
			printf("\nOrder Details\n");
			printf("--------------------\n");
			printf("Order ID: %1d\n", orders[i].OrderID);
			printf("Order Date: %s\n", orders[i].OrderDate);


			printf("Order Status\n");
			switch (orders[i].OrderStatus) {
			case STATUS_PLACED: printf("Placed"); break;
			case STATUS_PROCESSING: printf("Processing"); break;
			case STATUS_FULFILLED: printf("FullFilled"); break;
			case STATUS_SHIPPED: printf("Shipped"); break;
			case STATUS_DELIVERED: printf("Delivered"); break;
			case STATUS_CANCELLED: printf("Cancelled"); break;
			case STAUS_BACKORDERED:printf("BackOrdered"); break;
			default: printf("Unknown");
			}
			printf("Customer ID: %d\n", orders[i].CustomerID);
			printf("Order Total: $%.2f\n", orders[i].OrderTotal);
			printf("Distinct Parts: %d", orders[i].DistinctParts);
			printf("Total Parts: %d\n", orders[i].TotalParts);

			printf("Order Items: \n");
			for (int j = 0; j < orders[i].DistinctParts; j++) {
				printf(" Part ID: %d, Quantity: %d\n", orders[i].Items[j], orders[i].NumberOfParts);
			}
			return;
		}
	}
	printf("Order Not found\n");
}
void updateOrderStatus(long orderID, int newStatus) {
	if (newStatus < 0 || newStatus > 6) {
		printf("Status Code Not Valid\n");
		return;
	}

	for (int i = 0; i < orderCount; i++) {
		if (orders[i].OrderID == orderID) {
			orders[i].OrderStatus = newStatus;
			printf("Order status updated successfully\n");
			return;

		}
	}

	printf("Order Not found\n");

}

void listAllOrders() {
	if (orderCount == 0) {
		printf("No Orders found\n");
		return;
	}

	printf("\nList All Orders\n");
	printf("--------------------\n");

	for (int i = 0; i < orderCount; i++) {
		printf("%1d | %s ", orders[i].OrderID, orders[i].OrderDate);

		switch (orders[i].OrderStatus) {

		case STATUS_PLACED: printf("Placed    "); break;
		case STATUS_PROCESSING: printf("Processing    "); break;
		case STATUS_FULFILLED: printf("FullFilled    "); break;
		case STATUS_SHIPPED: printf("Shipped    "); break;
		case STATUS_DELIVERED: printf("Delivered    "); break;
		case STATUS_CANCELLED: printf("Cancelled    "); break;
		case STAUS_BACKORDERED:printf("BackOrdered    "); break;
		default: printf("Unknown   ");
			
		}
		printf(" | Customer: %d | Total: $%.2f\n", orders[i].CustomerID, orders[i].OrderTotal);
	}
}

void processEndOfDayOrders() {
	printf("Processing end-of-day orders\n");

}

void loadOrderFromFile() {
	FILE* file = fopen("orders.dat", "rb");
	if (file) {
		fread(&orderCount, sizeof(int), 1, file);
		fread(orders, sizeof(Order), orderCount, file);
		fclose(file);
		printf("Loaded orders from database\n");
	}
	else {
		printf("No existing orders in database file found");
	}

}

void saveOrderToFile() {
	FILE* file = fopen("orders.dat", "wb");
	if (file) {
		fwrite(&orderCount, sizeof(int), 1, file);
		fwrite(orders, sizeof(Order), orderCount, file);
		fclose(file);
		printf("Saved database\n");
	}
	else {
		printf("Cannot save data to Database\n");
	}
}


int main() {
	


	int choice;
	long orderID;
	int newStatus;

	while (1) {
		orderSubMenu();

		switch (choice) {
		case 1:
			listAllOrders();
			break;

		case 2:
			printf("Enter Order ID to search: ");
			if (scanf("1d", &orderID) == 1) {
				displayOrderDetails(orderID);
			}
			else {
				printf("Invalid Order ID\n");
			}
			break;

		case 3:
			createNewOrder();
			break;

		case 4:
			processEndOfDayOrders();
			break;

		case 5:
			loadOrderFromFile();
			break;

		case 6:
			saveOrderToFile();
			break;

		case 7: 
			return;

		default:
			printf("Invalid choice. Try again\n");
		

		}
			
	}


}




