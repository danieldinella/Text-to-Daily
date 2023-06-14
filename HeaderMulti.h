#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int     dist, width, height, columns;   // Parametri di ingresso iniziali 
int     readPipe[2], printPipe[2];      // Pipe per i processi
FILE    *fin, *fout;    // File d'ingresso e d'uscita
int     wlimit;         // Caratteri rimanenti che entrano nella riga in questione 
char    ***page;        // Puntatore alle righe
int     *tot_offset;    // Array di contatori di parole delle linee
char    next = '\0';    // Prossimo carattere del file

/* Funzione per la lettura di una nuova parola
   Input: puntatore a stringa su cui caricare la parola*/
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
          numero della colonna
          prossimo carattere del file*/
void add_space(char **line, int offset, int h, int c, char next){

    // Setto l'ultimo a carattere a NULL solo se non mi trovo alla fine del file altrimenti diminuisco wlimit
    if(line[offset][(int)strlen(line[offset])-1] == '\n' || line[offset][(int)strlen(line[offset])-1] == ' '){
        line[offset][(int)strlen(line[offset])-1] = '\0';
    }  else {
        wlimit--;
    }

    /* Se il prossimo carattere è \n o c'è solo una parola nella riga
    allora inserisco tutti gli spazi necessari a completare la riga */
    if(( next == '\n') || offset == 0){
        while (wlimit >= 0){
            strcat(line[offset]," ");
            wlimit--;
        }
    } else {
        //Calcolo gli spazi che vanno inseriti tra ogni parola
        int spaces = wlimit/offset;

        //Inserisco tra le parole il numero di spazi calcolato
        if (spaces > 0){
            for (int j = 0; j < offset; j++){
                for (int s = 0; s < spaces; s++){
                    strcat(line[j]," ");
                    wlimit--;
                }
            }
        }

        //Se c'è ancora spazio inserisco altri spazi fino a esaurimento
        for (int j = 0; wlimit >= 0; j++){
                strcat(line[j]," ");
                wlimit--;
        }
    }
    
    //Setto l'ultimo carattere dell'ultima parola come a capo
    if(h+1 < height && ((int)next != -1 || c != 0)){
        
        strcat(line[offset],"\n");
    }
    return;
}

/* Funzione per la lettura di una nuova riga
   Input: puntatore alla linea
          contatore di parole della linea*/
void new_line(char **line, int *offset){
    wlimit = width;
    while (true)
    {
        // Ottengo la nuova parola e la sua lunghezza in caratteri
        line[*offset] = (char *)malloc(width * sizeof(char *));
        new_word(line[*offset]);
        int length = my_strlen(line[*offset]);

        // Se l'ultima parola scansionata non entra nella riga resetto l'ultima operazione
        if (wlimit < length-1){
            fseek(fin,-strlen(line[*offset]),SEEK_CUR);
            *offset-=1;
            break;
        }

        wlimit -= length;
            
        // Se il file è finito o l'ultima parola scansionata termina a capo allora interrompo la scansione
        if (feof(fin) || line[*offset][(int)strlen(line[*offset])-1] == '\n'){
            break;
        }

        *offset+=1;
    }

    return;
}

/* Funzione per distanziare correttamente le nuove colonne
   Input: la riga della nuova colonna da aggiungere alla riga preesistente,
          numero della riga
          numero di stringhe contenute nella nuova riga*/
void set_new_column(char **line, int h, int offset){

    // Setto l'ultimo carattere della riga a \0 e da li aggiungo gli spazi richiesti da input
    if (h+1 != height){
        page[h][tot_offset[h]][(int)strlen(page[h][tot_offset[h]])-1] = '\0';
    }
    for (int j = 0; j < dist; j++){
        strcat(page[h][tot_offset[h]]," ");
    }

    // Inserisco la nuova riga
    for(int i = 0; i <= offset; i++){
        tot_offset[h]++;
        page[h][tot_offset[h]] = line[i];
    } 
    return;
}