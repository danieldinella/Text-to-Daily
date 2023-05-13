#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Header.h"

// Funzione per la lettura del file input e scrittura del file output
void read_n_print()
{

    int limit;  // Caratteri rimanenti in una riga

    while (!feof(fin))
    {

        char **line = (char **)malloc(width * sizeof(char *)); // Alloco la memoria per la nuova riga
        int i = 0;
        limit = width;

        while (true)
        {
            line[i] = new_word();

            //Se l'ultima parola scansionata non entra nella riga resetto l'ultima operazione
            if (limit < (int)strlen(line[i])-1){
                fseek(fin,-(int)strlen(line[i]),SEEK_CUR);
                i--;
                break;
            }

            limit -= (int)strlen(line[i]);
            
            if (feof(fin))
                break;
            
            //Se l'ultima parola scansionata termina a capo allora interrompo la scansione
            if (line[i][(int)strlen(line[i])-1] == '\n'){
                break;
            }

            i++;
        }

        justify(line, limit, i);

        //Scrivo la riga sul file
        for (int j = 0; j <= i; j++){
            fprintf(fout, "%s", line[j]);
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