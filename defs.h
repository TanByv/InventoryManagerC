#define MAX_STR 32
#define MAX_NUM 100
#define MAX_JOBS 50
#define MAX_CLANS 20
#define MAX_ITEMS 100
#define MAX_RARITIES 5
#define MAX_ITEM_TYPES 14
#define MAX_INVENTORY_ITEMS 50
#define MAX_LINE_LENGTH 20

typedef struct {
    int id;
    char name[MAX_STR];
    int level;
    int job_id;
    int clan_id;
} Character;

typedef struct {
    Character *characters[MAX_NUM];
    int numCharacters;
} CharacterDatabase;

typedef struct {
    int id;
    char name[MAX_STR];
} Job;

typedef struct {
    Job *jobs[MAX_JOBS];
    int numJobs;
} JobDatabase;

typedef struct {
    int id;
    char name[MAX_STR];
    int level;
} Clan;

typedef struct {
    Clan *clans[MAX_CLANS];
    int numClans;
} ClanDatabase;

// character.c
CharacterDatabase *initDatabase();//
int loadCharacter(CharacterDatabase *db, int id, char *name, int level, int job_id, int clan_id);//
int removeCharacter(CharacterDatabase *db, int id);//
int updateCharacter(CharacterDatabase *db, int id, char *new_name, int new_level, int new_job_id, int new_clan_id);//
int writeDatabaseToCSV(CharacterDatabase *db, const char *filename);//
int readDatabaseFromCSV(CharacterDatabase *db, const char *filename);//
int generateUniqueID(CharacterDatabase *db);//
int addCharacter(CharacterDatabase *db, char *name, int level, int job_id, int clan_id);//
void printCharactersByJobID(CharacterDatabase *db, int job_id);//
void printCharactersByClanID(CharacterDatabase *db, int clan_id);//
void printCharactersByLevel(CharacterDatabase *db, int level);//
void printCharacters(CharacterDatabase *db);//
void printCharactersWithDetails(CharacterDatabase *db, JobDatabase *jdb, ClanDatabase *clanDB);
void freeCharacterDatabase(CharacterDatabase *db);//
void printCharactersByMaxLevel(CharacterDatabase *db, int max_level);
void printCharacterWithNames(CharacterDatabase *db, JobDatabase *jdb, ClanDatabase *clanDB, int character_id);
void printCharacter(CharacterDatabase *db, int character_id);
char *getCharName(CharacterDatabase *db, int id);
int getCharID(CharacterDatabase *db, char *name);
void printCharactersName(CharacterDatabase *db);

// job.c
JobDatabase *initJobDatabase();//
int loadJob(JobDatabase *jdb, int id, char *name);//
int readJobDatabaseFromCSV(JobDatabase *jdb, const char *filename);//
char *getJobName(JobDatabase *jdb, int id);//
int getJobID(JobDatabase *jdb, char *name);//
void printAllJobs(JobDatabase *jdb);//
void freeJobDatabase(JobDatabase *jdb);//
void printJobDistribution(CharacterDatabase *db, JobDatabase *jdb);

// clan.c
ClanDatabase *initClanDatabase();//
int generateUniqueClanID(ClanDatabase *db);//
int addClan(ClanDatabase *clanDB, char *name, int level);//
int loadClan(ClanDatabase *db, int id, char *name, int level);//
int deleteClan(ClanDatabase *db, int id);//
int readClanDatabaseFromCSV(ClanDatabase *db, const char *filename);//
char *getClanName(ClanDatabase *db, int id);//
int getClanID(ClanDatabase *clanDB, char *name);//
int updateClan(ClanDatabase *clanDB, int id, const char *newName, int newLevel);//
void printAllClans(const ClanDatabase *clanDB);//
void freeClanDatabase(ClanDatabase *clanDB);//
int joinClan(CharacterDatabase *db, int character_id, int clan_id);
void printCharactersInClan(CharacterDatabase *db, int clan_id);
void printClanJobDistribution(CharacterDatabase *db, int clan_id);
void printClansByMinLevel(ClanDatabase *clanDB, int min_level);
void printClansByMaxLevel(ClanDatabase *clanDB, int max_level);
int writeClanDatabaseToCSV(ClanDatabase *db, const char *filename);

// ---------------------------------------------------------------------------------

typedef struct {
    int id;
    char name[MAX_STR];
    int rarity_id;
    int type_id;
    int stat_amount;
} Item;

typedef struct {
    Item *items[MAX_ITEMS];
    int numItems;
} ItemDatabase;

typedef struct {
    int id;
    char name[MAX_STR];
} Rarity;

typedef struct {
    Rarity *rarities[MAX_RARITIES];
    int numRarities;
} RarityDatabase;

typedef struct {
    int id;
    char type[MAX_STR];
    char stat_type[MAX_STR];
} ItemType;

typedef struct {
    ItemType *itemTypes[MAX_ITEM_TYPES];
    int numItemTypes;
} ItemTypeDatabase;

typedef struct {
    int* item_ids;
    int num_items;
} ItemArray;

typedef struct {
    int character_id;
    ItemArray items;
} Inventory;

typedef struct {
    Inventory *inventories[MAX_NUM];
    int numInventories;
} InventoryDatabase;

// menu.c
void mainMenu(CharacterDatabase *db, JobDatabase *jdb, ClanDatabase *clanDB, InventoryDatabase *invDB, ItemDatabase *itemDB, ItemTypeDatabase *itemTypeDB, RarityDatabase *rarityDB);
void characterMenu(CharacterDatabase *db, JobDatabase *jdb, ClanDatabase *clanDB);
void jobMenu(CharacterDatabase *db, JobDatabase *jdb);
void clanMenu(CharacterDatabase *db, ClanDatabase *clanDB);
void inventoryMenu(InventoryDatabase *invDB, ItemDatabase *itemDB, ItemTypeDatabase *itemTypeDB, RarityDatabase *rarityDB, CharacterDatabase *db);

// inventory.c
InventoryDatabase *initInventoryDatabase();
int loadInventory(InventoryDatabase *invDB, int character_id, int *item_ids, int num_items);
void freeInventoryDatabase(InventoryDatabase *invDB);
int readInventoryFromCSV(InventoryDatabase *db, const char *filename);
int get_items(char* items_str, int** items);
void remove_item(InventoryDatabase* db, int character_id, int item_id);
void add_item(InventoryDatabase* db, int character_id, int item_id);
void print_inventory(InventoryDatabase* db, int character_id);
int writeInventoryToCSV(InventoryDatabase *db, const char *filename);
void print_inventoryName(InventoryDatabase* db, ItemDatabase* item_db, int character_id);

// item.c
ItemDatabase *initItemDatabase();
int loadItem(ItemDatabase *itemDB, int id, char *name, int rarity_id, int type_id, int stat_amount);
void printItemsByRarity(ItemDatabase *itemDB, int rarity_id);
void printItemsByType(ItemDatabase *itemDB, int type_id);
void printItemTypesByStatType(ItemTypeDatabase *itemTypeDB, const char *stat_type);
void freeItemDatabase(ItemDatabase *itemDB);
void printAllItems(ItemDatabase * itemDB, RarityDatabase * rarityDB, ItemTypeDatabase * itemTypeDB);
int readItemDatabaseFromCSV(ItemDatabase *db, const char *filename); 

// rarity.c
RarityDatabase *initRarityDatabase();
int loadRarity(RarityDatabase *rarityDB, int id, char *name);
char *getRarityName(RarityDatabase *rarityDB, int id);
void printAllRarities(RarityDatabase *rarityDB);
void freeRarityDatabase(RarityDatabase *rarityDB);
int readRarityDatabaseFromCSV(RarityDatabase *db, const char *filename);

// type.c
ItemTypeDatabase *initItemTypeDatabase();
int loadItemType(ItemTypeDatabase *itemTypeDB, int id, char *type, char *stat_type);
char *getItemTypeName(ItemTypeDatabase *itemTypeDB, int id);
void printAllItemTypes(ItemTypeDatabase *itemTypeDB);
void freeItemTypeDatabase(ItemTypeDatabase *itemTypeDB);
int readItemTypeDatabaseFromCSV(ItemTypeDatabase *itemTypeDB, const char *filename);
void printItemTypesAndStats(ItemTypeDatabase *itemTypeDB);
