#include "menu.h"
#include "cargo_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_menu() {
    printf("\nCargo Management System\n");
    printf("1. Add new cargo item\n");
    printf("2. Update cargo item\n");
    printf("3. Remove cargo item\n");
    printf("4. Display all cargo items\n");
    printf("5. Calculate total weight\n");
    printf("6. Calculate transportation fee\n");
    printf("7. Calculate total shipment cost\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

int read_input_with_skip(char* buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return 0; 
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }

    return len; 
}

void handle_menu_choice(CargoList* list) {
    int choice;
    scanf_s("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {
    case 1: { 
        int id, quantity;
        double price, weight;
        char input[100];
        int valid_input;

        do {
            printf("Enter item ID (must be positive integer): ");
            valid_input = 1;

            if (read_input_with_skip(input, sizeof(input)) == 0) {
                printf("Error: ID cannot be empty.\n");
                valid_input = 0;
                continue;
            }

            if (sscanf_s(input, "%d", &id) != 1 || id <= 0) {
                printf("Error: Invalid ID. Must be a positive integer.\n");
                valid_input = 0;
                continue;
            }

            if (find_item(list, id) != NULL) {
                printf("Error: Item with ID %d already exists.\n", id);
                valid_input = 0;
            }
        } while (!valid_input);

        do {
            printf("Enter price (must be positive number): ");
            valid_input = 1;

            if (read_input_with_skip(input, sizeof(input)) == 0) {
                printf("Error: Price cannot be empty.\n");
                valid_input = 0;
                continue;
            }

            if (sscanf_s(input, "%lf", &price) != 1 || price <= 0) {
                printf("Error: Invalid price. Must be a positive number.\n");
                valid_input = 0;
            }
        } while (!valid_input);

        do {
            printf("Enter weight in kg (must be positive number): ");
            valid_input = 1;

            if (read_input_with_skip(input, sizeof(input)) == 0) {
                printf("Error: Weight cannot be empty.\n");
                valid_input = 0;
                continue;
            }

            if (sscanf_s(input, "%lf", &weight) != 1 || weight <= 0) {
                printf("Error: Invalid weight. Must be a positive number.\n");
                valid_input = 0;
            }
        } while (!valid_input);

        do {
            printf("Enter quantity (must be positive whole number): ");
            valid_input = 1;

            if (read_input_with_skip(input, sizeof(input)) == 0) {
                printf("Error: Quantity cannot be empty.\n");
                valid_input = 0;
                continue;
            }

            if (strchr(input, '.') != NULL) {
                printf("Error: Quantity must be a whole number.\n");
                valid_input = 0;
                continue;
            }

            if (sscanf_s(input, "%d", &quantity) != 1 || quantity <= 0) {
                printf("Error: Invalid quantity. Must be a positive whole number.\n");
                valid_input = 0;
            }
        } while (!valid_input);

        CargoItem new_item = create_cargo_item(id, price, weight, quantity);
        add_item(list, new_item);
        printf("Item added successfully.\n");
        break;
    }
     
    case 2: {
        int id, new_quantity;
        double new_price, new_weight;
        char input[100];
        int valid_input;

        do {
            printf("Enter item ID to update: ");
            valid_input = 1;

            if (read_input_with_skip(input, sizeof(input)) == 0) {
                printf("Error: ID cannot be empty.\n");
                valid_input = 0;
                continue;
            }

            if (sscanf_s(input, "%d", &id) != 1) {
                printf("Error: Invalid ID format.\n");
                valid_input = 0;
            }
        } while (!valid_input);

        CargoItem* item = find_item(list, id);
        if (item == NULL) {
            printf("Error: Item with ID %d not found.\n", id);
            break;
        }

        printf("Current details: ");
        print_cargo_item(item);

        do {
            printf("Enter new price (press Enter to keep current: $%.2f): ", item->price);
            valid_input = 1;

            if (read_input_with_skip(input, sizeof(input)) == 0) {
                break; 
            }

            if (sscanf_s(input, "%lf", &new_price) != 1 || new_price <= 0) {
                printf("Error: Price must be a positive number.\n");
                valid_input = 0;
            }
            else {
                item->price = new_price;
            }
        } while (!valid_input);

        do {
            printf("Enter new weight (press Enter to keep current: %.2f kg): ", item->weight);
            valid_input = 1;

            if (read_input_with_skip(input, sizeof(input)) == 0) {
                break; 
            }

            if (sscanf_s(input, "%lf", &new_weight) != 1 || new_weight <= 0) {
                printf("Error: Weight must be a positive number.\n");
                valid_input = 0;
            }
            else {
                item->weight = new_weight;
            }
        } while (!valid_input);

        do {
            printf("Enter new quantity (press Enter to keep current: %d): ", item->quantity);
            valid_input = 1;

            if (read_input_with_skip(input, sizeof(input)) == 0) {
                break; 
            }

            if (strchr(input, '.') != NULL) {
                printf("Error: Quantity must be a whole number.\n");
                valid_input = 0;
                continue;
            }

            if (sscanf_s(input, "%d", &new_quantity) != 1 || new_quantity <= 0) {
                printf("Error: Quantity must be a positive whole number.\n");
                valid_input = 0;
            }
            else {
                item->quantity = new_quantity;
            }
        } while (!valid_input);

        printf("Item updated successfully.\n");
        break;
    }

    case 3: { 
        int id;
        printf("Enter item ID to remove: ");
        scanf_s("%d", &id);
        while (getchar() != '\n'); 

        if (remove_item(list, id)) {
            printf("Item removed successfully.\n");
        }
        else {
            printf("Error: Item with ID %d not found.\n", id);
        }
        break;
    }

    case 4: 
        display_all_items(list);
        break;

    case 5: 
        printf("Total weight: %.2f kg\n", calculate_total_weight(list));
        break;

    case 6: 
        printf("Transportation fee: $%.2f\n", calculate_transportation_fee(list));
        break;

    case 7: 
        printf("Total shipment cost: $%.2f\n", calculate_total_cost(list));
        printf("Transportation fee: $%.2f\n", calculate_transportation_fee(list));
        printf("Grand total: $%.2f\n",
            calculate_total_cost(list) + calculate_transportation_fee(list));
        break;

    case 8: 
        printf("Goodbye, exiting program...\n");
        exit(0);
        break;

    default:
        printf("Invalid choice. Please try again.\n");
    }
}