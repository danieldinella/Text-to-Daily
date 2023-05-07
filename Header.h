#include <stdio.h>
#include <stdlib.h>

int dist, width, lines, columns, line_l;
FILE *fin, *fout;

char* new_word(){
    //Scorro il file fino alla fine di una nuova parola
    char *word = (char *)malloc(width * sizeof(char));
    int j;
    for (j = 0; fscanf(fin, "%c", &word[j]) != 0; j++){          
        if (word[j] == ' ' || word[j] == '\n')                  
            break;
    }
    //Se la parola termina con uno spazio lo rimuovo
    if (word[j] == ' ')
        word[j] = '\0';

    return word;
}