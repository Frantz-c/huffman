/*
** linked_list.h
** 18.09.2017
*/

#ifndef FC_occlist_t_141107_H
#define FC_occlist_t_141107_H

/* STRUCTURE =========================*/

typedef struct occlist_t
{
  char     byte;
  uint32_t occ;
  struct   occlist_t *next;
}
occlist_t;

/* PROTOTYPES ========================*/


/* Ajoute un noeud à la liste chainée, tout en maintenant 
** la liste triée par ordre décroissant
** - occlist_t **list = liste chainée dans laquelle ajouter un noeud
** - short octet = valeur de l'octet
** - uint32_t occ = nombre d'occurrences de l'octet*/
extern void OL_add(occlist_t **list, char octet, uint32_t occ);

/* utilisée par addLinkFrom, ajoute un noeud tout en maintenant la chaine triée...*/
       void OL_add2(occlist_t **list, occlist_t *newNode, occlist_t *before);


/* */
extern void addLinkInOrder(occlist_t **list, char octet, uint32_t occ);

/* */
       void addLinkInOrder2(occlist_t **list, occlist_t *newNode);


/* Supprime tous les noeuds de la liste chainée*/
extern void Free_occlist(occlist_t *list);


/* Retourne le n-ième noeud de la liste chainée
** - occlist_t *link = liste chainée
** - int cible = position de l'élément dans la liste
** - valeur de retour = élément trouvé ou NULL si [cible] est trop grand */
extern occlist_t *OL_Get(occlist_t *link, char cible);

extern occlist_t *OL_GetAll(occlist_t *link);

extern int16_t OL_get_element_count(occlist_t *ol);

extern void Show_OccList(occlist_t *list); 

#endif
