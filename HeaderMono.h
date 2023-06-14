#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int dist, width, height, columns, wlimit;
char ***page;
int *offset;
FILE *fin, *fout;

/* Funzione per la lettura di una nuova parola
   Input: puntatore alla stringa dove inserire la nuova parola*/
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

/* Funzione per ottenere la lunghezza di una parola in caratteri e non in byte
   Input: puntatore a stringa di cui misurare la lunghezza
   Output: lunghezza della parola calcolata in caratteri*/
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

/* Funzione per inserire gli spazi necessari per giustificare ogni riga
   Input: puntatore di puntatori a stringhe in cui inserire gli spazi,
          numero di stringhe contenute nella riga,
          numero della riga,
          numero della colonna*/
void justify(char **line, int i, int h, int c){
    // Creo una variabile d'appoggio per ottenere il prossimo carattere
    char x = fgetc(fin);
    if(!feof(fin)){
        fseek(fin,-sizeof(char),SEEK_CUR);
    }

    // Setto l'ultimo a carattere a NULL solo se non mi trovo alla fine
    if(line[offset[h]][strlen(line[offset[h]])-1] == '\n' || line[offset[h]][strlen(line[offset[h]])-1] == ' '){
        line[offset[h]][strlen(line[offset[h]])-1] = '\0';
    }

    /* Se il prossimo carattere è \n o c'è solo una parola nella riga
    allora inserisco tutti gli spazi necessari a completare la riga */
    if(( x == '\n') || i == 0){
        while (wlimit >= 0){
            strcat(line[offset[h]]," ");
            wlimit--;
        }
    } else {
        //Calcolo gli spazi che vanno inseriti tra ogni parola
        int spaces = wlimit/i;

        //Inserisco tra le parole il numero di spazi calcolato
        if (spaces > 0){
            for (int j = offset[h]-i; j < offset[h]; j++){
                for (int s = 0; s < spaces; s++){
                    strcat(line[j]," ");
                    wlimit--;
                }
            }
        }

        //Se c'è ancora spazio inserisco altri spazi fino a esaurimento
        for (int j = offset[h]-i; wlimit >= 0; j++){
                strcat(line[j]," ");
                wlimit--;
        }
    }
    
    //Setto l'ultimo carattere dell'ultima parola come a capo
    if(h+1 != height && (!feof(fin) || c != 0))
        strcat(line[offset[h]],"\n");
    
    return;
}

/* Funzione per la lettura di una nuova riga
   Input: puntatore alla linea
          numero della riga
          numero della colonna*/
void new_line(char **line, int h, int c){
    wlimit = width;
    int i = 0;  // Contatore delle parole nella riga che fanno parte solo di questa colonna
    while (true)
    {
        // Ottengo la nuova parola e la sua lunghezza in caratteri
        line[offset[h]] = (char *)malloc(width * sizeof(char *));
        new_word(line[offset[h]]);
        int length = my_strlen(line[offset[h]]);

        // Se l'ultima parola scansionata non entra nella riga resetto l'ultima operazione
        if (wlimit < length-1){
            fseek(fin,-strlen(line[offset[h]]),SEEK_CUR);
            offset[h]--;
            i--;
            break;
        }

        wlimit -= length;
            
        // Se il file è finito o l'ultima parola scansionata termina a capo allora interrompo la scansione
        if (feof(fin) || line[offset[h]][(int)strlen(line[offset[h]])-1] == '\n'){
            break;
        }

        offset[h]++;
        i++;
    }

    justify(line, i, h, c);
    return;
}

/* Funzione per distanziare correttamente le nuove colonne
   Input: numero della riga*/
void set_new_column(int h){
    // Setto l'ultimo carattere della riga a \0 e da li aggiungo gli spazi richiesti da input
    if (h+1 != height)
        page[h][offset[h]][(int)strlen(page[h][offset[h]])-1] = '\0';
    page[h][offset[h]+1] = (char *)malloc(width*sizeof(char *));
    for (int j = 0; j < dist; j++){
        strcat(page[h][offset[h]+1]," ");
    }
    offset[h]+=2; 
    return;
}