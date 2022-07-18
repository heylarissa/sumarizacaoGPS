
#ifndef __LOGS__
#define __LOGS__

#include <dirent.h>
#include <stdio.h>

#define LINESIZE 1024
#define isFILE 8
#define true 1
#define false 0

typedef struct node
{
    char filePath[LINESIZE + 1],
        type[LINESIZE + 1],
        date[LINESIZE+1];
    float elev,
          distance;
    struct node *next;

} bikeNode;

typedef struct list {
    bikeNode *first, *end;
    int pos;
} bikeList;

void checkFileOpening(FILE *arq);
void readLog(bikeNode *newNode, bikeList *list);
void listInit(bikeList *list);

void getFilePath(char filePath[], char *argv, char dirName[]);

void checkInput(int argc);

void checkDirectoryOpening(DIR *dir);
bikeList *loadLogs(int argc, char *argv[]);

void printList(bikeList *list);
void searchBike(bikeList *list, char *bikeName);
void bikeInit(bikeNode *bike);
#endif