#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"

// Initializes the rarity database
RarityDatabase * initRarityDatabase() {
    RarityDatabase * rarityDB = malloc(sizeof(RarityDatabase));
    if (!rarityDB) {
        // Handle error: memory allocation failed
        return NULL;
    }
    rarityDB -> numRarities = 0;
    for (int i = 0; i < MAX_RARITIES; ++i) {
        rarityDB -> rarities[i] = NULL;
    }
    return rarityDB;
}

// Loads a rarity into the database
int loadRarity(RarityDatabase * rarityDB, int id, char * name) {
    if (rarityDB -> numRarities >= MAX_RARITIES) {
        // Handle error: database is full
        return -1;
    }

    Rarity * rarity = malloc(sizeof(Rarity));
    if (!rarity) {
        // Handle error: memory allocation failed
        return -1;
    }

    rarity -> id = id;
    strncpy(rarity -> name, name, MAX_STR - 1);
    rarity -> name[MAX_STR - 1] = '\0'; // Ensure null-termination

    rarityDB -> rarities[rarityDB -> numRarities++] = rarity;
    return 0; // Success
}

// Retrieves the name of a rarity by its ID
char * getRarityName(RarityDatabase * rarityDB, int id) {
    for (int i = 0; i < rarityDB -> numRarities; ++i) {
        if (rarityDB -> rarities[i] -> id == id) {
            return rarityDB -> rarities[i] -> name;
        }
    }
    return NULL; // Rarity ID not found
}

// Prints all rarities in the database
void printAllRarities(RarityDatabase * rarityDB) {
    printf("All rarities:\n");
    for (int i = 0; i < rarityDB -> numRarities; ++i) {
        printf("Rarity ID: %d, Name: %s\n", rarityDB -> rarities[i] -> id, rarityDB -> rarities[i] -> name);
    }
}

// Frees the memory allocated for the rarity database
void freeRarityDatabase(RarityDatabase * rarityDB) {
    if (rarityDB) {
        for (int i = 0; i < rarityDB -> numRarities; ++i) {
            free(rarityDB -> rarities[i]); // Free each rarity
        }
        free(rarityDB); // Free the database structure itself
    }
}

int readRarityDatabaseFromCSV(RarityDatabase * db,
    const char * filename) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }
    int id;
    char name[MAX_STR];
    while (fscanf(file, "%d,%[^\n]\n", & id, name) == 2) {
        if (loadRarity(db, id, name) != 0) {
            fclose(file);
            return -1;
        }
    }
    fclose(file);
    return 0; // Return 0 to indicate success
}