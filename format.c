//Vasile Madalin Constantin 312 CB
#include "lib.h"

//aceasta functie verifica daca campul contents contine spatii,
//taburi sau caracterul newline la inceput sau la sfarsit
int check (char * contents) {
    int i, n;
    n = strlen(contents);
    int k, j;
    k = 0;
    char new_contents[CMAX];
    //se adauga in noul sir doar caracterele diferite de tab sau newline
    for (i = 0; i < n; i++) {
        if (contents[i] != '\t' && contents[i] != '\n') {
            new_contents[k] = contents[i];
            k = k + 1;
        }
    }
    new_contents[k] = '\0';
    j = 0;
    //se elimina spatiile de la inceput
    while (new_contents[j] == ' ') {
        j = j + 1;
    }
    //se elimina spatiile de la sfarsit
    while(new_contents[k-1] == ' ') {
        k = k - 1;
    }
    for (i = j; i <= k; i++) contents[i - j] = new_contents[i];
    contents[k-j] = '\0';
    if (k > 0) return 1;
    else return 0;
}
//aceasta functie afiseaza un arbore
void afisare(TArb a, int d, FILE * output) {
    int i;
    TArb aux;
    char c = '/';
    char f = '"';
    aux = a;
    //se parcurge fiecare nivel al arborelul pentru fiecare arbore de un
    while (aux != NULL) {//nivel se apeleaza functia pentru "fiul" acestuia
        for (i = 1; i <= d; i++) fprintf(output, "\t");
        fprintf(output, "<%s", aux->info->type);
        if(aux->info->style) {
            TAttr aux1;//se afiseaza style-ul
            aux1 = aux->info->style;
            fprintf(output, " style=");
            fprintf(output, "%c", f);
            while (aux1 != NULL) {
                if(strcmp(aux1->name," ")!=0) {
                    fprintf(output, "%s:", aux1->name);
                    fprintf(output, " ");
                    fprintf(output, "%s;", aux1->value);
                    if(aux1->next) fprintf(output, " ");
                }
                aux1 = aux1->next;
            }
            fprintf(output, "%c", f);//se afiseaza atributele
            if (aux->info->otherAttributes) {
                fprintf(output, " ");
                TAttr aux1 = aux->info->otherAttributes ;
                while (aux1 != NULL) {
                    fprintf(output, "%s=", aux1->name);
                    fprintf(output, "%c%s%c",f,  aux1->value, f);
                    if(aux1->next) fprintf(output, " ");
                    aux1 = aux1->next;
                }
            }
            if(aux->info->isSelfClosing) fprintf(output, "%c>\n", c);
            else fprintf(output, ">\n");
        }
        else {
            if(aux->info->otherAttributes) {
                fprintf(output, " ");
                TAttr aux1 = aux->info->otherAttributes ;
                while (aux1 != NULL) {
                    fprintf(output, "%s=", aux1->name );
                    fprintf(output, "%c%s%c", f, aux1->value, f);
                    if(aux1->next) fprintf(output, " ");
                    aux1 = aux1->next;
                }
            }
            if(aux->info->isSelfClosing) fprintf(output, "%c>\n", c);
            else fprintf(output, ">\n");
        }//se afiseaza continututl fiecarui tag
        if(strcmp(aux->info->contents, "null")) {
            if (check(aux->info->contents) == 1) {
                for (i = 1; i <= d + 1; i++) fprintf(output, "\t");
                        fprintf(output, "%s\n",aux->info->contents);
            }
        }
        afisare(aux->firstChild, d+1, output);
        if(!aux->info->isSelfClosing) { 
            for (i = 1; i <= d; i++) fprintf(output, "\t");
            fprintf(output, "<%c%s>\n", c, aux->info->type);
        }
        aux = aux->nextSibling;
    }
}
