//Vasile Madalin Constantin 312 CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef __STRUCTURES_AND_FUNCTIONS__
#define __STRUCTURES_AND_FUNCTIONS__

#define MAX_CMD_LEN 300
#define CMAX 1000
#define S_LEN 50
#define SEP ";"
#define SEP1 "."

typedef struct TNodAttr {
	char * name;
	char * value;
	struct TNodAttr *next;
} TNodAttr, *TAttr;

typedef struct TNodInfo {
	char * type;
	char * id;
	TAttr style;
	TAttr otherAttributes;
	int isSelfClosing;
	char * contents;
} TNodInfo, *TInfo;

typedef struct TNodArb {
	TInfo info;
	struct TNodArb *nextSibling;
	struct TNodArb *firstChild;
	//struct TNodArb * Parinte;
} TNodArb, *TArb;
 
 typedef enum
{
    PARSE_ERROR             = 0,
    PARSE_CONTENTS          = 1,
    PARSE_OPENING_BRACKET   = 2,
    PARSE_TAG_TYPE          = 3,
    PARSE_CLOSING_TAG       = 4,
    PARSE_REST_OF_TAG       = 5,
    PARSE_ATTRIBUTE_NAME    = 6,
    PARSE_ATTRIBUTE_EQ      = 7,
    PARSE_ATTRIBUTE_VALUE   = 8,
    PARSE_SELF_CLOSING      = 9,
} TParseState;

void eliberare(TArb * a);
TArb gasire_dupa_id(char * id, TArb a );
void sterge_dupa_selector(TArb * a, char * comanda, FILE * output);
char * id_pentru_frate(char  s[CMAX]);
char * id_pentru_fiu(char s[CMAX]);
void DistrugeL(TAttr* aL);
void gaseste_nume_si_valoare(TArb *c, char * atribut, int sw);
TArb gasire_dupa_id(char * id, TArb a );
void afisare(TArb a, int d, FILE * output);
void adauga(char * comanda, TArb * a, FILE * output);
TArb alocare_arbore();
void override_and_append_dupa_selector(TArb * a, char * comanda, 
	FILE * output, int sw);

#endif /* _STRUCTURES_AND_FUNCTIONS_ */
