/*
** bitcode_list.h
** 21.09.2017
*/

#ifndef FC_codelist_t_210917_H
#define FC_codelist_t_210917_H


/* STRUCTURES =========================*/

typedef struct codelist_t
{
  char    byte;
  int32_t code;
  uint8_t length;

  struct codelist_t *next;
}
codelist_t;

/* PROTOTYPES =========================*/


extern int32_t CL_get_sum_codes_length(codelist_t *cl);


/* Ajoute un élément à la suite de la liste chainée.
** Si *list vaut NULL, crée la liste.
** - codelist_t **list = liste dans laquelle ajouter l'élément
** - short octet = valeur de l'octet à ajouter
** - char *code = code binaire de l'octet à ajouter sous forme de chaine*/
extern void CL_AddAfter(codelist_t **list, char octet, int32_t code, uint32_t cursor);


/* Retourne le code sous forme de chaine binaire correspondant 
** à l'octet envoyé en paramètre dans la liste chainée
** - codelist_t *list = liste chainée dans laquelle chercher l'octet
** - char octet = octet à rechercher
** - valeur de retour = code binaire sous forme de chaine binaire */
extern int32_t CL_Get(codelist_t *list, char octet);


/* Retourne le n-ième élément de la liste chainée
** - codelist_t **list = liste chainée
** - short cible = numéro du noeud recherché dans la liste*/
extern codelist_t *CL_GetN(codelist_t *link, char cible);


extern codelist_t *CL_GetAll(codelist_t *link, codelist_t *prev);


/* Affiche le contenu de la liste chainée*/
extern void Show_CodeList(codelist_t *list);


/* Supprime tous les éléments de la liste chainée*/
extern void Free_codelist(codelist_t *list);



extern uint16_t CL_SumLength(codelist_t *list);
extern void CL_AddAsc(codelist_t **list, codelist_t *link);
extern void CL_Add(codelist_t **list, char octet, int32_t code, uint32_t cursor);
extern uint16_t CL_LengthAverage(codelist_t *list, uint16_t n_codes);

#endif
