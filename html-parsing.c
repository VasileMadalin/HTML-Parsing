//Vasile Madalin Constantin 312 CB
#include "lib.h"

//aceasta functie gaseste parintele unui nod introdus ca parametru
void gaseste_parinte(TArb a, TArb y, TArb *w) {
    TArb aux;
    if(a) {
        aux = a->firstChild;
        if(aux) {//se parcurge lista cu "fractii" nodului curent
            while (aux != NULL) {
                if (aux == y) *w = a;//se apeleaza functia pentru fiul
                else gaseste_parinte(aux, y, w);//nodului analizat
                aux = aux->nextSibling;
            }
        }
    }
}
//aceasta functie distruge o lista TAttr
void DistrugeL(TAttr* aL) {
    TAttr aux;
    while (*aL) {
        aux = *aL;
        *aL = aux->next;
        free(aux->name);
        free(aux->value);
        free(aux);
    }
}
//aceasta functie elimina memoria pentru un arbore primit ca parametru
void eliberare(TArb * a) {
    TArb aux, urm;
    if(*a) {//se parcurge fiecare "nivel" al arborelui
        while(*a) {//pentru fiecare nivel se distrug listele
            aux = * a;//corespunzatoare fiecarui "fiu"
            urm = (*a)->firstChild;
            *a = aux->nextSibling;
            free(aux->info->id);
            free(aux->info->type);
            if (aux->info->style) {//se apeleaza functia care distruge lista
                DistrugeL(&aux->info->style);//pentru "style"
                if(aux->info->otherAttributes) {//se apeleaza functia care
                    DistrugeL(&aux->info->otherAttributes);//distruge lista
                }                               //pentru "otherAttributes"
            }
            else {//se apeleaza functia care distruge lista pentru
                if(aux->info->otherAttributes) {//"otherAttributes"
                    DistrugeL(&aux->info->otherAttributes);
                }
            }//se elibereaza memoria pentru string-ul aux->info->contents
            free(aux->info->contents);
            free(aux->info);
            free(aux);
            //se apeleaza functia pentru fiul nod-ului care a fost distrus
            eliberare(&urm);
        }
    }
}
//aceasta functie aloca memorie pentru un arbore
TArb alocare_arbore() {
    TArb a;
    //se aloca memorie pentru arbore
    a = (TArb ) malloc(sizeof(TNodArb));
    if(!a) printf("Eroare de alocare");
    //se aloca memorie pentru componentele arborelui
    a->info = (TInfo) malloc(sizeof(TNodInfo));
    if(!a->info) printf("Eroare de alocare");
    a->info->type = (char *) malloc(CMAX*sizeof(char));
    if(!a->info->type) printf("Eroare de alocare");
    strcpy(a->info->type, "null");
    a->info->id = (char *) malloc(CMAX*sizeof(char));
    if(!a->info->id) printf("Eroare de alocare");
    strcpy(a->info->id, "null");
    //se seteaza "style","otherAttributes","nextSibling","firstChild" pe NULL
    a->info->style = NULL;
    a->info->otherAttributes = NULL;
    a->nextSibling = NULL;
    a->firstChild = NULL;
    a->info->isSelfClosing = 0;
    a->info->contents = (char *) malloc(CMAX*sizeof(char));
    if(!a->info->contents) printf("Eroare de alocare");
    strcpy(a->info->contents, "null");
    return a;
}
//aceasta functie se foloseste pentru a analiza fiserul html
TParseState Interpret(TParseState currentState, char c, TArb *a, TArb d)
{
    TArb w = NULL;
    TParseState nextState;
    switch (currentState)
    {
    case PARSE_CONTENTS:
        if (c == '<')           {   nextState = PARSE_OPENING_BRACKET;  }
        else                    {   nextState = PARSE_CONTENTS;
                                    //se adauga textul pentru "contents"
                                    int length;
                                    if(strcmp ((*a)->info->contents, 
                                        "null") == 0 ) {
                                        length = 0;
                                    }
                                    else {  
                                        length = strlen((*a)->info->contents);
                                    }
                                    (*a)->info->contents[ length ] = c;
                                    (*a)->info->contents[ length+1 ] = '\0';
                                                                        }
        break;
    case PARSE_OPENING_BRACKET:
        if (isspace(c))         {   nextState = PARSE_OPENING_BRACKET;  }
        else if (c == '>')      {   nextState = PARSE_ERROR;            }
        else if (c == '/')      {   nextState = PARSE_CLOSING_TAG;      }
        else                    {   nextState = PARSE_TAG_TYPE;         }
        break;
    case PARSE_TAG_TYPE:
        if (isspace(c))         {   nextState = PARSE_REST_OF_TAG;      }
        else if (c == '>')      {   nextState = PARSE_CONTENTS;         }
        else                    {   nextState = PARSE_TAG_TYPE;         }
        break;
    case PARSE_CLOSING_TAG:
        if (c == '>')           {   nextState = PARSE_CONTENTS;
                                //la intalnirea acestui caracter nodul
                                //curent este inlocuit de parintele acestuia
                                    gaseste_parinte(d, *a, &w);
                                    if(w) (*a) = w;                     }
        else                    {   nextState = PARSE_CLOSING_TAG;      }
        break;
    case PARSE_REST_OF_TAG:
        if (isspace(c))         {   nextState = PARSE_REST_OF_TAG;      }
        else if (c == '>')      {   nextState = PARSE_CONTENTS;         }
        else if (c == '/')      {   nextState = PARSE_SELF_CLOSING;     }
        else                    {   nextState = PARSE_ATTRIBUTE_NAME;   }
        break;
    case PARSE_ATTRIBUTE_NAME:
        if (c == '=')           {   nextState = PARSE_ATTRIBUTE_EQ;     }
        else                    {   nextState = PARSE_ATTRIBUTE_NAME;   }
        break;
    case PARSE_ATTRIBUTE_EQ:
        if (c == '\"')          {   nextState = PARSE_ATTRIBUTE_VALUE;  }
        break;
    case PARSE_ATTRIBUTE_VALUE:
        if (c == '\"')          {   nextState = PARSE_REST_OF_TAG;      }
        else                    {   nextState = PARSE_ATTRIBUTE_VALUE;  }
        break;
    case PARSE_SELF_CLOSING:
        if (c == '>')           {   nextState = PARSE_CONTENTS;
                                //acest carcater semnaleaza faptul ca 
                                //tag-ul este unul SelfClosing
                                    (*a)->info->isSelfClosing = 1; 
                                     gaseste_parinte(d, *a, &w);
                                     (*a) = w;                          }
        else                    {   nextState = PARSE_ERROR;            }
        break;
        default:
        break;
    }
    return nextState;
}
//aceasta functie adauga un stil nou nodului c, introdus ca parametru
void adauga_stil_nou (TArb *c, char name[CMAX], char value[CMAX]) {
    TAttr aux = (*c)->info->style;
    //se parcurge lista,daca este vida,se adauga noul stil ca primul element
    if (aux == NULL) {
        (*c)->info->style = malloc(sizeof(TNodAttr));
        if(!(*c)->info->style) printf("Eroare de alocare");
        (*c)->info->style->name = malloc(CMAX*sizeof(char));
        if(!(*c)->info->style->name) printf("Eroare de alocare");
        (*c)->info->style->value = malloc(CMAX*sizeof(char));
        if(!(*c)->info->style->value) printf("Eroare de alocare");
        strcpy((*c)->info->style->name, name);
        strcpy((*c)->info->style->value, value);
        (*c)->info->style->next = NULL;
    }
    else {//daca lista nu este vida, se parcurge pana
          //la final si se adaga pe ultima pozitie noul stil
        while(aux->next!=NULL) {
            aux = aux->next;
        }
        aux->next = malloc(sizeof(TNodAttr));
        if(!aux->next) printf("Eroare de alocare");
        aux->next->name = malloc(CMAX*sizeof(char));
        if(!aux->next->name) printf("Eroare de alocare");
        aux->next->value = malloc(CMAX*sizeof(char));
        if(!aux->next->value) printf("Eroare de alocare");
        strcpy(aux->next->name, name);
        strcpy(aux->next->value, value);
        aux->next->next = NULL;
    }
}
//aceasta functie adauga un nou stil sau il actalizeaza daca exista
void actualizeaza_stil (TArb *c, char name[CMAX], char value[CMAX]) {
    int ok ;
    TAttr aux = (*c)->info->style;
    //daca lista este vida, se aduag noul stil ca primul element
    if (aux == NULL) {
        (*c)->info->style = malloc(sizeof(TNodAttr));
        if(!(*c)->info->style) printf("Eroare de alocare");
        (*c)->info->style->name = malloc(CMAX*sizeof(char));
        if(!(*c)->info->style->name) printf("Eroare de alocare");
        (*c)->info->style->value = malloc(CMAX*sizeof(char));
        if(!(*c)->info->style->value) printf("Eroare de alocare");
        strcpy((*c)->info->style->name, name);
        strcpy((*c)->info->style->value, value);
        (*c)->info->style->next = NULL;
    }
    else {//daca lista nu este vida, se parcurge pana la final, daca se
          //gaseste acelasi stil in lista, se actalizeaza altfel se 
          //adauga stilul pe ultima pozitie
        ok = 0;
        TAttr ultim = NULL;
        while (aux != NULL && ok == 0) {
            if(strcmp(aux->name, name)==0) {
                ok = 1;
                strcpy(aux->value, value);
            }  
            ultim = aux;
            aux = aux->next;
        }
        if (ok == 0) {
            ultim->next = malloc(sizeof(TNodAttr));
            if(!ultim->next) printf("Eroare de alocare");
            ultim->next->name = malloc(CMAX*sizeof(char));
            if(!ultim->next->name) printf("Eroare de alocare");
            ultim->next->value = malloc(CMAX*sizeof(char));
            if(!ultim->next->value) printf("Eroare de alocare");
            strcpy(ultim->next->name, name);
            strcpy(ultim->next->value, value);
            ultim->next->next = NULL;
        }
    }
}
//aceasta functie primeste ca parametru intregul string de stil-uri si
//il imparte dupa nume si valoare
void gaseste_nume_si_valoare(TArb *c, char * atribut, int sw) {
    int i, j;
    char * word;
    char * word1 = malloc(CMAX*sizeof(char));
    if(!word1) printf("Eroare de alocare");
    //folosind separatorul ";" se desparte string-ul 
    //in componenete de forma name:value
    word = strtok (atribut, SEP);
    while (word!=NULL) {
        strcpy(word1, word);
        int k = 0;
        int ok = 1;
        char name[CMAX], value[CMAX];
        int t = 0;
        int n = strlen(word1);
        //fiecare dintre aceste componenete este despartita in name si value
        //prin parcurgerea acesteia si identificarea elemntelor
        //dintre name si value
        while (word1[t] ==' ') {
            t = t + 1;
        } 
        for (i = t; i < n && ok == 1; i++) {
            if (word1[i] == ':' || word1[ i ] == ' ') ok = 0;
            if (ok == 1) {
                name[ k ] = word1[ i ];
                k = k + 1;
            }   
        }
        name[ k ] = '\0';
        int k1 = 0;
        for (j = i; j < n; j++) {
            if (word1[ j ] != ' ' && word1[ j ] != ':') {
                value[ k1 ] = word1[ j ];
                k1 = k1 + 1;
            }
        }
        value[ k1 ] = '\0';
        //in functide comanda override sau append se apeleaza 
        if (k != 0 && k1 != 0) {
            if (sw==1) adauga_stil_nou(c, name, value);
            if (sw==0) actualizeaza_stil(c, name, value);
        }
        word = strtok (NULL, SEP);
    }
    free(word1);
}
//aceasta functie creeaza id-ul pentru "fratele" unui anumit nod
char * id_pentru_frate(char  s[S_LEN]) {
    int n, k, p, i, j, nr;
    char aux[S_LEN], num[S_LEN], numprim[S_LEN]; 
    char *rez = malloc(S_LEN * sizeof(char));
    if(!rez) printf("Eroare de alocare");
    n = strlen(s);
    if(n>1) {
        //se gaseste ultimul numar dupa ultimul punct 
        k = 0;//al id-ului primit ca parametru
        for (i = n - 1; i >= 0 && s[i] != '.'; i--) {
            aux[k] = s[i];
            k = k + 1;
        }
        p = 0 ;
        for (j = 0; j <= i; j++) {
            rez[p] = s[j];
            p = p + 1;
        }
        aux[k] = '\0';
        for (i = 0; i < k; i++) num[i] = aux[k - i - 1];
        num[k] = '\0';
        //se  transforma string-ul in numar
        nr = atoll(num);
        //se transforma in string numarul + 1
        sprintf(numprim, "%d", nr + 1);
        n = strlen(numprim);
        //se adauga la id-primit ca parametru dar numarul dupa ultimul punct
        for (i = 0; i < n; i++) {
            rez[p] = numprim[i];
            p = p + 1;
        }
        rez[p] = '\0';
    }
    else {
    //daca id este format dintr-un singur carcater, id-ul fratelui se 
    //formeaza din acest carcater transformat in intreg la care se adauga 1
        nr = atoll(s);
        sprintf(numprim, "%d", nr + 1);
        strcpy(rez, numprim);
    }
    return rez;
}
//aceasta functie creaza id-ul pentru primul fiu al unui nod
char * id_pentru_fiu(char s[CMAX]) {
    int n;
    char *rez = malloc(CMAX * sizeof(char));
    if(!rez) printf("Eroare de alocare");
    if(strcmp(s, "0") == 0) {//daca nodul primit este "html"
        strcpy(rez, "1");//id-ul fiului este 1
    }
    else {//altfel id-ul primul fiu se formeaza prin adaugarea
        strcpy(rez,s);//string-ului ".1"
        n = strlen(rez);
        rez[n] = '.';
        rez[n+1] = '1';
        rez[n+2] = '\0';
    }
    return rez;
}
//aceasta functie adauga un nod nod in arbore
void adauga_nod(TArb * a, TArb * c, char tag[S_LEN]) {
    TArb d;
    if(!*a) {//daca arborele este vid, adauga primul nod in arbore
        *a = alocare_arbore();
        strcpy((*a)->info->type, tag);
        strcpy((*a)->info->id, "0");
        *c = *a;
    }
    else {
        d = (*c)->firstChild;
        if (!d) {//daca nodul curent nu are fii, atunci se adauga primul fiu,
            (*c)->firstChild = alocare_arbore();// si se seteaza id-ul in
            char id_parinte[S_LEN];//functie de id-ul parintelui
            strcpy(id_parinte, (*c)->info->id);
            char * id_fiu = id_pentru_fiu(id_parinte);
            strcpy((*c)->firstChild->info->id, id_fiu);
            free(id_fiu);
            strcpy((*c)->firstChild->info->type, tag);
            (*c) = (*c)->firstChild;
        }
        else {//daca nodul curent are mai multi fii,
            TArb aux, penultim;//atunci se parcurge lista pana la
            aux = (*c)->firstChild;//final si se adauga nodul pe ultima
            penultim = aux;//pozitie, id-ul se modifica in functie de
                        //id-ul fratelui
            while (aux != NULL) {
                penultim = aux;
                aux = aux->nextSibling;
            }
            TArb t;
            t = alocare_arbore();
            char id_frate[S_LEN];
            strcpy(id_frate, penultim->info->id);
            char * id_frate_urm = id_pentru_frate(id_frate);
            strcpy(t->info->id, id_frate_urm);
            free(id_frate_urm);
            strcpy(t->info->type, tag);
            penultim->nextSibling = t;
            *c = t;
        }
    }
}
//aceasta functie adauga stilul unui nod sau ii adauga campul "value" 
//pentru alte atribute
void adauga_value(char style_check[CMAX], TArb * c, char * value) {
    TAttr aux1;
    if(strcmp(style_check,"style") == 0) {//in acest caz se adauga un nou stil
            gaseste_nume_si_valoare(c, value + 1, 1);
            strcpy(style_check, "null");
    }
    else {//sau adauga campul value pentru alte atribute
        if(strcmp((*c)->info->otherAttributes->value, "null") == 0) {        
            strcpy((*c)->info->otherAttributes->value, value + 1);
        }
        else {
            aux1 = (*c)->info->otherAttributes;
            while (strcmp(aux1->value, "null") != 0) {
                aux1 = aux1->next;
            } 
            strcpy(aux1->value, value + 1);
        }
    }
}
//aceasta functie adauga campul "name" in atributul unui nod
void adauga_atribut(TArb * c, char * name) {
    TAttr aux1;
    if (!(*c)->info->otherAttributes) {//daca nu sunt alte atribute,
        //aceasta functie adauga pentru primul nod
        (*c)->info->otherAttributes = malloc(sizeof(TNodAttr));
        if(!(*c)->info->otherAttributes) printf("Eroare de alocare");
        (*c)->info->otherAttributes->name = malloc(CMAX*sizeof(char));
        if(!(*c)->info->otherAttributes->name) printf("Eroare de alocare");
        (*c)->info->otherAttributes->value = malloc(CMAX*sizeof(char));
        if(!(*c)->info->otherAttributes->value) printf("Eroare de alocare");
        (*c)->info->otherAttributes->next = NULL;
        strcpy((*c)->info->otherAttributes->name, name);
        strcpy((*c)->info->otherAttributes->value, "null");
    }
    else {//daca lista de atribute nu este vida, atunci 
        //se creeaza un nou atribut care se adauga pe 
        //ultima pozitie, si se completeaza campul "name"  
        aux1 = (*c)->info->otherAttributes;
        TAttr ultim;
        while (aux1 != NULL) {
                ultim = aux1;
                aux1 = aux1->next;
        }   
        TAttr t = NULL;
        t = malloc(sizeof(TNodAttr)); 
        if(!t) printf("Eroare de alocare");
        t->name = malloc(CMAX*sizeof(char));
        if(!t->name) printf("Eroare de alocare");
        t->value = malloc(CMAX*sizeof(char));
        if(!t->value) printf("Eroare de alocare");
        strcpy(t->name, name);
        strcpy(t->value, "null"); 
        t->next = NULL;
        ultim->next = t;
    }
}
//aceasta functie citeste elementele arborelului si il creeaza
TArb citire(FILE * html) {
    TArb a = NULL;
    TArb c = NULL;
    char * comanda = (char *)malloc(MAX_CMD_LEN * sizeof(char));
    if(!comanda) printf("Eroare de alocare");
    int n, i;
    int length_tag = 0;
    char tag[CMAX];
    char * name = (char *) malloc(CMAX*sizeof(char));
    if(!name) printf("Eroare de alocare");
    TParseState currentState = PARSE_CONTENTS;
    int length_value = 0;
    int length_name = 0;
    char style_check[CMAX];
    strcpy(style_check,"null");
    char * value = (char *) malloc(CMAX*sizeof(char));
    if(!value) printf("Eroare de alocare");
    strcpy(value, "null");//se citeste pe linii fisierul html, 
    while (fgets(comanda, MAX_CMD_LEN, html) != NULL) {
        n = strlen(comanda);
        for (i=0;i<n;i++) {//folosind rezultatul generat de functia Interpret 
            if(comanda[i] != 13) {//se foremeaza string-urile corespunzatoare
                                 //fiecarui elementul al unui nod
                currentState = Interpret(currentState, comanda[i], &c, a);
                if(currentState == 3) {
                    tag[length_tag] = comanda[i];
                    length_tag = length_tag + 1;
                }
                else {
                    tag[length_tag] = '\0';
                    if (length_tag) {
                                        //cand se gaseste un nod nou,
                        adauga_nod(&a, &c, tag);//se apeleaza functia
                    }                   //pentru adaugarea unui nou nod
                    length_tag = 0;
                }                       
                if (currentState == 8) {
                    value[length_value] = comanda[i];
                    length_value = length_value + 1;
                }
                else {
                    value[length_value] = '\0';
                    if(length_value) {//cand se gaseste "value"
                            //se apeleaza functia care adauga un style sau value
                        adauga_value(style_check, &c, value);//in campul value
                    }                                    //al unui atribut
                    length_value = 0;
                } 
                if (currentState == 6) {
                    name[length_name] = comanda[i];
                    length_name = length_name + 1;
                }
                else {
                    name[length_name] = '\0';
                    if(length_name) {//cand se gaseste "name" se apeleaza
                        if(strcmp(name, "style")==0) {//functia care adauga
                            strcpy(style_check, name);//"name" in campul name
                        }                             //al unui atribut
                        else {
                            adauga_atribut(&c, name);
                        }
                    }
                    length_name = 0;
                }
            }
        }
    }
    free(comanda);
    free(name);
    free(value);
    return a;
}
//aceasta functie returneaza un nod care are un anumit id primit ca parametru
TArb gasire_dupa_id(char * id, TArb a ) {
    int nr_niv, k, ok;
    char * number;
    number = strtok (id, SEP1);
    TArb w = a;
    ok = 1;
    //se imparte numarul in mai multe numere, folosind separatorl "."
    while (number && ok == 1) {//fiecare nuamru reprezinta un nivel
        nr_niv = atoll(number);//ltimul numar reprezinta nivelul pe care
        TArb aux = w->firstChild;//se afla nodul cautat, daca unul dintre
        k = 1;//numere nu are un nivel corespunzator in arbore,
        while (aux->nextSibling != NULL && k != nr_niv) {//inseamna ca nodul
            aux = aux->nextSibling;// cautat nu exista si se intoarce NULL
            k = k + 1;
        }
        if (k != nr_niv) ok = 0;
        w = aux;
        number = strtok (NULL, SEP1);
    }//se returneaza nodul
    if (ok == 0) {
        if (strcmp(id, "0") == 0) {
            return a;
        } 
        else return NULL;
    }
    else return w;
}
int main(int argc, char *argv[])  {
    if(!argc) printf("Nu s-au introdus denumirile pentru fisiere!");
    TArb  a = NULL;
    FILE * html, * input, * output;
	//se deschid fiserele din care se citesc comenzile si fisierul html
	html = fopen(argv[1], "rt");
	input = fopen(argv[2], "rt");
	//se deschide fisierul de iesire
	output = fopen(argv[3], "w");
    if(html == NULL || input == NULL || output == NULL) 
        printf("Eroare la deschiderea fisierului");
    //se apeleaza functia care "citeste" si creeaza arborele
 	a = citire(html);
    char * comanda = (char *)malloc(MAX_CMD_LEN * sizeof(char));
    if(!comanda) printf("Eroare de alocare");
    int n;
    //se citeste fiecare linie din fisierul de intrare
    while (fgets(comanda, MAX_CMD_LEN, input) != NULL) {
        n = strlen(comanda);
        comanda[ n - 1 ] = '\0';
        if(strcmp(comanda, "format")==0) {
            afisare(a, 0, output);//se apeleaza functia pentru 
                            //afisarea arborelul in forma indexata
        }
        if(strstr(comanda, "add")) {
            adauga(comanda, &a, output);//se apeleaza functia 
                            //care adauga un nod nou in arbore
        }
        if(strstr(comanda, "deleteRecursively")) {
            sterge_dupa_selector(&a, comanda, output);//se apeleaza functia 
            //care sterge din arbore nodurile care respecta anumite conditii
        }
        if(strstr(comanda, "overrideStyle")) {
            //se apeleaza functia corespunzatoare comenzii "overrideStyle"
            //parametrul "1" este specifica aceste comenzii
            override_and_append_dupa_selector(&a, comanda, output, 1);
        }
        if(strstr(comanda, "appendStyle")) {
            //se apeleaza functia corespunzatoare comeznii "appendStyle"
            //parametrul "0" este specifica acestei comenzii
            override_and_append_dupa_selector(&a, comanda, output, 0);
        }
    }
    //se elibereaza memoria folosita
    free(comanda);
    eliberare(&a);
	fclose(input);
	fclose(output);
	fclose(html);
	return 0;
}
