#include "HeaderMono.h"

// Funzione per la lettura del file input e scrittura del file output
void read_n_print()
{
    //Sezione dedicata alla lettura
    page = (char ***)malloc(height * sizeof(char *));   // Puntatore alla pagina
    offset = (int *)malloc(height*sizeof(int *));   // Array che conta il numero di parole inserito in ogni riga
    for (int c = 0; c < columns; c++) {
        if(feof(fin))
            break;
        for(int h = 0; h < height; h++)
        {
            if(feof(fin))
                break;
            if (c == 0){
                page[h] = (char **)malloc(columns*(width+dist)*sizeof(char *)); // Alloco la memoria per la nuova riga
                offset[h] = 0;  // Setto il contatore di parole per la riga
            } else {
                set_new_column(h);
            }
            new_line(page[h],h,c);
        }
    }


    // Sezione dedicata alla stampa
    for (int h = 0; h < height; h++){
        
        if (page[h] == NULL)
            break;

        for (int j = 0; j <= offset[h]; j++){
            
            if (page[h][j]== NULL)
                break;
        
            fprintf(fout,"%s",page[h][j]);
        }
    }

    free(page);
    return;
}

int main()
{
    // Inizializzazione parametri e file
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt","w");

    printf("Inserire i parametri e premere invio:\n");

    printf("Righe di ogni colonna: ");
    scanf("%d", &height);

    printf("Caratteri per ogni colonna: ");
    scanf("%d", &width);

    printf("Numero massimo di colonne per pagina: ");
    scanf("%d", &columns);

    printf("Distanza tra ogni colonna: ");
    scanf("%d", &dist);

    while(!feof(fin)){
        read_n_print();
        if (!feof(fin)){
            fprintf(fout,"%s","\n\n%%%\n\n");
        }
    }

    // Chiusura dei file
    fclose(fin);
    fclose(fout);

    return 0;
}