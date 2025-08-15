#include "menu.h"
#include "cargo_list.h"
#include <stdio.h>

int main(void) {
    CargoList cargo_list;
    init_list(&cargo_list);

    printf("Welcome to the Cargo Management System\n");

    while (1) {
        display_menu();
        handle_menu_choice(&cargo_list);
    }

    free_list(&cargo_list);
    return 0;
}