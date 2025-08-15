#include "cargo_list.h"
#include <stdlib.h>
#include <stdio.h>

void init_list(CargoList* list) {
    list->head = NULL;
    list->size = 0;
}

void free_list(CargoList* list) {
    ListNode* current = list->head;
    while (current != NULL) {
        ListNode* temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->size = 0;
}

void add_item(CargoList* list, CargoItem item) {
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->item = item;
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

int remove_item(CargoList* list, int id) {
    ListNode* current = list->head;
    ListNode* prev = NULL;

    while (current != NULL) {
        if (current->item.id == id) {
            if (prev == NULL) {
                list->head = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current);
            list->size--;
            return 1; // Success
        }
        prev = current;
        current = current->next;
    }
    return 0; // Not found
}

int update_item(CargoList* list, int id, double new_price, double new_weight, int new_quantity) {
    CargoItem* item = find_item(list, id);
    if (item != NULL) {
        item->price = new_price;
        item->weight = new_weight;
        item->quantity = new_quantity;
        return 1; // Success
    }
    return 0; // Not found
}

void display_all_items(const CargoList* list) {
    if (list->size == 0) {
        printf("No cargo items in the system.\n");
        return;
    }

    printf("All Cargo Items:\n");
    ListNode* current = list->head;
    while (current != NULL) {
        print_cargo_item(&current->item);
        current = current->next;
    }
}

double calculate_total_weight(const CargoList* list) {
    double total = 0.0;
    ListNode* current = list->head;
    while (current != NULL) {
        total += current->item.weight * current->item.quantity;
        current = current->next;
    }
    return total;
}

double calculate_total_cost(const CargoList* list) {
    double total = 0.0;
    ListNode* current = list->head;
    while (current != NULL) {
        total += current->item.price * current->item.quantity;
        current = current->next;
    }
    return total;
}

double calculate_transportation_fee(const CargoList* list) {
    return calculate_total_weight(list) * 0.5;
}

CargoItem* find_item(const CargoList* list, int id) {
    ListNode* current = list->head;
    while (current != NULL) {
        if (current->item.id == id) {
            return &current->item;
        }
        current = current->next;
    }
    return NULL;
}