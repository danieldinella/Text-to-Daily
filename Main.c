#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Header.h"

// Funzione per la lettura del file input e scrittura del file output
void read_n_print()
{
    //Sezione dedicata alla lettura
    page = (char ***)malloc(height * sizeof(char *));   // Puntatore alla pagina
    offset = (int *)malloc(height*sizeof(int *));   // Array che conta il numero di parole inserito in ogni riga
    for (int c = 0; c < columns; c++) {
        if(feof(fin))
            break;
        for(int k = 0; k < height; k++)
        {
            if(feof(fin))
                break;
            if (c == 0){
                page[k] = (char **)malloc(columns*(width+dist)*sizeof(char *)); // Alloco la memoria per la nuova riga
                offset[k] = 0;  // Setto il contatore di parole per la riga
            } else {
                set_new_column(page[k],k);
            }
            new_line(page[k],k);
        }
    }


    // Sezione dedicata alla stampa
    for (int k = 0; k < height; k++){
        
        if (page[k] == NULL)
            break;

        for (int j = 0; j <= offset[k]; j++){
            
            if (page[k][j]== NULL)
                break;
        
            fprintf(fout,"%s",page[k][j]);
        }
    }
    return;
}

int main()
{
    // Inizializzazione parametri e file
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt","w");

    printf("Inserire i parametri e premere invio:\n");

    printf("Righe di ogni colonna: ");
    scanf("%d", &height);

    printf("Caratteri per ogni colonna: ");
    scanf("%d", &width);

    printf("Numero massimo di colonne per pagina: ");
    scanf("%d", &columns);

    printf("Distanza tra ogni colonna: ");
    scanf("%d", &dist);

    while(!feof(fin)){
        read_n_print();
        if (!feof(fin)){
            fprintf(fout,"%s","\n\n%%%\n\n");
        }
    }

    // Chiusura dei file
    fclose(fin);
    fclose(fout);

    return 0;
}