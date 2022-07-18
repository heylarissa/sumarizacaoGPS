#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "logs.h"

#define true 1
#define false 0

void printList(bikeList *list)
{
    bikeNode *inicio;
    inicio = list->first;
    int i = 0;

    while (inicio != NULL)
    {
        fprintf(stdout, "%d      path: %s        %s        data:%s       sub_acumulada: %.2f               distance:%.2f\n",
                i++,
                inicio->filePath,
                inicio->type,
                inicio->date,
                inicio->elev,
                inicio->distance);

        inicio = inicio->next;
    }
}

void searchBike(bikeList *list, char *bikeName)
{
    int i = 0;
    bikeNode *thisBike;

    thisBike = list->first;

    while (thisBike != NULL)
    {
        if (strcmp(thisBike->type, bikeName) == 0)
        {
            fprintf(stdout, "path: %s        %s        data:%s       sub_acumulada: %.2f               distance:%.2f\n",

                    thisBike->filePath,
                    thisBike->type,
                    thisBike->date,
                    thisBike->elev,
                    thisBike->distance);
            i++;
        }
        thisBike = thisBike->next;
    }
    if (i==0){
        fprintf(stdout, "\nNão existem bicicletas com esse nome\n");
        return;
    }
}

int main(int argc, char *argv[])
{
    int op;
    char thisBike[20];
    int quit = false;

    checkInput(argc);

    bikeList *list;

    printf("Loading logs in list");
    list = loadLogs(argc, argv);

    while (!quit)
    {
        fprintf(stdout, "\n\n----------MENU----------\n\n0: Sair do programa\n1: Mostra todas as bicicletas encontradas durante o processamento dos arquivos de log.\n2: Pede para o usuário informar uma das bicicletas encontradas e apresenta a lista de atividades, resumo conforme descrito acima.\n\n");
        scanf("%d", &op);
        switch (op)
        {
        case 0:
            quit = true;
            free(list);
            break;

        case 1:

            printList(list);
            break;

        case 2:
            fprintf(stdout, "Leitura de input não implementada. Somente demonstração de função\nDigite o nome da bicicleta\n");
            strcpy(thisBike, "GT Grade");
            printf("GT Grade\n");
            searchBike(list, thisBike);
            // 2 Pede para o usuário informar uma das bicicletas encontradas e apresenta a lista de atividades, resumo conforme descrito acima.
            break;
        case 3:
            fprintf(stdout, "Não implementado\n");
            // 3 Lista todas atividades agrupadas por bicicleta e ordenadas pela data (quicksort)
            break;
        case 4:
            fprintf(stdout, "Não implementado\n");

            // 4 Lista todas atividades agrupadas por bicicleta e ordenadas pela distância (quicksort)
            break;
        case 5:
            fprintf(stdout, "Não implementado\n");

            // 5 Lista todas atividades ordenadas pela subida acumulada (quick sort)
            break;
        case 6:
            fprintf(stdout, "Não implementado\n");
            // 6 Histograma: O usuário deve escolher uma bicicleta e apresentar um histograma
            break;
        default:
            printf("Voce deve escolher uma opcao valida\n");
            break;
        }
    }
}

void checkFileOpening(FILE *arq)
{
    if (!arq)
    {
        perror("Erro ao abrir arquivo");
    }
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

void ignoreEnter(FILE *arq)
{
    char c = fgetc(arq);
    while (c == '\n')
    {
        c = fgetc(arq);
    }
    ungetc(c, arq);
}

float subidaAcumuladaCalc(FILE *bikeLog, int block)
{
    char *altitude = malloc(sizeof(char));
    float newAltitude = 0,
          oldAltitude = 0,
          subidaAcumulada = 0;

    fscanf(bikeLog, "%[^\n]", altitude);

    altitude[strcspn(altitude, " ")] = '\0'; // retirando o espaço e a unidade
    newAltitude = atof(altitude);

    if (block > 1 && ((newAltitude - oldAltitude) > 0))
    {
        subidaAcumulada += (newAltitude - oldAltitude);
    }
    oldAltitude = newAltitude;
    return (subidaAcumulada);
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

void getFilePath(char filePath[], char dirName[], char fileName[])
{
    memset(filePath, 0, sizeof(char));
    strcpy(filePath, "./");
    strcat(filePath, dirName);
    strcat(filePath, "/");
    strcat(filePath, fileName);
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