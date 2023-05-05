#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Header.h"

//Funzione per la lettura del file input e scrittura del file output
void read_n_print(){
    
    int limit = width;  //Caratteri rimanenti in una riga
    
    while (!feof(fin)) {

        char *line = (char*) malloc(width * sizeof(char));  //Alloco memoria per la nuova parola

        //Scorro il file fino alla fine di una nuova parola
        int i;
        for (i = 0; fscanf(fin, "%c", &line[i]) != 0; i++){          
            if (line[i] == ' ' || line[i] == '\n')                  
                break;                                                  
        }

        //Se c'è spazio la inserisco nella riga 
        if ((limit - (int)strlen(line)) > 0){
            if (line[i] == '\n'){                
                limit = width;
            }
        } else if (limit - (int)strlen(line) == 0) {
                line[i] = '\n';
                limit = width;
        }
        //Altrimenti vado a capo e la inserisco nella riga dopo.
        else {
            if (line[i] != '\n')
                fputs("\n",fout);
            limit = width;
        }
        limit -= (int)strlen(line);
        for (i = 0; i < (int)strlen(line); i++){
            if (line[i] == '\0')                  
                break;
            else
                fprintf(fout,"%c",line[i]);                                         
        }

        free(line);     //Libero la memoria della parola

    }

    //Chiusura dei file
    fclose(fin);
    fclose(fout);
}


int main(){

    //Inizializzazione parametri e file
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    printf("Inserire i parametri e premere invio:\n");

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