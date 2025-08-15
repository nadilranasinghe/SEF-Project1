#ifndef CARGO_ITEM_H
#define CARGO_ITEM_H

typedef struct {
    int id;
    double price;
    double weight;
    int quantity;
} CargoItem;

void print_cargo_item(const CargoItem* item);
CargoItem create_cargo_item(int id, double price, double weight, int quantity);

#endif