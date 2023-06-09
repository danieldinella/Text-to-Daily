#include "Header.h"

// Funzione per la lettura del file input
void read_txt()
{
    while(!feof(fin)) {
        int offset = 0;         // Setto il contatore delle parole a 0
        char **line = (char **)malloc(columns*(width+dist)*sizeof(char *));     // Alloco la memoria per la nuova riga
        new_line(line, &offset);
        long int finCount = ftell(fin);     // Ottengo la distanzza dall'inizio del file per passarla al processo justify

        // Scrivo nella pipe del processo che giustifica i dati di cui ha bisogno
        write(readPipe[1],&finCount,sizeof(long int));
        write(readPipe[1],&wlimit,sizeof(int));
        write(readPipe[1],&offset,sizeof(int));
        for (int i = 0; i <= offset; i++){
            write(readPipe[1],line[i],width+1);
        }

        free(line);
    }
    return;
}

// Funzione per la giustificazione del testo
void justify_txt(){
    char* bp = "\n\n%%%\n\n";   // Separatore di fine pagina
    int offset;                 // Contatore delle parole della nuova riga (viene usato quando si va oltre la prima colonna)
    long int finCount;          // Variabile che prende dalla pipe l'ftell(fin) del processo read
    const long int value = 0;   // Valore d'appoggio da passare al posto di fincount
    tot_offset = (int *)malloc(height * sizeof(int));   // Array di contatori di parole delle linee
    page = (char ***)malloc(height * sizeof(char *));   // Puntatore alla pagina

    while(!feof(fin)){
        int ph = 0;             // Contatore delle righe che si possono già stampare

        for(int c = 0; c < columns; c++){
            if (feof(fin)){
                break;
            }
            for(int h = 0; h < height && !feof(fin); h++){
                // Se la lettura del file è terminata lo comunico al processo che si occupa di stampare
                read(readPipe[0],&finCount,sizeof(long int));
                fseek(fin,finCount,SEEK_SET);

                // Leggo ed effettuo la giustificazione del testo (se non sto alla prima colonna setto la distanza giusta)
                read(readPipe[0],&wlimit,sizeof(int));
                if (c == 0){
                    read(readPipe[0],&tot_offset[h],sizeof(int));
                    page[h] = (char **)malloc(columns*(width+dist)*sizeof(char *));
                    for (int i = 0; i <= tot_offset[h]; i++){
                        page[h][i] = (char *)malloc(width * sizeof(char *));
                        read(readPipe[0],page[h][i],width+1);
                    }
                    add_space(page[h],tot_offset[h],h,c);
                } else {
                    int offset = 0;     
                    read(readPipe[0],&offset,sizeof(int));
                    char **line = (char **)malloc(columns*(width+dist)*sizeof(char *));
                    for (int i = 0; i <= offset; i++){
                        line[i] = (char *)malloc(width * sizeof(char *));
                        read(readPipe[0],line[i],width+1);
                    }
                    add_space(line,offset,h,c);
                    set_new_column(line,h,offset);
                }
                
                // Se sono arrivato al limite della riga o il file è finito autorizzo il processo a scrivere
                if (c == columns - 1){
                    write(printPipe[1],&finCount,sizeof(long int));
                    write(printPipe[1],&tot_offset[ph],sizeof(int));
                    for (int i = 0; i <= tot_offset[ph]; i++){
                        write(printPipe[1],page[ph][i],width+1+dist);
                    }
                    ph++;
                } else if(feof(fin)) {
                    for (int j = ph; j < height; j++){
                        if (page[j] == NULL){
                            break;
                        }
                        if ((j < h && c == 0) || (j < height-1 && c > 0)){
                            write(printPipe[1],&value,sizeof(long int));
                        }else{
                            write(printPipe[1],&finCount,sizeof(long int));
                        }
                        write(printPipe[1],&tot_offset[j],sizeof(int));
                        for (int i = 0; i <= tot_offset[j]; i++){
                            write(printPipe[1],page[j][i],width+1+dist);
                        }
                    }
                }
                if (feof(fin)){
                    break;
                }
            }
        }

        // Se la pagina è finita e devo crearne una nuova passo il separatore
        if (feof(fin)){
            break;
        } else {
            write(printPipe[1],&finCount,sizeof(long int));
            write(printPipe[1],&value,sizeof(int));
            write(printPipe[1],bp,width+1+dist);
        }
    }
    return;
}

// Funzione per la stampa del testo giustificato sul file output
void print_txt(){
    long int finCount;      // Variabile che prende dalla pipe l'ftell(fin) del processo justify
    int offset;             // Contatore delle parole della riga da stampare
    char x;                 // Variabile d'appoggio per far scorrere il file e verificare l'feof

    while(!feof(fin)){
        // Lettura e processazione dei dati letti dalla pipe
        read(printPipe[0],&finCount,sizeof(long int));
        fseek(fin,finCount,SEEK_SET);
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
        x = fgetc(fin);
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
            printf("Processo print concluso\n");
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