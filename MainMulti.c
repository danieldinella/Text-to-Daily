#include "HeaderMulti.h"

// Funzione per la lettura del file input
void read_txt()
{
    while(!feof(fin)) {
        int offset = 0;         // Setto il contatore delle parole a 0
        char **line = (char **)malloc(columns*(width+dist)*sizeof(char *));     // Alloco la memoria per la nuova riga
        new_line(line, &offset);
        long int finCount = ftell(fin);     // Ottengo la distanzza dall'inizio del file per passarla al processo justify

        // Scrivo nella pipe del processo che giustifica i dati di cui ha bisogno
        write(readPipe[1],&wlimit,sizeof(int));
        write(readPipe[1],&offset,sizeof(int));
        for (int i = 0; i <= offset; i++){
            write(readPipe[1],line[i],width+1);
        }

        fseek(fin,finCount,SEEK_SET);
        next = fgetc(fin);
        write(readPipe[1],&next,sizeof(char));
        if(!feof(fin)){
            fseek(fin,finCount,SEEK_SET);
        }

        free(line);
    }
    return;
}

// Funzione per la giustificazione del testo
void justify_txt(){
    const char  *bp = "\n\n%%%\n\n";   // Separatore di fine pagina
    int         offset;         // Contatore delle parole della nuova riga (viene usato quando si va oltre la prima colonna)
    const long int value = 0;   // Valore d'appoggio da passare al posto di offset
    const char value2 = '\0';   // Valore d'appoggio da passare al posto di next
    tot_offset = (int *)malloc(height * sizeof(int));   // Array di contatori di parole delle linee
    page = (char ***)malloc(height * sizeof(char *));   // Puntatore alla pagina

    while((int)next != -1){
        int ph = 0;             // Contatore delle righe che si possono già stampare
        for(int c = 0; c < columns && (int)next != -1; c++){
            for(int h = 0; h < height && (int)next != -1; h++){
                // Leggo ed effettuo la giustificazione del testo (se non sto alla prima colonna setto la distanza giusta)
                read(readPipe[0],&wlimit,sizeof(int));
                if (c == 0){
                    read(readPipe[0],&tot_offset[h],sizeof(int));
                    page[h] = (char **)malloc(columns*(width+dist)*sizeof(char *));
                    for (int i = 0; i <= tot_offset[h]; i++){
                        page[h][i] = (char *)malloc(width * sizeof(char *));
                        read(readPipe[0],page[h][i],width+1);
                    }
                    read(readPipe[0],&next,sizeof(char));
                    add_space(page[h],tot_offset[h],h,c,next);
                } else {
                    int offset = 0;     
                    read(readPipe[0],&offset,sizeof(int));
                    char **line = (char **)malloc(columns*(width+dist)*sizeof(char *));
                    for (int i = 0; i <= offset; i++){
                        line[i] = (char *)malloc(width * sizeof(char *));
                        read(readPipe[0],line[i],width+1);
                    }
                    read(readPipe[0],&next,sizeof(char));
                    add_space(line,offset,h,c,next);
                    set_new_column(line,h,offset);
                }

                // Se sono arrivato al limite della riga o il file è finito autorizzo il processo a scrivere
                if (c == columns - 1){
                    write(printPipe[1],&tot_offset[ph],sizeof(int));
                    for (int i = 0; i <= tot_offset[ph]; i++){
                        write(printPipe[1],page[ph][i],width+1+dist);
                    }
                    if(ph == height - 1){
                        write(printPipe[1],&next,sizeof(char));
                    } else{
                        write(printPipe[1],&value2,sizeof(char));
                    }
                    ph++;
                } if((int)next == -1) {
                    for (int j = ph; j < height; j++){
                        if (page[j] == NULL){
                            break;
                        }
                        write(printPipe[1],&tot_offset[j],sizeof(int));
                        for (int i = 0; i <= tot_offset[j]; i++){
                            write(printPipe[1],page[j][i],width+1+dist);
                        }
                        if((j == h && c == 0) || (j == height-1 && c > 0)){
                            write(printPipe[1],&next,sizeof(char));
                        }
                        else{
                            write(printPipe[1],&value2,sizeof(char));
                        }
                    }
                }
            }
        }

        // Se la pagina è finita e devo crearne una nuova passo il separatore
        if ((int)next != -1) {
            write(printPipe[1],&value,sizeof(int));
            write(printPipe[1],bp,width+1+dist);
            write(printPipe[1],&value2,sizeof(char));
        }
    }

    return;
}

// Funzione per la stampa del testo giustificato sul file output
void print_txt(){
    int offset;             // Contatore delle parole della riga da stampare

    while((int)next != -1){
        // Lettura e processazione dei dati letti dalla pipe
        read(printPipe[0],&offset,sizeof(int));
        char **line = (char **)malloc(columns*(width+dist)*sizeof(char *));
        for (int i = 0; i <= offset; i++){
            line[i] = (char *)malloc(width * sizeof(char *));
            read(printPipe[0],line[i],width+1+dist);
        }

        // Sezione dedicata alla stampa
        for (int j = 0; j <= offset; j++){     
            fprintf(fout,"%s",line[j]);
        }
        free(line);
        
        read(printPipe[0],&next,sizeof(char));
    }
    return;
}

int main()
{
    // Inizializzazione parametri, file e pipe
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt","w");
    pipe(readPipe);
    pipe(printPipe);

    printf("Inserire i parametri e premere invio:\n");

    printf("Righe di ogni colonna: ");
    scanf("%d", &height);

    printf("Caratteri per ogni colonna: ");
    scanf("%d", &width);

    printf("Numero massimo di colonne per pagina: ");
    scanf("%d", &columns);

    printf("Distanza tra ogni colonna: ");
    scanf("%d", &dist);

    // Creo i processi figli e faccio eseguire a ognuno la propia parte

    pid_t pid_justify = fork();
    if(pid_justify == 0){
        pid_t pid_print = fork();
        if (pid_print == 0){
            print_txt();
            close(printPipe[0]);
        } else {
            justify_txt();
            wait(NULL);
            close(printPipe[1]);
            close(readPipe[0]);
        }
    } else {
        read_txt();
        close(readPipe[1]);
        wait(NULL);
    }

    // Chiusura dei file
    fclose(fin);
    fclose(fout);

    return 0;
}
