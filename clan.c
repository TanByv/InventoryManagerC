#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"

// Initialize the clan database
ClanDatabase * initClanDatabase() {
    ClanDatabase * clanDB = malloc(sizeof(ClanDatabase));
    if (clanDB == NULL) {
        perror("Error allocating memory for clan database");
        return NULL;
    }
    clanDB -> numClans = 0;
    for (int i = 0; i < MAX_CLANS; i++) {
        clanDB -> clans[i] = NULL;
    }
    return clanDB;
}

// Function to generate a unique 6-digit ID for clans
int generateUniqueClanID(ClanDatabase * clanDB) {
    int id;
    int isUnique;

    // Keep generating IDs until we find a unique one
    do {
        isUnique = 0;

        // Generate a random 6-digit ID
        id = rand() % 900000 + 100000;

        // Check if the ID is unique
        for (int i = 0; i < clanDB -> numClans; i++) {
            if (clanDB -> clans[i] -> id == id) {
                isUnique = 1;
                break;
            }
        }
    } while (isUnique);

    return id;
}

// Function to add a clan with a unique ID to the database
int addClan(ClanDatabase * clanDB, char * name, int level) {
    int id = generateUniqueClanID(clanDB);
    return loadClan(clanDB, id, name, level);
}

// Load a clan into the database
int loadClan(ClanDatabase * clanDB, int id, char * name, int level) {
    if (clanDB -> numClans >= MAX_CLANS) {
        perror("Error: Clan database is full.\n");
        return -1;
    }

    // Check for existing clan
    for (int i = 0; i < clanDB -> numClans; i++) {
        if (strcmp(clanDB -> clans[i] -> name, name) == 0) {
            perror("Error: Clan with this name already exists.\n");
            return -1;
        }
    }

    Clan * clan = malloc(sizeof(Clan));
    if (clan == NULL) {
        perror("Error allocating memory for clan\n");
        return -1;
    }

    clan -> id = id;
    strncpy(clan -> name, name, MAX_STR);
    clan -> name[MAX_STR - 1] = '\0';
    clan -> level = level;
    clanDB -> clans[clanDB -> numClans] = clan;
    clanDB -> numClans++;
    return 0;
}

int deleteClan(ClanDatabase * clanDB, int id) {
    if (clanDB == NULL) {
        perror("Database is NULL.\n");
        return -1;
    }

    for (int i = 0; i < clanDB -> numClans; i++) {
        if (clanDB -> clans[i] != NULL && clanDB -> clans[i] -> id == id) {
            free(clanDB -> clans[i]);
            clanDB -> clans[i] = clanDB -> clans[clanDB -> numClans - 1]; // Move the last clan to the current position
            clanDB -> clans[clanDB -> numClans - 1] = NULL;
            clanDB -> numClans--;
            return 0; // Clan deleted successfully
        }
    }

    printf("No clan found with ID: %d\n", id);
    return -1; // Clan not found
}

// Load clans from file
int readClanDatabaseFromCSV(ClanDatabase * clanDB,
    const char * filename) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file\n");
        return -1;
    }

    char name[MAX_STR];
    int id, level;

    while (fscanf(file, "%d,%[^,],%d\n", & id, name, & level) == 3) {
        if (loadClan(clanDB, id, name, level) != 0) {
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;

}

// Turn clan id into clan name
char * getClanName(ClanDatabase * clanDB, int id) {
    for (int i = 0; i < clanDB -> numClans; i++) {
        if (clanDB -> clans[i] -> id == id) {
            return clanDB -> clans[i] -> name;
        }
    }
    perror("No clan with the given ID is found \n");
    return NULL; // Return NULL if no clan with the given ID is found
}

// Turn clan name into clan id
int getClanID(ClanDatabase * clanDB, char * name) {
    for (int i = 0; i < clanDB -> numClans; i++) {
        if (strcmp(clanDB -> clans[i] -> name, name) == 0) {
            return clanDB -> clans[i] -> id;
        }
    }
    perror("No clan with the given name is found \n");
    return -1; // Return -1 if no clan with the given name is found
}

// Function to update a clan
int updateClan(ClanDatabase * clanDB, int id,
    const char * newName, int newLevel) {
    // Check if new name already exists
    for (int i = 0; i < clanDB -> numClans; i++) {
        if (strcmp(clanDB -> clans[i] -> name, newName) == 0) {
            perror("New clan name already taken");
            return -2; // New name already exists
        }
    }

    // Update clan
    for (int i = 0; i < clanDB -> numClans; i++) {
        if (clanDB -> clans[i] -> id == id) {
            if (strlen(newName) > 0) {
                strncpy(clanDB -> clans[i] -> name, newName, MAX_STR - 1);
                clanDB -> clans[i] -> name[MAX_STR - 1] = '\0'; // Ensure null termination
            }
            if (newLevel >= 0) {
                clanDB -> clans[i] -> level = newLevel;
            }

            return 0; // Update successful
        }
    }
    perror("id you want to update doesn't exist");
    return -1; // Clan not found
}

void printAllClans(const ClanDatabase * clanDB) {
    if (clanDB == NULL || clanDB -> numClans == 0) {
        printf("No clans available in the database.\n");
        return;
    }

    printf("List of Clans:\n");
    for (int i = 0; i < clanDB -> numClans; i++) {
        if (clanDB -> clans[i] != NULL) {
            printf("Clan ID: %d, Name: %s, Level: %d\n", clanDB -> clans[i] -> id, clanDB -> clans[i] -> name, clanDB -> clans[i] -> level);
        }
    }
}

void freeClanDatabase(ClanDatabase * clanDB) {
    if (clanDB == NULL) return;

    for (int i = 0; i < clanDB -> numClans; i++) {
        free(clanDB -> clans[i]); // Free each individual clan
    }

    free(clanDB);
}

int joinClan(CharacterDatabase * db, int character_id, int clan_id) {
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> id == character_id) {
            db -> characters[i] -> clan_id = clan_id;
            return 0; // Success
        }
    }
    return -1; // Character not found
}

void printCharactersInClan(CharacterDatabase * db, int clan_id) {
    int count = 0;
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> clan_id == clan_id) {
            printCharacter(db, db -> characters[i] -> id);
            count++;
        }
    }
    printf("Total characters in clan: %d\n", count);
}

void printClanJobDistribution(CharacterDatabase * db, int clan_id) {
    int jobCounts[MAX_JOBS] = {
        0
    }, totalInClan = 0;
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> clan_id == clan_id) {
            jobCounts[db -> characters[i] -> job_id]++;
            totalInClan++;
        }
    }
    for (int j = 0; j < MAX_JOBS; j++) {
        printf("Job ID: %d, Count: %d, Percentage: %.2f%%\n",
            j, jobCounts[j], totalInClan ? 100.0 * jobCounts[j] / totalInClan : 0.0);
    }
}

void printClansByMinLevel(ClanDatabase * clanDB, int min_level) {
    int count = 0;
    for (int i = 0; i < clanDB -> numClans; i++) {
        if (clanDB -> clans[i] -> level >= min_level) {
            printf("Clan ID: %d, Name: %s, Level: %d\n", clanDB -> clans[i] -> id, clanDB -> clans[i] -> name, clanDB -> clans[i] -> level);
            count++;
        }
    }
    if (count == 0) {
        printf("No clans found with a minimum level of %d\n", min_level);
    } else {
        printf("Total clans found: %d\n", count);
    }
}

void printClansByMaxLevel(ClanDatabase * clanDB, int max_level) {
    int count = 0;
    for (int i = 0; i < clanDB -> numClans; i++) {
        if (clanDB -> clans[i] -> level <= max_level) {
            printf("Clan ID: %d, Name: %s, Level: %d\n", clanDB -> clans[i] -> id, clanDB -> clans[i] -> name, clanDB -> clans[i] -> level);
            count++;
        }
    }
    if (count == 0) {
        printf("No clans found with a maximum level of %d\n", max_level);
    } else {
        printf("Total clans found: %d\n", count);
    }
}

int writeClanDatabaseToCSV(ClanDatabase * db,
    const char * filename) {
    FILE * file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        return -1;
    }
    for (int i = 0; i < db -> numClans; i++) {
        if (db -> clans[i] != NULL) { // Check if the clan pointer is not null
            fprintf(file, "%d,%s,%d\n", db -> clans[i] -> id, db -> clans[i] -> name, db -> clans[i] -> level);
        }
    }
    fclose(file);
    return 0;
}
