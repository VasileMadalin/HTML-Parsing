//Vasile Madalin Constantin 312 CB
#include "lib.h"

//aceasta functie este apelata cand nu se gasesc noduri
void afisare_eroare_del(FILE *output, char oare[CMAX]) {
    fprintf(output,          //care sa respecte cerintele impuse de selector
        "Delete recursively failed: no node found for selector %s!\n", oare);
}
//aceasta functie modifica id-ul nodurilor dintr-un arbore,
void modifica_id(TArb a) {// dupa ce a fost eliminat un nod
    TArb aux, ultim;
    aux = a->firstChild;
    while (aux != NULL) {//fiecare copil al unui nod isi schimba id-ul in
         if (aux == a->firstChild) {//functie de id-ul parintelui sau
            char id_copie[S_LEN];                          //al fratelui
            strcpy(id_copie, a->info->id);
            char *id_fiu = id_pentru_fiu(id_copie);
            strcpy(aux->info->id, id_fiu);
            free(id_fiu);
        }
        else {
            char id_copie[S_LEN];
            strcpy(id_copie, ultim->info->id);
            char * id_frate = id_pentru_frate(id_copie);
            strcpy(aux->info->id, id_frate);
            free(id_frate);
        }
        modifica_id(aux);
        ultim = aux;
        aux = aux->nextSibling;
    }
}
//aceasta functie elimina un nod cu toti fii lui din arbore
void distruge_nod(TArb * parinte, TArb * fiu) {
    TArb aux = (*parinte)->firstChild;
    TArb ultim = NULL;//se gaseste noul in fii
    while (aux != *fiu && aux != NULL) {//parintelui introdus ca parametru
        ultim = aux;
        aux = aux->nextSibling;
    }
    if(aux) {//se elimina nodul din lista de fii
        if (ultim == NULL) {
            (*parinte)->firstChild = aux->nextSibling;
            aux->nextSibling = NULL;
            eliberare(&aux);//se elibereaza memoria ocupata de acesta si
        }                   //fii sai, folosind functia de eliberare
        else {
            ultim->nextSibling = aux->nextSibling;
            aux->nextSibling = NULL;
            eliberare(&aux);
        }
        modifica_id(*parinte);
    }
}
void del_selector_2(TArb * a, char s[CMAX], int * verif) {
    TArb fiu;
    if(*a) {//aceasta functie sterge din arbore nodurile corespunzatoare
        fiu = (*a)->firstChild;//selectorului .className
        while(fiu) {
            TAttr aux;
            aux = fiu->info->otherAttributes;
            int ok = 1;
            while (aux != NULL && ok == 1) {
                if(strcmp(aux->value,s) == 0) ok = 0;
                aux = aux->next;
            }//se apeleaza recursiv aceasta functie si pentru fiecare
            if (ok == 0) {//nod gasit care respecta cerintele se apeleaza
                *verif = 1;// functia distruge_nod
                TArb urm = fiu->nextSibling;
                distruge_nod(a, &fiu);
                fiu = urm;
                del_selector_2(&fiu, s, verif);
            }
            else {
                del_selector_2(&fiu, s, verif);
                fiu = fiu->nextSibling;
            }
        }
    }
}
void del_selector_3(TArb * a, char s[CMAX], int * verif) {
    TArb fiu;
    if(*a) {//aceasta functie sterge din arbore nodurile corespunzatoare
        fiu = (*a)->firstChild;//selectorului p
        while(fiu) {
            if(strcmp(fiu->info->type, s) == 0) {
                *verif = 1;
                TArb urm = fiu->nextSibling;
                distruge_nod(a, &fiu);
                fiu = urm;
                del_selector_3(&fiu, s, verif);
            }
            else {
                del_selector_3(&fiu, s, verif);
                fiu = fiu->nextSibling;
            }
        }
    }
}
void del_selector_4(TArb * a, char tag[CMAX], char className[CMAX],
    int * verif) {
    TArb fiu;
    if(*a) {
        fiu = (*a)->firstChild;
        while(fiu) {//aceasta functie sterge din arbore nodurile corespunza-
            TAttr aux;//toare selectorului p.className
            aux = fiu->info->otherAttributes;
            int ok = 1;
            while (aux != NULL && ok == 1) {
                if (strcmp(aux->value, className) == 0) ok = 0;
                aux = aux->next;
            }
            if (ok == 0 && strcmp(fiu->info->type, tag) == 0) {
                *verif = 1;
                TArb urm = fiu->nextSibling;
                distruge_nod(a, &fiu);
                fiu = urm;
                del_selector_4(&fiu, tag, className, verif);
            }
            else {
                del_selector_4(&fiu, tag, className, verif);
                fiu = fiu->nextSibling;
            }
        }
    }
}
void del_selector_5(TArb * a, char parinte[CMAX], char fiuprim[CMAX],
    int * verif) {
    TArb fiu;
    if(*a) {//aceasta functie sterge din arbore nodurile corespunzatoare
        fiu = (*a)->firstChild;//selectorului div>p
        while(fiu) {
            if( strcmp((*a)->info->type, parinte)==0&&
                strcmp(fiu->info->type, fiuprim)==0) { 
                *verif = 1;
                TArb urm = fiu->nextSibling;
                distruge_nod(a, &fiu);
                fiu = urm;
                del_selector_5(&fiu, parinte, fiuprim, verif);
            }
            else {
                del_selector_5(&fiu, parinte, fiuprim, verif);
                fiu = fiu->nextSibling;
            }
        }
    }
}
void del_selector_6(TArb * a, char parinte[CMAX], char fiuprim[CMAX],
    int * verif) {
    TArb fiu;
    if(*a) {//aceasta functie sterge din arbore nodurile corespunzatoare
        fiu = (*a)->firstChild;//selectorului div p
        while(fiu) {
            if(strcmp(fiu->info->type, parinte)==0) { 
                TArb urm = fiu->nextSibling;
                int check = 0;
                del_selector_3(&fiu, fiuprim, &check);
                if(check == 1) * verif = 1;
                fiu = urm;
            }
            else {
                del_selector_6(&fiu, parinte, fiuprim, verif);
                fiu = fiu->nextSibling;
            }
        }
    }
}
//aceasta functie sterge anumite noduri dupa un selector introdus in comanda
void sterge_dupa_selector(TArb * a, char * comanda, FILE * output) {
    int k, n, i, j;
    char selector[CMAX],className[CMAX],tag[CMAX],parinte[CMAX],fiuprim[CMAX];
    k = 0;
    n = strlen(comanda);
    for (i = 28; i < n-1; i++) {
        selector[k] = comanda[i];
        k = k + 1;
    }
    selector[k] = '\0';
    int ok = 3;//in functie de anumite caractere din selector ok ia diferite 
    if (selector[0] == '.') {//valori, care indica ce selector se foloseste
        ok = 2;
    }//pentru fiecare selector se aplica una dintre functiile de mai sus
    else {
        if (selector[0] == '#') ok = 1;
        else {
            for (i = 0; i < k; i++) {
                    if (selector[i] == '.') ok = 4;
                    if (selector[i] == ' ') ok = 6;
                    if (selector[i] == '>') ok = 5;
            }
        }
    }   
    if (ok == 2) {
        for (i = 1; i <= k; i++) className[i-1] = selector[i];
        int verif = 0;
        del_selector_2(a, className, &verif);
        if(verif == 0) afisare_eroare_del(output, selector);
    }
    if (ok == 4) {
        int p = 0;
        for (i = 0; selector[i] != '.'; i++) {
            tag[p] = selector[i];
            p = p + 1;
        }
        tag[p] = '\0'; 
        for (j = i + 1; j <= k; j++) {
            className[j - i - 1] = selector[j];
        }
        int verif = 0;
        del_selector_4(a, tag, className, &verif);
        if(verif == 0) afisare_eroare_del(output, selector);
    }
    if (ok == 3) {
        int verif = 0;
        del_selector_3(a, selector, &verif);
        if(verif == 0) afisare_eroare_del(output, selector);
    }
    if (ok == 5) {
        int p = 0;
        for (i = 0; selector[i] != '>'; i++) {
            parinte[p] = selector[i];
            p = p + 1;
        }
        parinte[p] = '\0';
        for (j = i + 1; j <= k; j++) {
            fiuprim[j - i - 1] = selector[j];
        }
        int verif = 0;
        del_selector_5(a, parinte, fiuprim, &verif);
        if(verif == 0) afisare_eroare_del(output, selector);
    }
    if (ok == 6) {
        int p = 0;
        for (i = 0; selector[i]!=' '; i++) {
            parinte[p] = selector[i];
            p = p + 1;
        }
        parinte[p]='\0';
        for (j = i + 1;j <= k; j++) {
            fiuprim[j-i-1] = selector[j];
        }
        int verif = 0;
        del_selector_6(a, parinte, fiuprim, &verif);
        if(verif == 0) afisare_eroare_del(output, selector);
    }
    if (ok == 1) {//in acest caz se distruge doar nod-ul cu un anumit id
        char * id1 = malloc(CMAX * sizeof(char));//specificat de selector
        if(!id1) printf("Eroare de alocare");
        for (i = 1; i <= k; i++) id1[i - 1] = selector[i];
        int verif = 0;
        char * id2 = malloc(CMAX*sizeof(char));
        if(!id2) printf("Eroare de alocare");
        strcpy(id2, id1);
        TArb fiu = gasire_dupa_id(id1 , *a );
        if(fiu) {
            id2[k-3] = '\0';
            TArb parinte = gasire_dupa_id(id2 , *a);
            distruge_nod(&parinte, &fiu);
        }
        else if(verif==0) afisare_eroare_del(output, selector);
        free(id1);
        free(id2);
    }
}
