#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "defs.h"

// Function to initialize the job database
JobDatabase * initJobDatabase() {
    JobDatabase * jdb = malloc(sizeof(JobDatabase));
    if (jdb == NULL) {
        perror("Error allocating memory for job database");
        return NULL;
    }
    jdb -> numJobs = 0;
    for (int i = 0; i < MAX_JOBS; i++) {
        jdb -> jobs[i] = NULL;
    }
    return jdb;
}

// Function to load jobs to memory
int loadJob(JobDatabase * jdb, int id, char * name) {
    if (jdb -> numJobs >= MAX_JOBS) {
        perror("Error: Job database is full.\n");
        return -1;
    }

    // Check if a job with the given name already exists
    for (int i = 0; i < jdb -> numJobs; i++) {
        if (strcmp(jdb -> jobs[i] -> name, name) == 0) {
            perror("Error: Job with this name already exists.\n");
            return -1;
        }
    }

    Job * job = malloc(sizeof(Job));
    if (job == NULL) {
        perror("Error allocating memory for job\n");
        return -1;
    }

    job -> id = id;
    strncpy(job -> name, name, MAX_STR);
    job -> name[MAX_STR - 1] = '\0'; // Ensure null termination
    jdb -> jobs[jdb -> numJobs] = job;
    jdb -> numJobs++;
    return 0;
}

// Function to read the job database's contents from a CSV file
int readJobDatabaseFromCSV(JobDatabase * jdb,
    const char * filename) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file\n");
        return -1;
    }

    char name[MAX_STR];
    int id;

    while (fscanf(file, "%d,%[^,\n]\n", & id, name) == 2) {
        if (loadJob(jdb, id, name) != 0) {
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0; // Return 0 to indicate success
}

char * getJobName(JobDatabase * jdb, int id) {
    for (int i = 0; i < jdb -> numJobs; i++) {
        if (jdb -> jobs[i] -> id == id) {
            return jdb -> jobs[i] -> name;
        }
    }
    perror("No job with the given ID is found\n");
    return NULL; // Return NULL if no job with the given ID is found
}

// Turn job name into job id
int getJobID(JobDatabase * jdb, char * name) {
    for (int i = 0; i < jdb -> numJobs; i++) {
        if (strcmp(jdb -> jobs[i] -> name, name) == 0) {
            return jdb -> jobs[i] -> id;
        }
    }
    perror("No job with the given name is found \n");
    return -1; // Return -1 if no job with the given name is found
}

void printAllJobs(JobDatabase * jdb) {
    if (jdb == NULL || jdb -> numJobs == 0) {
        printf("No jobs available in the database.\n");
        return;
    }

    printf("List of Jobs:\n");
    for (int i = 0; i < jdb -> numJobs; i++) {
        if (jdb -> jobs[i] != NULL) {
            printf("Job ID: %d, Name: %s\n", jdb -> jobs[i] -> id, jdb -> jobs[i] -> name);
        }
    }
}

void freeJobDatabase(JobDatabase * jdb) {
    if (jdb == NULL) return;

    for (int i = 0; i < jdb -> numJobs; i++) {
        free(jdb -> jobs[i]);
    }

    free(jdb);
}

void printJobDistribution(CharacterDatabase * db, JobDatabase * jdb) {
    int jobCounts[MAX_JOBS] = {
        0
    };
    for (int i = 0; i < db -> numCharacters; i++) {
        if (db -> characters[i] -> job_id >= 0 && db -> characters[i] -> job_id < MAX_JOBS) {
            jobCounts[db -> characters[i] -> job_id]++;
        }
    }
    for (int j = 0; j < jdb -> numJobs; j++) {
        printf("Job: %s, Number of Characters: %d, Percentage: %.2f%%\n",
            jdb -> jobs[j] -> name, jobCounts[j], 100.0 * jobCounts[j] / db -> numCharacters);
    }
}
