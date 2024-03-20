#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"

int main() {
    srand((unsigned int) time(NULL));

    // Initialize the database
    CharacterDatabase * db = initDatabase();
    JobDatabase * jdb = initJobDatabase();
    ClanDatabase * clanDB = initClanDatabase();
    InventoryDatabase * invDB = initInventoryDatabase();
    ItemDatabase * itemDB = initItemDatabase();
    ItemTypeDatabase * itemTypeDB = initItemTypeDatabase();
    RarityDatabase * rarityDB = initRarityDatabase();

    // Read existing characters from a CSV file
    readDatabaseFromCSV(db, "database.csv");
    readJobDatabaseFromCSV(jdb, "jobs.csv");
    readClanDatabaseFromCSV(clanDB, "clans.csv");
    readInventoryFromCSV(invDB, "inventories.csv");
    readItemDatabaseFromCSV(itemDB, "items.csv");
    readItemTypeDatabaseFromCSV(itemTypeDB, "itemtypes.csv");
    readRarityDatabaseFromCSV(rarityDB, "rarities.csv");

    mainMenu(db, jdb, clanDB, invDB, itemDB, itemTypeDB, rarityDB);

    // Write the updated database to a CSV file
    writeDatabaseToCSV(db, "database.csv");
    writeClanDatabaseToCSV(clanDB, "clans.csv");
    writeInventoryToCSV(invDB, "inventories.csv");

    freeCharacterDatabase(db);
    freeJobDatabase(jdb);
    freeClanDatabase(clanDB);
    freeInventoryDatabase(invDB);
    freeItemDatabase(itemDB);
    freeItemTypeDatabase(itemTypeDB);
    freeRarityDatabase(rarityDB);

    return 0;
}
