#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "defines.h"
#include "occlist.h"

extern int16_t OL_get_element_count(occlist_t *ol)
{
  int16_t count;
  for (count = 0; ol; ol = ol->next, count++);
  return count;
}

extern void addLinkInOrder(occlist_t **list, char octet, uint32_t occ)
{
  occlist_t *newNode = malloc(sizeof(occlist_t));
  newNode->next = NULL;
  newNode->byte = octet;
  newNode->occ = occ;

  if (!*list)
  {
    *list = newNode;
  }
  else if (!(*list)->next) {
    (*list)->next = newNode;
  }
  else {
    addLinkInOrder2(&(*list)->next, newNode);
  }
}

void addLinkInOrder2(occlist_t **list, occlist_t *newNode)
{
  if ((*list)->next) {
    addLinkInOrder2(&(*list)->next, newNode);
  }
  else {
    (*list)->next = newNode;
  }
}

void OL_add2(occlist_t **list, occlist_t *newNode, occlist_t *before)
{
  if (newNode->occ < (*list)->occ)
  {
    if ((*list)->next && newNode->occ < (*list)->next->occ)
    {
      OL_add2(&(*list)->next, newNode, *list);
    }
    else
    {
      newNode->next = (*list)->next;
      (*list)->next = newNode;
    }
  }
  else
  {
    newNode->next = *list;
    if (!before) *list = newNode;
  }
}

extern void OL_add(occlist_t **list, char octet, uint32_t occ)
{
  occlist_t *newNode = malloc(sizeof(occlist_t));
  newNode->byte = octet;
  newNode->occ = occ;
  newNode->next = NULL;
  
  if (!*list) {
    *list = newNode;
  }
  else {
    OL_add2(list, newNode, NULL);
  }
}

extern void Show_OccList(occlist_t *list)
{
  if (list)
  {
    if      (list->byte > 99  ) printf(" %d", list->byte);
    else if (list->byte > 9   ) printf(" %d ", list->byte);
    else if (list->byte > -1  ) printf(" %d  ", list->byte);
    else if (list->byte > -10 ) printf("%d  ", list->byte);
    else if (list->byte > -100) printf("%d ", list->byte);
    else                         printf("%d", list->byte);
    printf(" : x%d\n", list->occ);

    if (list->next) {
      Show_OccList(list->next);
    }
  }
}

extern void Free_occlist(occlist_t *list)
{
  occlist_t *tmp = NULL;

  while (list)
  {
    tmp  = list;
    list = list->next;
    free(tmp);
  }
}

extern occlist_t *OL_GetAll(occlist_t *link)
{
  static occlist_t *tmp = NULL;

  if (!tmp) tmp = link;
  else tmp = tmp->next;

  return tmp;
}

extern occlist_t *OL_Get(occlist_t *link, char cible)
{
  int i;

  if (link == NULL) return NULL;
  if (cible <= 0) return link;

  for (i = 0; i < cible; i++)
  {
    if (link) link = link->next;
  }
  return (link) ? link: NULL;
}
