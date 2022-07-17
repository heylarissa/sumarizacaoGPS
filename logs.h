
#ifndef __LOGS__
#define __LOGS__

#define LINESIZE 1024
#define isFILE 8

typedef struct node
{
    char filePath[LINESIZE + 1],
        type[LINESIZE + 1],
        *date;
    int pos;
    struct node *next;

} bikeNode;

typedef struct list {
    bikeNode *first, *end;
    int size;
} bikeList;

void checkFileOpening(FILE *arq);
void readLog(bikeNode *bike, bikeList *list);
void listInit(bikeList *list);

void getFilePath(char filePath[], char *argv, char dirName[]);

void checkInput(int argc);

void checkDirectoryOpening(DIR *dir);

bikeNode *loadLogs(int argc, char *argv[], bikeList *list);


void bikeInit(bikeNode *bike);
#endif