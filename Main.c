#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int main(){

    //Dichiarazione e inizializzazione parametri e file
    int dist, width, lines, columns, line_l;
    FILE *fin, *fout;

    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    printf("Inserire i parametri e premere invio:\n");

    printf("Righe di ogni colonna:\n");
    scanf("%d", lines);

    printf("Caratteri per ogni colonna:\n");
    scanf("%d", width);

    printf("Numero massimo di colonne per pagina:\n");
    scanf("%d", columns);

    printf("Distanza tra ogni colonna:\n");
    scanf("%d", dist);


    //Lettura del file dato in input
    char line[width];
    bool endp = false;

    while (!feof(fin)) {
        
        //Se la riga prima era l'ultima di un paragrafo salta una riga altrimenti procedi con la lettura
        if (endp) {
            fputs("\n",fout);
            endp = false;
        }
        else {
            fgets(line,width,fin);
            line_l=strlen(line);
            if (line[line_l-1]=='\n'){
                if (line[line_l-2]=='\n'){
                    endp = true;
                }
                line[line_l-1]='\0';
            }
            if (line[line_l-1]!='\n')
                line[line_l-1]='\n';

            fputs(line,fout);
        }
    }

}