#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"

// Initializes the inventory database
InventoryDatabase * initInventoryDatabase() {
    InventoryDatabase * invDB = malloc(sizeof(InventoryDatabase));
    if (!invDB) {
        // Handle error: memory allocation failed
        return NULL;
    }
    invDB -> numInventories = 0;
    for (int i = 0; i < MAX_NUM; ++i) {
        invDB -> inventories[i] = NULL;
    }
    return invDB;
}

// Adds an inventory to the database
int loadInventory(InventoryDatabase * invDB, int character_id, int * item_ids, int num_items) {
    if (invDB -> numInventories >= MAX_NUM) {
        // Handle error: database is full
        return -1;
    }
    Inventory * inventory = malloc(sizeof(Inventory));
    if (!inventory) {
        // Handle error: memory allocation failed
        return -1;
    }
    inventory -> character_id = character_id;
    inventory -> items.num_items = num_items < MAX_INVENTORY_ITEMS ? num_items : MAX_INVENTORY_ITEMS;
    inventory -> items.item_ids = malloc(sizeof(int) * (size_t) inventory -> items.num_items);
    // Check if malloc was successful
    if (!inventory -> items.item_ids) {
        // Handle error: memory allocation failed
        free(inventory);
        return -1;
    }
    memcpy(inventory -> items.item_ids, item_ids, sizeof(int) * (size_t) inventory -> items.num_items);

    invDB -> inventories[invDB -> numInventories++] = inventory;
    return 0; // Success
}

// Free the memory allocated for the inventory database
void freeInventoryDatabase(InventoryDatabase * invDB) {
    if (invDB) {
        for (int i = 0; i < invDB -> numInventories; ++i) {
            free(invDB -> inventories[i] -> items.item_ids);
            free(invDB -> inventories[i]);
        }
        free(invDB);
    }
}

// Function to load an InventoryDatabase from a CSV file
int readInventoryFromCSV(InventoryDatabase * db,
    const char * filename) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char * token = strtok(line, ",");
        int character_id = atoi(token);

        token = strtok(NULL, ",");
        int * items;
        int num_items = get_items(token, & items);

        Inventory inv;
        inv.character_id = character_id;
        inv.items.item_ids = items;
        inv.items.num_items = num_items;

        db -> inventories[db -> numInventories] = malloc(sizeof(Inventory));
        *(db -> inventories[db -> numInventories]) = inv;
        db -> numInventories++;
    }

    fclose(file);
    return 0; // Return 0 to indicate success
}

int get_items(char * items_str, int ** items) {
    int num_items = 0;
    char * items_str_copy = strdup(items_str); // Make a copy of the string
    char * token = strtok(items_str_copy, ";");

    // First, count the number of items
    while (token != NULL) {
        num_items++;
        token = strtok(NULL, ";");
    }

    // Allocate memory for the items
    * items = malloc(num_items * sizeof(int));

    // Reset the strtok
    free(items_str_copy);
    items_str_copy = strdup(items_str);
    token = strtok(items_str_copy, ";");

    // Now, populate the items array
    int i = 0;
    while (token != NULL) {
        ( * items)[i] = atoi(token);
        i++;
        token = strtok(NULL, ";");
    }

    free(items_str_copy); // Free the copied string
    return num_items;
}

void remove_item(InventoryDatabase * db, int character_id, int item_id) {
    // Find the inventory for the character.
    for (int i = 0; i < db -> numInventories; i++) {
        if (db -> inventories[i] -> character_id == character_id) {
            Inventory * inv = db -> inventories[i];

            // Find the item to remove.
            for (int j = 0; j < inv -> items.num_items; j++) {
                if (inv -> items.item_ids[j] == item_id) {
                    // Shift the remaining items to fill the gap.
                    for (int k = j; k < inv -> items.num_items - 1; k++) {
                        inv -> items.item_ids[k] = inv -> items.item_ids[k + 1];
                    }

                    // Decrease the number of items.
                    inv -> items.num_items--;

                    // Optionally, reallocate the items array to free the unused space.
                    inv -> items.item_ids = realloc(inv -> items.item_ids, inv -> items.num_items * sizeof(int));

                    // Stop after removing one item.
                    return;
                }
            }
        }
    }
}

void add_item(InventoryDatabase * db, int character_id, int item_id) {
    // Find the inventory for the character.
    for (int i = 0; i < db -> numInventories; i++) {
        if (db -> inventories[i] -> character_id == character_id) {
            Inventory * inv = db -> inventories[i];

            // Increase the number of items.
            inv -> items.num_items++;

            // Reallocate the items array to make space for the new item.
            inv -> items.item_ids = realloc(inv -> items.item_ids, inv -> items.num_items * sizeof(int));

            // Add the new item.
            inv -> items.item_ids[inv -> items.num_items - 1] = item_id;

            // Stop after adding the item.
            return;
        }
    }
}

void print_inventory(InventoryDatabase * db, int character_id) {
    // Find the inventory for the character.
    for (int i = 0; i < db -> numInventories; i++) {
        if (db -> inventories[i] -> character_id == character_id) {
            Inventory * inv = db -> inventories[i];

            printf("Character ID: %d\n", inv -> character_id);
            printf("Items: ");
            for (int j = 0; j < inv -> items.num_items; j++) {
                printf("%d ", inv -> items.item_ids[j]);
            }
            printf("\n");

            // Stop after printing the inventory.
            return;
        }
    }
}

// Function to write an InventoryDatabase to a CSV file
int writeInventoryToCSV(InventoryDatabase * db,
    const char * filename) {
    FILE * file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        return -1;
    }

    for (int i = 0; i < db -> numInventories; i++) {
        fprintf(file, "%d,", db -> inventories[i] -> character_id);
        for (int j = 0; j < db -> inventories[i] -> items.num_items; j++) {
            fprintf(file, "%d", db -> inventories[i] -> items.item_ids[j]);
            if (j < db -> inventories[i] -> items.num_items - 1) {
                fprintf(file, ";");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}

void print_inventoryName(InventoryDatabase* db, ItemDatabase* item_db, int character_id) {
    // Find the inventory for the character.
    for (int i = 0; i < db->numInventories; i++) {
        if (db->inventories[i]->character_id == character_id) {
            Inventory* inv = db->inventories[i];
            printf("Item Names: ");
            for (int j = 0; j < inv->items.num_items; j++) {
                int item_id = inv->items.item_ids[j];
                // Find the item in the item database and print its name.
                for (int k = 0; k < item_db->numItems; k++) {
                    if (item_db->items[k]->id == item_id) {
                        printf("%s ", item_db->items[k]->name);
                        break; // Break the loop once the item is found.
                    }
                }
            }
            printf("\n");
            // Stop after printing the inventory.
            return;
        }
    }
}