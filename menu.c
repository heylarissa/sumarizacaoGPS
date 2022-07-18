#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "logs.h"

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


void checkInput(int argc)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./program directory_name\n");
        exit(EXIT_FAILURE);
    }
}