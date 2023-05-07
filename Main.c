#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Header.h"

// Funzione per la lettura del file input e scrittura del file output
void read_n_print()
{

    /*
    int length = 0;     // Lunghezza della nuova parola scansionata
    int limit = width;  // Caratteri rimanenti in una riga
    char last = "\0";   // Ultima parola scansionata troppo lunga per la riga    
    */

    while (!feof(fin))
    {

        char **line = (char **)malloc(width * sizeof(char *)); // Alloco la memoria per la nuova riga
        int i = 0;

        while (true)
        {
            line[i] = new_word();
            i++;
        }

        //Scrivo la riga sul file
        for (int j = 0; j < (int)strlen(line[i]); j++){
            if (line[i][j] == '\0')
                break;
            else
                fprintf(fout, "%c", line[i][j]);
        }

        free(line); // Libero la memoria della riga
    }

    // Chiusura dei file
    fclose(fin);
    fclose(fout);
}

int main()
{

    // Inizializzazione parametri e file
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    printf("Inserire j parametri e premere invio:\n");

    printf("Righe di ogni colonna: ");
    scanf("%d", &lines);

    printf("Caratteri per ogni colonna: ");
    scanf("%d", &width);

    printf("Numero massimo di colonne per pagina: ");
    scanf("%d", &columns);

    printf("Distanza tra ogni colonna: ");
    scanf("%d", &dist);

    read_n_print();

    return 0;
}