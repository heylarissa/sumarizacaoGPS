#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "logs.h"

bikeList *loadLogs(int argc, char *argv[])
{
    bikeNode *newNode;

    bikeList *list;
    list = malloc(sizeof(bikeList));
    listInit(list);

    DIR *dir;
    dir = opendir(argv[1]);
    struct dirent *pDir;
    checkDirectoryOpening(dir);

    char dirName[LINESIZE + 1];
    strcpy(dirName, argv[1]);

    // load directory
    fprintf(stdout, "Reading directory");
    while (((pDir = readdir(dir))))
    {
        if (pDir->d_type == isFILE)
        {
            newNode = malloc(sizeof(bikeNode));
            getFilePath(newNode->filePath, dirName, pDir->d_name);
            readLog(newNode, list);
        }
    }

    return list;
}
void getFilePath(char filePath[], char dirName[], char fileName[])
{
    memset(filePath, 0, sizeof(char));
    strcpy(filePath, "./");
    strcat(filePath, dirName);
    strcat(filePath, "/");
    strcat(filePath, fileName);
}
void checkDirectoryOpening(DIR *dir)
{
    if (dir == NULL)
    {
        perror("Cannot open directory");
        exit(EXIT_FAILURE);
    }
}
void checkFileOpening(FILE *arq)
{
    if (!arq)
    {
        perror("Erro ao abrir arquivo");
    }
}

void ignoreEnter(FILE *arq)
{
    char c = fgetc(arq);
    while (c == '\n')
    {
        c = fgetc(arq);
    }
    ungetc(c, arq);
}

void insertNode(bikeNode *newNode, bikeList *list)
{
    bikeNode *temp;

    // list is empty?
    if (list->first == NULL)
    {
        list->first = newNode;
        list->first->next = NULL;
        list->pos++;
        return;
    }

    temp = list->first;

    // insert new node
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->next = NULL;
    list->end = newNode;

    list->pos++;
}


void readLog(bikeNode *newNode, bikeList *list)
{
    char c,
        *infoType = malloc(sizeof(char)),

        *distance = malloc(sizeof(char)),
        *other = malloc(sizeof(char));

    char *altitude = malloc(sizeof(char));
    float newAltitude = 0,
          oldAltitude = 0,
          newDistance = 0,
          oldDistance = 0;

    int block = 0; // conta o número de blocos dentro de um arquivo;

    FILE *bikeLog;
    bikeLog = fopen(newNode->filePath, "r");
    checkFileOpening(bikeLog);

    // incializar o nodo
    bikeInit(newNode);
    fprintf(stdout, "Reading log: %s \n", newNode->filePath);

    // inserir os dados
    while (!feof(bikeLog))
    {
        memset(infoType, 0, strlen(infoType));
        ignoreEnter(bikeLog);

        fscanf(bikeLog, "%[^:]", infoType);

        c = fgetc(bikeLog);
        c = fgetc(bikeLog);

        // check value type
        if (strcmp(infoType, "Gear") == 0)
        {
            fscanf(bikeLog, "%[^\n]", newNode->type);
        }
        else if (strcmp(infoType, "Date") == 0)
        {
            fscanf(bikeLog, "%[^\n]", newNode->date);
            // TO DO: converter para formato de data conforme enunciado
        }
        else if (strcmp(infoType, "altitude") == 0)
        {
            block++; // incrementa o número de blocos do arquivo
                     // newNode->elev = subidaAcumuladaCalc(bikeLog, block);

            fscanf(bikeLog, "%[^\n]", altitude);

            altitude[strcspn(altitude, " ")] = '\0'; // retirando o espaço e a unidade
            newAltitude = atof(altitude);

            if (block > 1 && ((newAltitude - oldAltitude) > 0))
            {
                newNode->elev += (newAltitude - oldAltitude);
            }
            oldAltitude = newAltitude;
        }
        else if (strcmp(infoType, "distance") == 0)
        {
            fscanf(bikeLog, "%[^\n]", distance);
            distance[strcspn(distance, " ")] = '\0';
            newDistance = atof(distance);
            if (block == 1)
                oldDistance = newDistance;

            newNode->distance += (newDistance - oldDistance);
        }

        else
        {
            fscanf(bikeLog, "%[^\n]", other);
            continue;
        }
    }

    newNode->distance = newNode->distance / 1000; // convertendo de metros para km

    fclose(bikeLog);

    // inserir na lista
    insertNode(newNode, list);

    printf("\n");
}

void bikeInit(bikeNode *newNode)
{
    if (!newNode)
    {
        fprintf(stderr, "Memória insuficiente\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        newNode->next = NULL;
        newNode->elev = 0;
        newNode->distance = 0;
    }
}

void listInit(bikeList *list)
{
    list->first = NULL;
    list->end = NULL;
    list->pos = 0;
}

