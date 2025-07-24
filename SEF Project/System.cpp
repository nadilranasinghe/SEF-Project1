#include<stdio.h>


void mainMenu();
void partsSubMenu();
void customerSubMenu();
void orderSubMenu();

void mainMenu() {
	printf("----MAIN--MENU------\n");
	printf("1. Parts\n");
	printf("2. Customers\n");
	printf("3. Orders\n");
}

void partsSubMenu() {
	printf("---PARTS-SUB-MENU---\n");
	printf("1. List All Parts\n");
	printf("2. Search for Part\n");
	printf("3. Enter New Part\n");
	printf("4. Update Inventory for Part\n");
	printf("5. Load Parts Database\n");
	printf("6. Save Parts Database\n");
	printf("7. Return to Main Menu\n");

}

void customerSubMenu() {
	printf("---Customer-SUB-MENU---\n");
	printf("1. List all Customers\n");
	printf("2. Search for Customer\n");
	printf("3. Enter New Customer\n");
	printf("4. Update Customer Information\n");
	printf("5. Customer in Bad Credit Standing\n");
	printf("6. Load Customer Database\n");
	printf("7. Save Customer Database\n");
	printf("8. Return to Main Menu\n");
}

void orderSubMenu() {
	printf("---ORDER-SUB-MENU---\n");
	printf("1. List All Orders\n");
	printf("2. Search for Order\n");
	printf("3. Enter New Order\n");
	printf("4. Process Orders at End of Day\n");
	printf("5. Load Order Database\n");
	printf("6. Save Order Database\n");
	printf("7. Return to Main Menu\n");
}

int main() {
	mainMenu();
	partsSubMenu();
	customerSubMenu();
	orderSubMenu();
}