#include "cargo_item.h"
#include <stdio.h>

void print_cargo_item(const CargoItem* item) {
    printf("ID: %d, Price: $%.2f, Weight: %.2f kg, Quantity: %d\n",
        item->id, item->price, item->weight, item->quantity);
}

CargoItem create_cargo_item(int id, double price, double weight, int quantity) {
    CargoItem new_item;
    new_item.id = id;
    new_item.price = price;
    new_item.weight = weight;
    new_item.quantity = quantity;
    return new_item;
}