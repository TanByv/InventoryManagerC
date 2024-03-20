#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include "defs.h"
//
// Created by Enes on 30.12.2023.
//
void mainMenu(CharacterDatabase * db, JobDatabase * jdb, ClanDatabase * clanDB, InventoryDatabase * invDB, ItemDatabase * itemDB, ItemTypeDatabase * itemTypeDB, RarityDatabase * rarityDB) {
    int choice;
    do {
        printf("\n--- Main Menu ---\n");
        printf("1. Character Menu\n");
        printf("2. Job Menu\n");
        printf("3. Clan Menu\n");
        printf("4. Inventory Menu\n");
        printf("5. Exit\n");
        printf("Enter Your Choice: ");
        scanf("%d", & choice);

        switch (choice) {
        case 1:
            characterMenu(db, jdb, clanDB);
            break;
        case 2:
            jobMenu(db, jdb);
            break;
        case 3:
            clanMenu(db, clanDB);
            break;
        case 4:
            inventoryMenu(invDB, itemDB, itemTypeDB, rarityDB, db);
            break;
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

void characterMenu(CharacterDatabase * db, JobDatabase * jdb, ClanDatabase * clanDB) {
    int choice;
    char name[MAX_STR];
    int level, job_id, clan_id, id, new_level, new_job_id, new_clan_id;
    char new_name[MAX_STR];

    do {
        printf("\n--- Character Menu ---\n");
        printf("1. Add Character\n");
        printf("2. Remove Character\n");
        printf("3. Update Character\n");
        printf("4. Look at Character Profile\n");
        printf("5. List All Characters\n");
        printf("6. Search Characters by Job\n");
        printf("7. Search Characters by Minimum Level\n");
        printf("8. Search Characters by Maximum Level\n");
        printf("9. Return to Main Menu\n");
        printf("Enter Your Choice: ");
        scanf("%d", & choice);

        switch (choice) {
        case 1:
            printf("Character Name: ");
            scanf("%s", name);
            printf("\nCharacter Level: ");
            scanf("%d", & level);
            printAllJobs(jdb); //
            printf("\nJob ID: ");
            scanf("%d", & job_id);
            printAllClans(clanDB);
            printf("\nClan ID: ");
            scanf("%d", & clan_id);
            addCharacter(db, name, level, job_id, clan_id);
            break;
        case 2:
            printCharacters(db); //
            printf("\nCharacter ID: ");
            scanf("%d", & id);
            removeCharacter(db, id); //
            break;
        case 3:
            printCharacters(db); //
            printf("\nCharacter ID: ");
            scanf("%d", & id);
            printf("\nNew Character Name: ");

            while (getchar() != '\n');
            if (fgets(new_name, sizeof(new_name), stdin) != "") {
                size_t len = strlen(new_name);
                if (len > 0 && new_name[len - 1] == '\n') {
                    new_name[len - 1] = '\0'; // Satır sonunu kaldır
                }
            }

            printf("\nNew Character Level: ");
            scanf("%d", & new_level);
            printAllJobs(jdb); //
            printf("\nNew Job ID: ");
            scanf("%d", & new_job_id);
            printAllClans(clanDB);
            printf("\nNew Clan ID: ");
            scanf("%d", & new_clan_id);
            updateCharacter(db, id, new_name, new_level, new_job_id, new_clan_id); //
            break;
        case 4:
            printCharactersName(db);
            printf("\nCharacter Name: ");
            scanf("%s", name);

            int charId = getCharID(db, name); // Declare charId at the beginning of the block
            if (charId != -1) {
                printCharacterWithNames(db, jdb, clanDB, charId);
            } else {
                printf("Character '%s' not found.\n", name);
            }
            break;
        case 5:
            printf("\n");
            printCharactersWithDetails(db, jdb, clanDB);
            break;
        case 6:
            printAllJobs(jdb); //
            printf("\nJob ID: ");
            scanf("%d", & job_id);
            printCharactersByJobID(db, job_id);
            break;
        case 7:
            printf("\nSearch Characters by Minimum Level: ");
            scanf("%d", & level);
            printCharactersByLevel(db, level);
            break;
        case 8:
            printf("\nSearch Characters by Maximum Level: ");
            scanf("%d", & level);
            printCharactersByMaxLevel(db, level);
            break;
        case 9:
            printf("Returning to Main Menu...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);
}

void jobMenu(CharacterDatabase * db, JobDatabase * jdb) {
    int choice;
    do {
        printf("\n--- Job Menu ---\n");
        printf("1. List Available Jobs\n");
        printf("2. How Many Characters Per Job\n");
        printf("3. Return to Main Menu\n");
        printf("Enter Your Choice: ");
        scanf("%d", & choice);

        switch (choice) {
        case 1:
            printAllJobs(jdb); //
            break;
        case 2:
            printJobDistribution(db, jdb);
            break;
        case 3:
            printf("Returning to Main Menu...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

void clanMenu(CharacterDatabase * db, ClanDatabase * clanDB) {
    int choice;
    char name[MAX_STR], newName[MAX_STR];
    int level, newLevel;
    int clan_id, char_id;
    do {
        printf("\n--- Clan Menu ---\n");
        printf("1. Add Clan\n");
        printf("2. Delete Clan\n");
        printf("3. Update Clan\n");
        printf("4. Join a Clan\n");
        printf("5. list All Clans\n");
        printf("6. List Characters in Clan\n");
        printf("7. Get Job Distribution in a Clan\n");
        printf("8. Search Clan by Minimum level\n");
        printf("9. Search Clan by Maximum level\n");
        printf("0. Return to Main Menu\n");
        printf("Enter Your Choice: ");
        scanf("%d", & choice);

        switch (choice) {
        case 1:
            printf("Clan Name: ");
            scanf("%s", name);
            printf("\nClan Level: ");
            scanf("%d", & level);
            addClan(clanDB, name, level); //
            break;
        case 2:
            printAllClans(clanDB);
            printf("\n Clan ID: ");
            scanf("%d", & clan_id);
            deleteClan(clanDB, clan_id); //
            break;
        case 3:
            printAllClans(clanDB);
            printf("\nClan ID: ");
            scanf("%d", & clan_id);
            printf("\nNew Clan Name: ");

            while (getchar() != '\n');
            if (fgets(newName, sizeof(newName), stdin) != "") {
                size_t len = strlen(newName);
                if (len > 0 && newName[len - 1] == '\n') {
                    newName[len - 1] = '\0'; // Satır sonunu kaldır
                }
            }

            printf("\nNew Clan Level: ");
            scanf("%d", & newLevel);
            updateClan(clanDB, clan_id, newName, newLevel); //
            break;
        case 4:
            printCharacters(db);
            printf("\nCharacter ID: ");
            scanf("%d", & char_id);
            printAllClans(clanDB);
            printf("\nClan ID: ");
            scanf("%d", & clan_id);
            joinClan(db, char_id, clan_id);
            break;
        case 5:
            printAllClans(clanDB);
            break;
        case 6:
            printAllClans(clanDB);
            printf("\nClan ID: ");
            scanf("%d", & clan_id);
            printCharactersInClan(db, clan_id);
            break;
        case 7:
            printAllClans(clanDB);
            printf("\nClan ID: ");
            scanf("%d", & clan_id);
            printClanJobDistribution(db, clan_id);
            break;
        case 8:
            printf("\nSearch Clans by Minimum Level: ");
            scanf("%d", & level);
            printClansByMinLevel(clanDB, level);
            break;
        case 9:
            printf("\nSearch Clans by Maximum Level: ");
            scanf("%d", & level);
            printClansByMaxLevel(clanDB, level);
            break;
        case 0:
            printf("Returning to Main Menu...\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

void inventoryMenu(InventoryDatabase * invDB, ItemDatabase * itemDB, ItemTypeDatabase * itemTypeDB, RarityDatabase * rarityDB, CharacterDatabase * db) {
    int choice;
    int char_id, item_id, rarity_id, type_id;
    const char stat_type[MAX_ITEM_TYPES];
    do {
        printf("\n--- Inventory Menu ---\n");
        printf("1. Add Item to User\n");
        printf("2. Remove Item From User\n");
        printf("3. Get User's Inventory\n");
        printf("4. List Existing Items\n");
        printf("5. List Existing Item Types and Their Stat Types\n");
        printf("6. List Existing Rarities\n");
        printf("7. List Item by Type\n");
        printf("8. List Item Types by Stat Type\n");
        printf("9. List Items by Rarity\n");
        printf("0. Return to Main Menu\n");
        printf("Enter Your Choice: ");
        scanf("%d", & choice);

        switch (choice) {
        case 1:
            printCharacters(db);
            printf("\nCharacter ID: ");
            scanf("%d", & char_id);
            printAllItems(itemDB, rarityDB, itemTypeDB);
            printf("\nItem ID: ");
            scanf("%d", & item_id);
            add_item(invDB, char_id, item_id);

            break;
        case 2:
            printCharacters(db);
            printf("\nCharacter ID: ");
            scanf("%d", & char_id);
            print_inventory(invDB, char_id);
            printf("\nItem ID: ");
            scanf("%d", & item_id);
            remove_item(invDB, char_id, item_id);
            break;
        case 3:
            printCharacters(db);
            printf("\nCharacter ID: ");
            scanf("%d", & char_id);
            print_inventory(invDB, char_id);
            print_inventoryName(invDB, itemDB, char_id);
            break;
        case 4:
            printAllItems(itemDB, rarityDB, itemTypeDB);
            break;
        case 5:
            printItemTypesAndStats(itemTypeDB);
            break;
        case 6:
            printAllRarities(rarityDB);
            break;
        case 7:
            printf("\n");
            printAllItemTypes(itemTypeDB);
            printf("\nType ID: ");
            scanf("%d", & type_id);
            printItemsByType(itemDB, type_id);
            break;
        case 8:
            printf("\n");
            printf("Physical_DMG\nRanged_DMG\nMagic_DMG\nMusic_DMG\nDefense\n");
            printf("\nStat Type: ");
            scanf("%s", stat_type);
            printItemTypesByStatType(itemTypeDB, stat_type);
            break;
        case 9:
            printf("\n");
            printAllRarities(rarityDB);
            printf("\nRarity ID: ");
            scanf("%d", & rarity_id);
            printItemsByRarity(itemDB, rarity_id);
            break;
        case 0:
            printf("Returning to Main Menu...\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}