#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "logs.h"

#define true 1
#define false 0

void printList (bikeList *list){

    bikeNode *temp;

    temp = list->first;

    while (temp->next!=NULL){
        fprintf(stdout, "%d: %s", temp->pos, temp->type);
    }

}

int main(int argc, char *argv[])
{
        int op;
        int quit = false;

    bikeNode vetor[LINESIZE + 1];
    checkInput(argc);

    bikeList *list;
    list = malloc(sizeof(bikeList));
    listInit(list);

    bikeNode *bike;
    bike = malloc(sizeof(bikeNode));

    printf("Loading logs in list");
    bike = loadLogs(argc, argv, list, vetor);

    while (!quit)
    {
        printf("Pressione 0 para sair do programa\n");
        scanf("%d", &op);
        switch (op)
        {
        case 0:
            quit = true;
            free(bike);
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

void insertNode(bikeNode *newNode, bikeNode *bike, bikeList *list, bikeNode vet[])
{

    bikeNode *temp = bike;
    bikeInit(newNode);
    newNode->pos = bike->pos++;
    newNode->next = NULL;

    // if the list is empty
    if (bike == NULL)
    {
        bikeInit(bike);
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

void readLog(bikeNode *bike, bikeList *list, bikeNode vetor[])
{
    char c,
        *infoType = malloc(sizeof(char)),
        *date = malloc(sizeof(char)),
        *altString = malloc(sizeof(char)),
        *distance = malloc(sizeof(char)),
        *other = malloc(sizeof(char));

    float newAltitude = 0,
          oldAltitude = 0;
    int block = 0; // conta o número de blocos dentro de um arquivo;

    FILE *bikeLog;
    bikeLog = fopen(bike->filePath, "r");
    checkFileOpening(bikeLog);

    bikeNode *newNode;
    newNode = malloc(sizeof(bikeNode));
    insertNode(newNode, bike, list, vetor);

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
            // converter para formato de data conforme enunciado
        }
        else if (strcmp(infoType, "altitude") == 0)
        {
            block++; // incrementa o número de blocos do arquivo

            fscanf(bikeLog, "%[^\n]", altString);

            altString[strcspn(altString, " ")] = '\0'; // retirando o espaço
            newAltitude = atof(altString);

            if (block >1 && ((newAltitude - oldAltitude) > 0))
            {
                bike->elev += (newAltitude - oldAltitude);
            }
            oldAltitude = newAltitude;
        }
        else if (strcmp(infoType, "distance") == 0)
        {
            fscanf(bikeLog, "%[^\n]", distance);
            // printf("%s: %s\n", infoType, distance);
        }
        else
        {
            fscanf(bikeLog, "%[^\n]", other);
            continue;
        }
    }
    printf("Numeros de blocos: %d\nSubida Acumulada: %.2f\n", block, bike->elev);
    fclose(bikeLog);

    list->end = bike;
    list->size ++;
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
        bike->elev = 0;
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

bikeNode *loadLogs(int argc, char *argv[], bikeList *list, bikeNode vetor[])
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

    int i = 0;

    // load directory
    while (((pDir = readdir(dir))) && i < 4)
    {
        if (pDir->d_type == isFILE)
        {
            printf("Arquivo: %s\n", pDir->d_name);
            getFilePath(bike->filePath, dirName, pDir->d_name);
            readLog(bike, list, vetor);
        }
        i++;
    }

    return bike;
}