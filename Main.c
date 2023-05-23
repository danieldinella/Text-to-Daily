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
    while(!feof(fin)){
        for(int k = 0; k < height; k++)
        {
            if(feof(fin))
                break;

            if (isFirstColumn){
                page[k] = (char **)malloc(columns*(width+dist)*sizeof(char *)); // Alloco la memoria per la nuova riga
                offset[k] = 0;  // Setto il contatore di parole per la riga
            } else {
                set_new_column(page[k],k);
            }
            new_line(page[k],k);
        }
        isFirstColumn = false;
    }

    // Sezione dedicata alla stampa
    bool end;
    for (int k = 0; k < height; k++){
        end = false;
        
        if (page[k] == NULL)
            break;

        for (int j = 0; end == false; j++){
            
            if (page[k][j]== NULL)
                break;

            if (page[k][j][my_strlen(page[k][j])-1] == '\n')
                end = true;
        
            fprintf(fout,"%s",page[k][j]);
        }
    }

}

int main()
{
    // Inizializzazione parametri e file
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    printf("Inserire i parametri e premere invio:\n");

    printf("Righe di ogni colonna: ");
    scanf("%d", &height);

    printf("Caratteri per ogni colonna: ");
    scanf("%d", &width);

    printf("Numero massimo di colonne per pagina: ");
    scanf("%d", &columns);

    printf("Distanza tra ogni colonna: ");
    scanf("%d", &dist);

    read_n_print();

    // Chiusura dei file
    fclose(fin);
    fclose(fout);

    return 0;
}