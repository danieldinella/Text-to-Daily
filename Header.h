#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int dist, width, height, columns, wlimit;
bool isFirstColumn = true;
char ***page;
int *offset;
FILE *fin, *fout;

// Funzione per la lettura di una nuova parola
void new_word(char *word){
    //Scorro il file fino alla fine di una nuova parola
    for (int j = 0; !feof(fin) && fscanf(fin, "%c", &word[j]) != 0; j++){          
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

    // Se nella riga c'è solo una parola inserisco gli spazi per facilitare gli incolonnamenti successivi
    if (i == 0){
        line[offset[k]][strlen(line[offset[k]])-1] = ' ';
        if (hlimit >= 0){ 
            while (hlimit >= 0){
                strcat(line[offset[k]]," ");
                hlimit--;
            }
        }
    }

    // Se nella riga ci sono due parole e l'ultima è un accapo allora si tratta di un fine paragrafo
    else if (line[offset[k]] == "\n" && i == 1){
        line[offset[k]-1][strlen(line[offset[k]-1])-1] = ' ';
        if (hlimit >= 0){ 
            while (hlimit >= 0){
                strcat(line[offset[k]-1]," ");
                hlimit--;
            }
        }
    }
    
    else {
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
        if (hlimit >= 0){ 
            for (int j = offset[k]-i; hlimit >= 0; j++){
                    strcat(line[j]," ");
                    hlimit--;
            }
        }
    }

    //Setto l'ultimo carattere dell'ultima parola come a capo
    line[offset[k]][strlen(line[offset[k]])-1] = '\n';
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
        if (feof(fin) || line[offset[k]][length-1] == '\n'){
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
    page[k][offset[k]][(int)strlen(page[k][offset[k]])-1] = '\0';
    page[k][offset[k]+1] = (char *)malloc(width*sizeof(char *));
    for (int j = 0; j < dist; j++){
        strcat(page[k][offset[k]+1]," ");
    }
    offset[k]+=2; 
    return;
}