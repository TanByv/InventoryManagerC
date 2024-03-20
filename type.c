#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"

// Initializes the item type database
ItemTypeDatabase * initItemTypeDatabase() {
    ItemTypeDatabase * itemTypeDB = malloc(sizeof(ItemTypeDatabase));
    if (!itemTypeDB) {
        // Handle error: memory allocation failed
        return NULL;
    }
    itemTypeDB -> numItemTypes = 0;
    for (int i = 0; i < MAX_ITEM_TYPES; ++i) {
        itemTypeDB -> itemTypes[i] = NULL;
    }
    return itemTypeDB;
}

// Loads an item type into the database
int loadItemType(ItemTypeDatabase * itemTypeDB, int id, char * type, char * stat_type) {
    if (itemTypeDB -> numItemTypes >= MAX_ITEM_TYPES) {
        // Handle error: database is full
        return -1;
    }

    ItemType * itemType = malloc(sizeof(ItemType));
    if (!itemType) {
        // Handle error: memory allocation failed
        return -1;
    }

    itemType -> id = id;
    strncpy(itemType -> type, type, MAX_STR - 1);
    itemType -> type[MAX_STR - 1] = '\0'; // Ensure null-termination
    strncpy(itemType -> stat_type, stat_type, MAX_STR - 1);
    itemType -> stat_type[MAX_STR - 1] = '\0'; // Ensure null-termination

    itemTypeDB -> itemTypes[itemTypeDB -> numItemTypes++] = itemType;
    return 0; // Success
}

// Retrieves the type name of an item by its ID
char * getItemTypeName(ItemTypeDatabase * itemTypeDB, int id) {
    for (int i = 0; i < itemTypeDB -> numItemTypes; ++i) {
        if (itemTypeDB -> itemTypes[i] -> id == id) {
            return itemTypeDB -> itemTypes[i] -> type;
        }
    }
    return NULL; // Item type ID not found
}

// Prints all item types in the database
void printAllItemTypes(ItemTypeDatabase * itemTypeDB) {
    printf("All item types:\n");
    for (int i = 0; i < itemTypeDB -> numItemTypes; ++i) {
        printf("Item Type ID: %d, Type: %s, Stat Type: %s\n",
            itemTypeDB -> itemTypes[i] -> id, itemTypeDB -> itemTypes[i] -> type, itemTypeDB -> itemTypes[i] -> stat_type);
    }
}

// Frees the memory allocated for the item type database
void freeItemTypeDatabase(ItemTypeDatabase * itemTypeDB) {
    if (itemTypeDB) {
        for (int i = 0; i < itemTypeDB -> numItemTypes; ++i) {
            free(itemTypeDB -> itemTypes[i]); // Free each item type
        }
        free(itemTypeDB); // Free the database structure itself
    }
}

int readItemTypeDatabaseFromCSV(ItemTypeDatabase * itemTypeDB,
    const char * filename) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    int id;
    char type[MAX_STR];
    char stat_type[MAX_STR];
    while (fscanf(file, "%d,%[^,],%[^\n]\n", & id, type, stat_type) == 3) {
        if (loadItemType(itemTypeDB, id, type, stat_type) != 0) {
            fclose(file);
            return -1;
        }
    }
    fclose(file);
    return 0; // Return 0 to indicate success
}

void printItemTypesAndStats(ItemTypeDatabase * itemTypeDB) {
    for (int i = 0; i < itemTypeDB -> numItemTypes; ++i) {
        printf("Item Type: %s, Stat Type: %s\n",
            itemTypeDB -> itemTypes[i] -> type, itemTypeDB -> itemTypes[i] -> stat_type);
    }
}

void printItemTypesByStatType(ItemTypeDatabase * itemTypeDB,
    const char * stat_type) {
    for (int i = 0; i < itemTypeDB -> numItemTypes; ++i) {
        if (strcmp(itemTypeDB -> itemTypes[i] -> stat_type, stat_type) == 0) {
            printf("Item Type: %s, Stat Type: %s\n",
                itemTypeDB -> itemTypes[i] -> type, itemTypeDB -> itemTypes[i] -> stat_type);
        }
    }
}
