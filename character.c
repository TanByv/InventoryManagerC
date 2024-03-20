#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"

// Function to initialize the database
CharacterDatabase * initDatabase() {
    CharacterDatabase * db = malloc(sizeof(CharacterDatabase));
    if (db == NULL) {
        perror("Error allocating memory for database");
        return NULL;
    }
    db -> numCharacters = 0;
    for (int i = 0; i < MAX_NUM; i++) {
        db -> characters[i] = NULL;
    }
    return db;
}

// Function to generate a unique 6-digit ID
int generateUniqueID(CharacterDatabase * db) {
    int id;
    int isUnique;

    // Keep generating IDs until we find a unique one
    do {
        isUnique = 0;

        // Generate a random 6-digit ID
        id = rand() % 900000 + 100000;

        // Check if the ID is unique
        for (int i = 0; i < db -> numCharacters; i++) {
            if (db -> characters[i] -> id == id) {
                isUnique = 1;
                break;
            }
        }
    } while (isUnique);

    return id;
}

// Function to add a character with a unique ID to the database
int addCharacter(CharacterDatabase * db, char * name, int level, int job_id, int clan_id) {
    int id = generateUniqueID(db);
    return loadCharacter(db, id, name, level, job_id, clan_id);
}

int loadCharacter(CharacterDatabase * db, int id, char * name, int level, int job_id, int clan_id) {
    if (db -> numCharacters >= MAX_NUM) {
        perror("Error: Database is full.\n");
        return -1;
    }

    // Check if a character with the given name already exists
    for (int i = 0; i < db -> numCharacters; i++) {
        if (strcmp(db -> characters[i] -> name, name) == 0) {
            perror("Error: Character with this name already exists.\n");
            return -1;
        }
    }

    Character * character = malloc(sizeof(Character));
    if (character == NULL) {
        perror("Error allocating memory for character\n");
        return -1;
    }

    character -> id = id;
    strncpy(character -> name, name, MAX_STR);
    character -> name[MAX_STR - 1] = '\0'; // Ensure null termination
    character -> level = level;
    character -> job_id = job_id;
    character -> clan_id = clan_id;
    db -> characters[db -> numCharacters] = character;
    db -> numCharacters++;
    return 0;
}

// Function to update a character
int updateCharacter(CharacterDatabase * db, int id, char * new_name, int new_level, int new_job_id, int new_clan_id) {

    // Check if new name already exists
    for (int i = 0; i < db -> numCharacters; i++) {
        if (strcmp(db -> characters[i] -> name, new_name) == 0) {
            perror("New name already taken");
            return -2; // New name already exists
        }
    }

    // Update character
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> id == id) {
            if (strlen(new_name) > 0) {
                strncpy(db -> characters[i] -> name, new_name, MAX_STR - 1);
            }
            if (new_level >= 0) {
                db -> characters[i] -> level = new_level;
            }
            if (new_job_id >= 0) {
                db -> characters[i] -> job_id = new_job_id;;
            }
            if (new_clan_id >= 0) {
                db -> characters[i] -> clan_id = new_clan_id;
            }

            return 0; // Update successful
        }
    }
    perror("id you want to update doesn't exist");
    return -1; // Character not found
}

// Function to remove a character
int removeCharacter(CharacterDatabase * db, int id) {
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> id == id) {
            free(db -> characters[i]);
            db -> characters[i] = db -> characters[db -> numCharacters - 1];
            db -> characters[db -> numCharacters - 1] = NULL;
            db -> numCharacters--;
            return 0; // Removal successful
        }
    }
    return -1; // Character not found
}

// Function to write the database to a CSV file
int writeDatabaseToCSV(CharacterDatabase * db,
    const char * filename) {
    FILE * file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        return -1;
    }

    for (int i = 0; i < db -> numCharacters; i++) {
        fprintf(file, "%d,%s,%d,%d,%d\n", db -> characters[i] -> id, db -> characters[i] -> name, db -> characters[i] -> level, db -> characters[i] -> job_id, db -> characters[i] -> clan_id);
    }

    fclose(file);
    return 0;
}

// Function to read the database's contents from a CSV file
int readDatabaseFromCSV(CharacterDatabase * db,
    const char * filename) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char name[MAX_STR];
    int id, level, job_id, clan_id;

    while (fscanf(file, "%d,%[^,],%d,%d,%d\n", & id, name, & level, & job_id, & clan_id) == 5) {
        if (loadCharacter(db, id, name, level, job_id, clan_id) != 0) {
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0; // Return 0 to indicate success
}

// Function to search users with jobID
void printCharactersByJobID(CharacterDatabase * db, int job_id) {
    int found = 0;
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> job_id == job_id) {
            printf("Character ID: %d, Name: %s, Level: %d, Job ID: %d, Clan ID: %d\n",
                db -> characters[i] -> id, db -> characters[i] -> name, db -> characters[i] -> level,
                db -> characters[i] -> job_id, db -> characters[i] -> clan_id);
            found = 1;
        }
    }
    if (!found) {
        printf("No characters found with Job ID: %d\n", job_id);
    }
}

// Function to search users with clanID
void printCharactersByClanID(CharacterDatabase * db, int clan_id) {
    int found = 0;
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> clan_id == clan_id) {
            printf("Character ID: %d, Name: %s, Level: %d, Job ID: %d, Clan ID: %d\n",
                db -> characters[i] -> id, db -> characters[i] -> name, db -> characters[i] -> level,
                db -> characters[i] -> job_id, db -> characters[i] -> clan_id);
            found = 1;
        }
    }
    if (!found) {
        printf("No characters found with Clan ID: %d\n", clan_id);
    }
}

// Function to search users with level (higher than)
void printCharactersByLevel(CharacterDatabase * db, int level) {
    int found = 0;
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> level > level) {
            printf("Character ID: %d, Name: %s, Level: %d, Job ID: %d, Clan ID: %d\n",
                db -> characters[i] -> id, db -> characters[i] -> name, db -> characters[i] -> level,
                db -> characters[i] -> job_id, db -> characters[i] -> clan_id);
            found = 1;
        }
    }
    if (!found) {
        printf("No characters found with a level higher than %d\n", level);
    }
}

// Function to print everyone
void printCharacters(CharacterDatabase * db) {
    for (int i = 0; i < db -> numCharacters; i++) {
        printf("Character ID: %d, Name: %s, Level: %d, Job ID: %d, Clan ID: %d\n",
            db -> characters[i] -> id, db -> characters[i] -> name, db -> characters[i] -> level,
            db -> characters[i] -> job_id, db -> characters[i] -> clan_id);
    }
}

void printCharactersWithDetails(CharacterDatabase * db, JobDatabase * jdb, ClanDatabase * clanDB) {
    for (int i = 0; i < db -> numCharacters; i++) {
        char * jobName = getJobName(jdb, db -> characters[i] -> job_id);
        char * clanName = getClanName(clanDB, db -> characters[i] -> clan_id);

        if (!jobName) jobName = "Unknown Job";
        if (!clanName) clanName = "Unknown Clan";

        printf("Character ID: %d, Name: %s, Level: %d, Job: %s, Clan: %s\n",
            db -> characters[i] -> id,
            db -> characters[i] -> name,
            db -> characters[i] -> level,
            jobName,
            clanName);
    }
}

void freeCharacterDatabase(CharacterDatabase * db) {
    if (db == NULL) return;

    for (int i = 0; i < db -> numCharacters; i++) {
        free(db -> characters[i]);
    }

    free(db);
}

void printCharactersByMaxLevel(CharacterDatabase * db, int max_level) {
    int found = 0;
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> level <= max_level) {
            printf("Character ID: %d, Name: %s, Level: %d, Job ID: %d, Clan ID: %d\n",
                db -> characters[i] -> id, db -> characters[i] -> name, db -> characters[i] -> level,
                db -> characters[i] -> job_id, db -> characters[i] -> clan_id);
            found = 1;
        }
    }
    if (!found) {
        printf("No characters found with a level at or below %d\n", max_level);
    }
}

void printCharacter(CharacterDatabase * db, int character_id) {
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> id == character_id) {
            Character * c = db -> characters[i];
            printf("ID: %d, Name: %s, Level: %d, Job ID: %d, Clan ID: %d\n",
                c -> id, c -> name, c -> level, c -> job_id, c -> clan_id);
            return;
        }
    }
    printf("Character not found.\n");
}

void printCharacterWithNames(CharacterDatabase * db, JobDatabase * jdb, ClanDatabase * clanDB, int character_id) {
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> id == character_id) {
            Character * c = db -> characters[i];

            char * jobName = getJobName(jdb, c -> job_id);
            char * clanName = getClanName(clanDB, c -> clan_id);

            // Fallback in case the job or clan name is not found
            if (!jobName) jobName = "Unknown Job";
            if (!clanName) clanName = "Unknown Clan";

            printf("ID: %d, Name: %s, Level: %d, Job: %s, Clan: %s\n",
                c -> id, c -> name, c -> level, jobName, clanName);
            return;
        }
    }
    printf("Character not found.\n");
}

char * getCharName(CharacterDatabase * db, int id) {
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> id == id) {
            return db -> characters[i] -> name;
        }
    }
    perror("No character with the given ID is found\n");
    return NULL; // Return NULL if no character with the given ID is found
}

int getCharID(CharacterDatabase * db, char * name) {
    for (int i = 0; i < db -> numCharacters; i++) {
        if (strcmp(db -> characters[i] -> name, name) == 0) {
            return db -> characters[i] -> id;

        }

    }
    perror("No character with the given name is found\n");
    return -1; // Return -1 if no character with the given name is found
}

void printCharactersName(CharacterDatabase * db) {
    for (int i = 0; i < db -> numCharacters; i++) {
        printf("Character Name: %s\n",
            db -> characters[i] -> name);
    }
}
