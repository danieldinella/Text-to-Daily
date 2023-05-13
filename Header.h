#include <stdio.h>
#include <stdlib.h>

int dist, width, lines, columns, line_l;
FILE *fin, *fout;

char* new_word(){

    char *word = (char *)malloc(width * sizeof(char));

    //Scorro il file fino alla fine di una nuova parola
    for (int j = 0; !feof(fin) && fscanf(fin, "%c", &word[j]) != 0; j++){          
        if (word[j] == ' ' || word[j] == '\n')                  
            break;
    }

    return word;
}


void justify(char **line, int limit, int i){

    //Se nella riga c'è solo una parola non faccio nulla
    if (i == 0)
        return;
    
    //Calcolo gli spazi che vanno inseriti tra ogni parola
    int spaces = limit/i;

    //Inserisco tra le parole il numero di spazi calcolato
    if (spaces > 0){
        for (int j = 0; j < i; j++){
            for (int k = 0; k < spaces; k++){
                strcat(line[j]," ");
                limit--;
            }
        }
    }

    //Se c'è ancora spazio inserisco altri spazi fino a esaurimento
    if (limit >= 0){ 
        for (int j = 0; limit >= 0; j++){
                strcat(line[j]," ");
                limit--;
        }
    }

    //Setto l'ultimo carattere dell'ultima parola come a capo
    line[i][(int)strlen(line[i])-1] = '\n';

    return;
}