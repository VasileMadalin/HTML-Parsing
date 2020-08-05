//Vasile Madalin Constantin 312 CB
#include "lib.h"

//aceasta functie este apelata cand nu se gasesc noduri
void afisare_eroare(int sw, FILE * output, char oare[CMAX]) {
	if(sw==1) fprintf(output, //care sa respecte cerintele impuse de selector
        "Override style failed: no node found for selector %s!\n", oare);
	else fprintf(output, 
        "Append to style failed: no node found for selector %s!\n", oare);
}
//aceasta functie suprascrie style-ul unei nod daca parametrul
//sw este 1 sau actualizeaza style-ul daca sw este 0
void override_and_append_selector_2(TArb * a, char s[CMAX], char style[CMAX],
 int * verif, int sw) {
    TArb fiu;//aceasta functie sterge din arbore nodurile corespunzatoare
    if(*a) {//selectorului .className
        fiu = (*a)->firstChild;
        while(fiu) {
            TAttr aux;
            aux = fiu->info->otherAttributes;
            int ok = 1;//se cauta nodurile care respecta cerinta
            while (aux != NULL && ok == 1) {//impusa de selector
                if (strcmp(aux->value, s) == 0) ok = 0;
                aux = aux->next;
            }
            if (ok == 0) {
                *verif = 1;
                char * newstyle = malloc(CMAX * sizeof(char));
                if(!newstyle) printf("Eroare de alocare");
                strcpy(newstyle, style);//daca sw este egal cu 1 se distruge
                if (sw == 1) {//lista //pentru style si se adauga nou style
                	DistrugeL(&fiu->info->style);
                	gaseste_nume_si_valoare(&fiu, newstyle, sw);
                }
                if (sw == 0) {//daca se este egal cu 0 doar se adauga
                	gaseste_nume_si_valoare(&fiu, newstyle, sw);
                }
                free(newstyle);
                override_and_append_selector_2(&fiu, s, style, verif, sw);
                fiu = fiu->nextSibling;
            }
            else {
                override_and_append_selector_2(&fiu, s, style, verif, sw);
                fiu = fiu->nextSibling;
            }
        }
    }
}
void override_and_append_selector_3(TArb * a, char tag[CMAX], 
    char style[CMAX], int * verif, int sw) {
    TArb fiu;
    if(*a) {//aceasta functie sterge din arbore nodurile corespunzatoare
        fiu = (*a)->firstChild;////selectorului p
        while (fiu) {
            if (strcmp(fiu->info->type, tag)==0) {
                *verif = 1;
                char * newstyle = malloc(CMAX * sizeof(char));
                if (!newstyle) printf("Eroare de alocare");
                strcpy(newstyle, style);
                if (sw == 1) {
                	DistrugeL(&fiu->info->style);
                	gaseste_nume_si_valoare(&fiu, newstyle, sw);
                }
                if (sw == 0) {
                	gaseste_nume_si_valoare(&fiu, newstyle, sw);
                }
                free(newstyle);
                override_and_append_selector_3(&fiu, tag, style, verif, sw);
                fiu = fiu->nextSibling;
            }
            else {
                override_and_append_selector_3(&fiu, tag, style, verif, sw);
                fiu = fiu->nextSibling;
            }
        }
    }
}
void override_and_append_selector_4(TArb * a, char tag[CMAX], char s[CMAX], 
    char style[CMAX], int * verif, int sw) {
    TArb fiu;
    if(*a) {//aceasta functie sterge din arbore nodurile corespunzatoare
        fiu = (*a)->firstChild;//selectorului p.className
        while(fiu) {
            TAttr aux;
            aux = fiu->info->otherAttributes;
            int ok = 1;
            while (aux != NULL && ok == 1) {
                if(strcmp(aux->value,s) == 0) ok = 0;
                aux = aux->next;
            }
            if (ok == 0 && strcmp(fiu->info->type, tag) == 0) {
                *verif = 1;
                char * newstyle = malloc(CMAX * sizeof(char));
                if (!newstyle) printf("Eroare de alocare");
                strcpy(newstyle, style);
                if (sw == 1) {
                	DistrugeL(&fiu->info->style);
                    gaseste_nume_si_valoare(&fiu, newstyle, sw);
                }
                if(sw==0) {
                    gaseste_nume_si_valoare(&fiu, newstyle, sw);
                }
                free(newstyle);
                override_and_append_selector_4(&fiu, tag, s, style,verif, sw);
                fiu = fiu->nextSibling;
            }
            else {
                override_and_append_selector_4(&fiu, tag, s, style,verif, sw);
                fiu = fiu->nextSibling;
            }
        }
    }
}
void override_and_append_selector_5(TArb * a, char parinte[CMAX], 
    char fiuprim[CMAX], char style[CMAX], int * verif, int sw) {
    TArb fiu;
    if(*a) {//aceasta functie sterge din arbore nodurile corespunzatoare
        fiu = (*a)->firstChild;//selectorului div>p
        while(fiu) {
            if(strcmp((*a)->info->type,parinte) == 0 && 
                strcmp(fiu->info->type, fiuprim) == 0) {
                *verif = 1;
                char * newstyle = malloc(CMAX * sizeof(char));
                if(!newstyle) printf("Eroare de alocare");
                strcpy(newstyle, style);
                if(sw==1) {
                    DistrugeL(&fiu->info->style);
                    gaseste_nume_si_valoare(&fiu, newstyle, sw);
                }
                if(sw==0) {
                	gaseste_nume_si_valoare(&fiu, newstyle, sw);
                }
                free(newstyle);
                override_and_append_selector_5(&fiu, parinte, fiuprim,
                    style, verif, sw);
                fiu = fiu->nextSibling;
            }
            else {
                override_and_append_selector_5(&fiu, parinte, fiuprim, 
                    style, verif, sw);
                fiu = fiu->nextSibling;
            }
        }
    }
}
void override_and_append_selector_6(TArb * a, char parinte[CMAX], 
    char fiuprim[CMAX], char style[CMAX], int * verif, int sw) {
    TArb fiu;
    if(*a) {//aceasta functie sterge din arbore nodurile corespunzatoare
        fiu = (*a)->firstChild;//selectorului div p
        while (fiu) {
            if (strcmp(fiu->info->type, parinte) == 0) {
            	int check = 0;
                override_and_append_selector_3(&fiu, fiuprim, style, 
                    &check, sw);
                if(check == 1) * verif = 1;
                fiu = fiu->nextSibling;
            }
            else {
            override_and_append_selector_6(&fiu, parinte, fiuprim, style, verif, sw);
            fiu = fiu->nextSibling;
            }
        }
    }
}
//aceasta functie suprascrie sau actalizeaza style-ul anumitor noduri
//dupa un selector introdus in comanda
void override_and_append_dupa_selector(TArb * a, char * comanda, 
	FILE * output, int sw) {
    int k, p, i, j;
    char oare[CMAX], style[CMAX], className[CMAX], tag[CMAX];
    char parinte[CMAX], fiuprim[100];
    k = 0; p = 0;
    if(sw==1) {
    	for (i = 24; comanda[i] !='"'; i++) {
        	oare[k] = comanda[i]; k = k + 1;
    	}
	}
	if(sw==0) {
		for (i = 22; comanda[i] !='"'; i++) { oare[k]=comanda[i]; k=k+1; }
	}
    oare[k] = '\0';
    for (j=i+9;comanda[j] != '"'; j++) {
		style[p] = comanda[j]; p = p + 1;    	
    }
    style[p] = '\0'; int ok = 3;
    if(oare[0]=='.') ok = 2;
    else {//in functie de anumite caractere din selector ok ia diferite 
        if(oare[0]=='#') ok = 1;
        else {             //valori, care indica ce selector se foloseste
            for(i=0;i<k;i++) {
                    if(oare[i]=='.') ok = 4; 
                    if(oare[i]==' ') ok = 6; 
                    if(oare[i]=='>') ok = 5;
            }
        }
    }//pentru fiecare selector se aplica una dintre functiile de mai sus
    if (ok == 2) {
        for(i = 1; i <= k; i++) className[i-1]=oare[i];
            int verif = 0;
        	override_and_append_selector_2(a, className, style, &verif, sw);
        	if (verif == 0) afisare_eroare(sw, output, oare);
    }
    if (ok == 4) {
                int p = 0;
                for(i = 0; oare[i] != '.'; i++) {
                    tag[p] = oare[i]; p = p + 1;
                }
                tag[p]='\0'; 
                for(j = i + 1; j <= k; j++) {
                    className[j - i - 1] = oare[j];
                }
                int verif = 0;
                override_and_append_selector_4(a, tag, className, 
                	style, &verif, sw);
                if (verif == 0) afisare_eroare(sw, output, oare);
    }
    if (ok == 3) {
        int verif = 0;
        override_and_append_selector_3(a, oare, style, &verif, sw);
        if(verif==0) afisare_eroare(sw, output, oare);
    }
    if (ok == 5) {
                int p = 0;
                for(i = 0; oare[i] != '>'; i++) {
                    parinte[p] = oare[i]; p = p + 1;
                }
                parinte[p]='\0';
                for(j = i + 1; j <= k; j++) {
                    fiuprim[j - i - 1] = oare[j];
                }
                int verif = 0;
                override_and_append_selector_5(a, parinte, fiuprim, style, 
                	&verif, sw);
                if (verif == 0) afisare_eroare(sw, output, oare);
    }
    if (ok == 6) {
                int p = 0;
                for(i = 0; oare[i] != ' '; i++) {
                    parinte[p] = oare[i]; p = p + 1;
                }
                parinte[p] = '\0';
                for (j = i + 1; j <= k; j++) {
                    fiuprim[j - i - 1] = oare[j];
                }
                int verif = 0;
                override_and_append_selector_6(a, parinte, fiuprim, style, 
                	&verif, sw);
                if (verif == 0) afisare_eroare(sw, output, oare);
    }
    if (ok == 1) {
        char * id1 = malloc(CMAX*sizeof(char));
        if(!id1) printf("Eroare de alocare");
        for(i = 1; i <= k; i++) id1[i - 1] = oare[i];
        TArb fiu = gasire_dupa_id(id1 , *a );
        if (fiu) {
        	if(sw == 1) {
        	DistrugeL(&fiu->info->style);
            gaseste_nume_si_valoare(&fiu, style, sw); 
        }
            if (sw == 0) gaseste_nume_si_valoare(&fiu, style, sw);
        }
        else afisare_eroare(sw, output, oare);
        free(id1);
    }
}
