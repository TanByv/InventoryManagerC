#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"

// Initializes the item database
ItemDatabase * initItemDatabase() {
    ItemDatabase * itemDB = malloc(sizeof(ItemDatabase));
    if (!itemDB) {
        // Handle error: memory allocation failed
        return NULL;
    }
    itemDB -> numItems = 0;
    for (int i = 0; i < MAX_ITEMS; ++i) {
        itemDB -> items[i] = NULL;
    }
    return itemDB;
}

// Loads an item into the database
int loadItem(ItemDatabase * itemDB, int id, char * name, int rarity_id, int type_id, int stat_amount) {
    if (itemDB -> numItems >= MAX_ITEMS) {
        // Handle error: database is full
        return -1;
    }

    // Allocate memory for new item
    Item * item = malloc(sizeof(Item));
    if (!item) {
        // Handle error: memory allocation failed
        return -1;
    }

    // Set item properties
    item -> id = id;
    strncpy(item -> name, name, MAX_STR - 1);
    item -> name[MAX_STR - 1] = '\0'; // Ensure null-termination
    item -> rarity_id = rarity_id;
    item -> type_id = type_id;
    item -> stat_amount = stat_amount;

    // Add the item to the database
    itemDB -> items[itemDB -> numItems++] = item;
    return 0; // Success
}

// Prints all items of a specific rarity
void printItemsByRarity(ItemDatabase * itemDB, int rarity_id) {
    for (int i = 0; i < itemDB -> numItems; ++i) {
        if (itemDB -> items[i] -> rarity_id == rarity_id) {
            printf("Item ID: %d, Name: %s, Rarity ID: %d, Type ID: %d, Stat Amount: %d\n",
                itemDB -> items[i] -> id, itemDB -> items[i] -> name, itemDB -> items[i] -> rarity_id,
                itemDB -> items[i] -> type_id, itemDB -> items[i] -> stat_amount);
        }
    }
}

// Prints all items of a specific type
void printItemsByType(ItemDatabase * itemDB, int type_id) {
    for (int i = 0; i < itemDB -> numItems; ++i) {
        if (itemDB -> items[i] -> type_id == type_id) {
            printf("Item ID: %d, Name: %s, Rarity ID: %d, Type ID: %d, Stat Amount: %d\n",
                itemDB -> items[i] -> id, itemDB -> items[i] -> name, itemDB -> items[i] -> rarity_id,
                itemDB -> items[i] -> type_id, itemDB -> items[i] -> stat_amount);
        }
    }
}

// Prints all item types that match a certain stat type

// Frees the memory allocated for the item database
void freeItemDatabase(ItemDatabase * itemDB) {
    if (itemDB) {
        for (int i = 0; i < itemDB -> numItems; ++i) {
            free(itemDB -> items[i]); // Free each item
        }
        free(itemDB); // Free the database structure itself
    }
}

void printAllItems(ItemDatabase * itemDB, RarityDatabase * rarityDB, ItemTypeDatabase * itemTypeDB) {
    for (int i = 0; i < itemDB -> numItems; ++i) {
        char* rarity_name;
        char* type_name;

        // Find the rarity name based on the rarity_id
        for (int j = 0; j < rarityDB -> numRarities; ++j) {
            if (rarityDB -> rarities[j] -> id == itemDB -> items[i] -> rarity_id) {
                rarity_name = rarityDB -> rarities[j] -> name;
                break;
            }
        }

        // Find the type name based on the type_id
        for (int k = 0; k < itemTypeDB -> numItemTypes; ++k) {
            if (itemTypeDB -> itemTypes[k] -> id == itemDB -> items[i] -> type_id) {
                type_name = itemTypeDB -> itemTypes[k] -> type;
                break;
            }
        }

        printf("Item: ID: %d, Name: %s, Rarity: %s, Type: %s, Stat Amount: %d\n",
            itemDB -> items[i] -> id, itemDB -> items[i] -> name, rarity_name,
            type_name, itemDB -> items[i] -> stat_amount);
    }
}

int readItemDatabaseFromCSV(ItemDatabase * db,
    const char * filename) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    int id, rarity_id, type_id, stat_amount;
    char name[MAX_STR];
    while (fscanf(file, "%d,%[^,],%d,%d,%d\n", & id, name, & rarity_id, & type_id, & stat_amount) == 5) {
        if (loadItem(db, id, name, rarity_id, type_id, stat_amount) != 0) {
            fclose(file);
            return -1;
        }
    }
    fclose(file);
    return 0; // Return 0 to indicate success
}
