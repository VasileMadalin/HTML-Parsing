//Vasile Madalin Constantin 312 CB
#include "lib.h"

//aceasta functie adauga un nou nod in arbore
void adauga(char * comanda, TArb * a, FILE * output) {
    char * id = (char *) malloc(S_LEN*sizeof(char));
    if(!id) printf("Eroare de alocare");
    int k = 0;
    int i, j;
    //se extrage din comanda doar id-ul parintelui 
    //nodului care trebuie introdus
    for (i = 7; comanda[i] != ' '; i++) {
        id[k] = comanda[i];
        k = k + 1;
    }
    char type_contents[CMAX];
    int p = 0;
    int n = strlen(comanda);
    //se extrage din comanda type si contents
    for (j = i + 9 ; j < n; j++) {
        type_contents[p] = comanda[j];
        p = p + 1;
    }
    type_contents[p] = '\0';
    id[k] = '\0';
    char type[S_LEN];
    k = 0;
    //se extrage type din type_contents
    for (i = 2; type_contents[i] != '>' ; i++) {
        type[k] = type_contents[i];
        k = k + 1;
    }
    type[k] = '\0';
    char contents[CMAX];
    k = 0;
    //se extrage contents din type_contents
    for (j = i + 1; type_contents[j] != '<'; j++) {
        contents[k] = type_contents[j];
        k = k + 1;
    }
    contents[k] = '\0';
    char idprim[S_LEN];
    //se face o copie a lui id
    strcpy(idprim, id);
    TArb c = gasire_dupa_id(id, (*a));
    if(c) {//daca nodul cu id respectiv nu are copii atunci
        if(!c->firstChild) {///se adauga primul copil
            c->firstChild = alocare_arbore();
            char copie_id[S_LEN];
            strcpy(copie_id, c->info->id);
            char * id_fiu = id_pentru_fiu(copie_id);
            strcpy(c->firstChild->info->id, id_fiu);
            free(id_fiu);
            strcpy(c->firstChild->info->type, type);
            strcpy(c->firstChild->info->contents, contents);
        }//daca nodul are copii atunci se parcurge lista pana la final
        else {// si se adauga pe ultima pozitie
            TArb aux, penultim;
            aux = c->firstChild;
            while (aux != NULL) {
                penultim = aux;
                aux = aux->nextSibling;
            }
            TArb t;
            t = alocare_arbore();
            char copie_id[S_LEN];
            strcpy(copie_id, penultim->info->id);
            char * id_frate = id_pentru_frate(copie_id);
            strcpy(t->info->id, id_frate);
            free(id_frate);
            strcpy(t->info->type, type);
            strcpy(t->info->contents, contents);
            penultim->nextSibling = t;
        }
    }//in caz ca nu se gaseste nodul se afiseaza urmatorul mesaj
    else fprintf(output, "Add tag failed: node with id %s not found!\n",
        idprim);
    free(id);
}