#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "logs.h"

#define true 1
#define false 0

int main(int argc, char *argv[])
{
    /*     int op;
        int quit = false; */

    checkInput(argc);

    bikeList *list;
    list = malloc(sizeof(bikeList));
    listInit(list);

    bikeNode *bike;
    bike = malloc(sizeof(bikeNode));
    bikeInit(bike);

    bike = loadLogs(argc, argv, list);
    /* while (!quit)
    {
        scanf("%d", &op);
        switch (op)
        {
        case 0:
            quit = true;
            free(bike);
            break;

        case 1:

            // imprime todos os valores da lista
            // 1 Bicicletas Encontradas: Mostra todas as bicicletas encontradas durante o processamento dos arquivos de log.
            break;
        case 2:
            // busca binária (recursivo)
            // as bicicletas recebem um número, a busca será feita por este (valor da posição do elemento na lista)
            // 2 Pede para o usuário informar uma das bicicletas encontradas e apresenta a lista de atividades, resumo conforme descrito acima.
            break;
        case 3:
            // 3 Lista todas atividades agrupadas por bicicleta e ordenadas pela data (quicksort)
            break;
        case 4:
            // 4 Lista todas atividades agrupadas por bicicleta e ordenadas pela distância (quicksort)
            break;
        case 5:
            // 5 Lista todas atividades ordenadas pela subida acumulada (quick sort)
            break;
        case 6:
            // 6 Histograma: O usuário deve escolher uma bicicleta e apresentar um histograma
            break;
        default:
            printf("voce deve escolher uma opcao valida\n");
            break;
        }
    } */
}

void checkFileOpening(FILE *arq)
{
    if (!arq)
    {
        perror("Erro ao abrir arquivo");
    }
}

void insertNode(bikeNode *newNode, bikeNode *bike, bikeList *list)
{

    bikeNode *temp = bike;
    newNode->pos = bike->pos++;
    newNode->next = NULL;

    // if the list is empty
    if (bike == NULL)
    {
        bike = newNode;
        list->first = bike;
        list->size++;
        return;
    }

    // finding the last node
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
    bike = temp->next;
    list->end = bike;
    list->size++;
    printf("Novo nodo criado\n");
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

void readLog(bikeNode *bike, bikeList *list)
{
    char c,
        *infoType = malloc(sizeof(char)),
        *date = malloc(sizeof(char)),
        *subAcumulada = malloc(sizeof(char)),
        *distance = malloc(sizeof(char)),
        *other = malloc(sizeof(char));

    FILE *bikeLog;
    bikeLog = fopen(bike->filePath, "r");
    checkFileOpening(bikeLog);

    bikeNode *newNode;
    newNode = malloc(sizeof(bikeNode));
    insertNode(newNode, bike, list);

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
            fscanf(bikeLog, "%[^\n]", bike->type);
            printf("Bicicleta: %s \n", bike->type);
        }
        else if (strcmp(infoType, "Date") == 0)
        {
            fscanf(bikeLog, "%[^\n]", date);
            printf("Data: %s\n", date);
            // converter data para formato data conforme enunciado
        }
        else if (strcmp(infoType, "altitude") == 0)
        {
            fscanf(bikeLog, "%[^\n]", subAcumulada);
            printf("%s: %s\n", infoType, subAcumulada);
        }
        else if (strcmp(infoType, "distance") == 0)
        {
            fscanf(bikeLog, "%[^\n]", distance);
            printf("%s: %s\n", infoType, distance);
        }
        else
        {
            fscanf(bikeLog, "%[^\n]", other);
            //printf("%s: %s\n", infoType, other);
            // printf ("%s\n", infoType);
            continue;
        }
    }
    fclose(bikeLog);
    printf("\n");
}

void bikeInit(bikeNode *bike)
{
    if (!bike)
    {
        fprintf(stderr, "Memória insuficiente\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        bike->next = NULL;
        bike->pos = 0;
    }
}

void listInit(bikeList *list)
{
    list->first = NULL;
    list->end = NULL;
    list->size = 0;
}

void getFilePath(char filePath[], char dirName[], char fileName[])
{
    memset(filePath, 0, sizeof(char));
    strcpy(filePath, "./");
    strcat(filePath, dirName);
    strcat(filePath, "/");
    strcat(filePath, fileName);
    printf("%s\n", filePath);
}

void checkInput(int argc)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./program directory_name\n");
        exit(EXIT_FAILURE);
    }
}

void checkDirectoryOpening(DIR *dir)
{
    if (dir == NULL)
    {
        perror("Cannot open directory");
        exit(EXIT_FAILURE);
    }
}

bikeNode *loadLogs(int argc, char *argv[], bikeList *list)
{
    bikeNode *bike;
    bike = malloc(sizeof(bikeNode));
    bikeInit(bike);

    DIR *dir;
    dir = opendir(argv[1]);
    struct dirent *pDir;
    checkDirectoryOpening(dir);

    char dirName[LINESIZE + 1];
    strcpy(dirName, argv[1]);

    // load directory
    while ((pDir = readdir(dir)))
    {
        if (pDir->d_type == isFILE)
        {
            printf("Arquivo: %s\n", pDir->d_name);
            getFilePath(bike->filePath, dirName, pDir->d_name);
            readLog(bike, list);
        }
    }

    return bike;
}