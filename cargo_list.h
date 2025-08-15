#ifndef CARGO_LIST_H
#define CARGO_LIST_H

#include "cargo_item.h"

typedef struct ListNode {
    CargoItem item;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
    int size;
} CargoList;

void init_list(CargoList* list);
void free_list(CargoList* list);
void add_item(CargoList* list, CargoItem item);
int remove_item(CargoList* list, int id);
int update_item(CargoList* list, int id, double new_price, double new_weight, int new_quantity);
void display_all_items(const CargoList* list);
double calculate_total_weight(const CargoList* list);
double calculate_total_cost(const CargoList* list);
double calculate_transportation_fee(const CargoList* list);
CargoItem* find_item(const CargoList* list, int id);

#endif