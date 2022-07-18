#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "logs.h"

int main(int argc, char *argv[])
{
    int op;
    char thisBike[LINESIZE+1];
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
            //strcpy(thisBike, "GT Grade");
            //scanf ( "%[^\n]", thisBike);
            // fgets(thisBike, sizeof(thisBike), stdin);
            printf("GT Grade\n");
            searchBike(list, thisBike);
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
