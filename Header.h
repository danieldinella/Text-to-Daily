#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int dist, width, height, columns, wlimit;
char ***page;
int *offset;
FILE *fin, *fout;

// Funzione per la lettura di una nuova parola
void new_word(char *word){
    int j;
    // Scorro il file fino alla fine di una nuova parola
    for (j = 0; !feof(fin) && fscanf(fin, "%c", &word[j]) != 0; j++){ 
        // Se la parola inizia con uno spazio non la considero    
        if  (word[j] == ' ' && j == 0){
            j--;
            continue;
        }
        if (word[j] == ' ' || word[j] == '\n')                  
            break;
    }
    return;
}

// Funzione per ottenere la lunghezza di una parola in caratteri e non in byte
int my_strlen(char *word){

    int length = 0;
    int index = 0;
    while (word[index] != '\0') {
        if ((word[index] & 0xC0) != 0x80) {
            // Il byte corrente non fa parte di un carattere multibyte
            length++;
        }
        index++;
    }

    return length;
}

// Funzione per inserire gli spazi necessari per giustificare ogni riga
void justify(char **line, int hlimit, int i, int k){
    // Creo una variabile d'appoggio per ottenere il prossimo carattere
    char x = fgetc(fin);
    if(!feof(fin)){
        fseek(fin,-sizeof(char),SEEK_CUR);
    }

    if(line[offset[k]][strlen(line[offset[k]])-1] == '\n' || line[offset[k]][strlen(line[offset[k]])-1] == ' ')
        line[offset[k]][strlen(line[offset[k]])-1] = '\0';

    /* Se il prossimo carattere è \n o c'è solo una parola nella riga
    allora inserisco tutti gli spazi necessari a completare la riga */
    if(( x == '\n') || i == 0){
        while (hlimit >= 0){
            strcat(line[offset[k]]," ");
            hlimit--;
        }
    } else {
        //Calcolo gli spazi che vanno inseriti tra ogni parola
        int spaces = hlimit/i;

        //Inserisco tra le parole il numero di spazi calcolato
        if (spaces > 0){
            for (int j = offset[k]-i; j < offset[k]; j++){
                for (int s = 0; s < spaces; s++){
                    strcat(line[j]," ");
                    hlimit--;
                }
            }
        }

        //Se c'è ancora spazio inserisco altri spazi fino a esaurimento
        for (int j = offset[k]-i; hlimit >= 0; j++){
                strcat(line[j]," ");
                hlimit--;
        }
    }
    
    //Setto l'ultimo carattere dell'ultima parola come a capo
    if(k+1 != height && !feof(fin))
        strcat(line[offset[k]],"\n");
    
    return;
}

// Funzione per la lettura di una nuova riga
void new_line(char **line, int k){
    wlimit = width;
    int i = 0;  // Contatore delle parole nella riga che fanno parte solo di questa colonna
    while (true)
    {
        // Ottengo la nuova parola e la sua lunghezza in caratteri
        line[offset[k]] = (char *)malloc(width * sizeof(char *));
        new_word(line[offset[k]]);
        int length = my_strlen(line[offset[k]]);

        // Se l'ultima parola scansionata non entra nella riga resetto l'ultima operazione
        if (wlimit < length-1){
            fseek(fin,-strlen(line[offset[k]]),SEEK_CUR);
            offset[k]--;
            i--;
            break;
        }

        wlimit -= length;
            
        // Se il file è finito o l'ultima parola scansionata termina a capo allora interrompo la scansione
        if (feof(fin) || line[offset[k]][(int)strlen(line[offset[k]])-1] == '\n'){
            break;
        }

        offset[k]++;
        i++;
    }

    justify(line, wlimit, i, k);
    return;
}

// Funzione per distanziare correttamente le nuove colonne
void set_new_column(char **line,int k){
    // Setto l'ultimo carattere della riga a \0 e da li aggiungo gli spazi richiesti da input
    if (k+1 != height)
        page[k][offset[k]][(int)strlen(page[k][offset[k]])-1] = '\0';
    page[k][offset[k]+1] = (char *)malloc(width*sizeof(char *));
    for (int j = 0; j < dist; j++){
        strcat(page[k][offset[k]+1]," ");
    }
    offset[k]+=2; 
    return;
}