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
    inicio = malloc(sizeof(bikeList));
    inicio = list->first;
    while (inicio != NULL)
    {
        fprintf(stdout, "path: %s    bike: %s    data:%s    sub_acumulada: %.2f    distance:%.2f\n",
                inicio->filePath,
                inicio->type,
                inicio->date,
                inicio->elev,
                inicio->distance);

        inicio = inicio->next;
    }
}

int main(int argc, char *argv[])
{
    int op;
    int quit = false;

    checkInput(argc);

    bikeList *list;
    list = malloc(sizeof(bikeList));
    listInit(list);

    printf("Loading logs in list");
    list = loadLogs(argc, argv);

    while (!quit)
    {
        printf("Pressione 0 para sair do programa\n");
        scanf("%d", &op);
        switch (op)
        {
        case 0:
            quit = true;
            free(list);
            break;

        case 1:

            printList(list);

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
    bikeList *temp;

    // list is empty?
    if (list->first == NULL)
    {
        list->first = newNode;
        list->pos++;
        return;
    }

    temp = list;

    // insert new node
    while (temp->first != NULL)
    {
        temp->first = temp->first->next;
    }

    temp->first->next = newNode;
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

    int block = 0; // conta o número de blocos dentro de um arquivo;

    FILE *bikeLog;
    bikeLog = fopen(newNode->filePath, "r");
    checkFileOpening(bikeLog);

    // incializar o nodo
    bikeInit(newNode);

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
            fprintf(stdout, "Bicicleta: %s \n", newNode->type);
        }
        else if (strcmp(infoType, "Date") == 0)
        {
            fscanf(bikeLog, "%[^\n]", newNode->date);
            fprintf(stdout, "Data: %s\n", newNode->date);
            // converter para formato de data conforme enunciado
        }
        else if (strcmp(infoType, "altitude") == 0)
        {
            block++; // incrementa o número de blocos do arquivo
            newNode->elev = subidaAcumuladaCalc(bikeLog, block);
        }
        else if (strcmp(infoType, "distance") == 0)
        {
            fscanf(bikeLog, "%[^\n]", distance);
            distance[strcspn(distance, " ")] = '\0';
            newNode->distance += atof(distance);
        }
        /*
         else if (strcmp(infoType, "cadence") == 0)
         {
             cadenciaMedia(bikeLog);
             fscanf(bikeLog, "%[^\n]", distance);
         } */

        else
        {
            fscanf(bikeLog, "%[^\n]", other);
            continue;
        }
    }

    newNode->distance = newNode->distance / 1000; // convertendo de metros para km

    fprintf(stdout, "Subida Acumulada: %.2f\nDistância: %.2f\n", newNode->elev, newNode->distance);
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

bikeList *loadLogs(int argc, char *argv[])
{
    bikeNode *newNode;
    newNode = malloc(sizeof(bikeNode));

    bikeList *list;
    list = malloc(sizeof(bikeList));
    listInit(list);

    DIR *dir;
    dir = opendir(argv[1]);
    struct dirent *pDir;
    checkDirectoryOpening(dir);

    char dirName[LINESIZE + 1];
    strcpy(dirName, argv[1]);

    int i = 0;

    // load directory
    while (((pDir = readdir(dir))) && i < 4)
    {
        if (pDir->d_type == isFILE)
        {
            fprintf(stdout, "Arquivo: %s\n", pDir->d_name);
            getFilePath(newNode->filePath, dirName, pDir->d_name);
            readLog(newNode, list);
        }
        i++;
    }

    return list;
}