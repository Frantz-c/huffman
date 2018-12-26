#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "codelist.h"

static int32_t Get_code(const char *code);
static void Print_strcode(const codelist_t *code);
static uint8_t Get_length(uint32_t cursor);

int32_t CL_get_sum_codes_length(codelist_t *cl)
{
  int32_t sum;

  for (sum = 0; cl; cl = cl->next)
  {
    sum += cl->length / 8 + (cl->length % 8) ? 1: 0;
  }

  return sum;
}

void Print_strcode(const codelist_t *code)
{
  uint32_t cursor = 1 << (code->length - 1);

  for (uint8_t i = 0; i < code->length; i++, cursor >>= 1) {
    (code->code & cursor) ? putchar('1'): putchar('0');
  }
}

uint8_t Get_length(uint32_t cursor)
{
  uint8_t i = 0;
  for (; cursor != 1; i++, cursor >>= 1);
  return i;
}

int32_t Get_code(const char *code)
{
  const char *ptr     = code;
  int32_t     bincode = 0;

  printf("Get_code()::\"%s\"", code);
  
  while (*ptr) ptr++;
  ptr--;

  for (uint32_t curs = 1; ptr >= code; ptr--, curs <<= 1)
  {
    if (*ptr == '1') bincode |= curs;
  }

  printf(" (%d)\n", bincode);
  return bincode;
}

extern void CL_AddAfter(codelist_t **list, char octet, int32_t code, uint32_t cursor)
{
  codelist_t *newLink = malloc(sizeof(codelist_t));
  codelist_t *tmp     = *list;
  newLink->byte       = octet;
  newLink->length     = Get_length(cursor);
  newLink->code       = code;
  newLink->next       = NULL ;

  if (!*list)
  {
    *list = newLink;
  }
  else
  {
    while (tmp)
    {
      if (!tmp->next) {
        tmp->next = newLink;
        break;
      }
      tmp = tmp->next;
    }
  }
}

extern void CL_Add(codelist_t **list, char octet, int32_t code, uint32_t cursor)
{
  codelist_t *link = malloc(sizeof(codelist_t));
  link->byte   = octet;
  link->length = Get_length(cursor);
  link->code   = code;
  link->next   = NULL;
  printf("length = %d\n", link->length);

  if (!*list)
    *list = link;
  else
    CL_AddAsc(list, link);
}

extern void CL_AddAsc(codelist_t **list, codelist_t *new)
{
  codelist_t *tmp = NULL;
  codelist_t *next = NULL;

  tmp = *list;

  while (tmp)
  {
    if (tmp->length >= new->length || !tmp->next) {
      if (tmp->next) {
        next = tmp->next;
      }
      tmp->next = new;
      new->next = next;
      return;
    }
    tmp = tmp->next;
  }
}

extern int32_t CL_Get(codelist_t *list, char octet)
{
  while (list)
  {
    if (list->byte == octet) return list->code;
    list = list->next;
  }

  return -1;
}

extern codelist_t *CL_GetN(codelist_t *link, char cible)
{
  char i;

  if (link == NULL) return NULL;
  if (cible <= 0) return link;

  for (i = -128; i != cible; i++)
  {
    if (link) link = link->next;
  }
  return (link) ? link: NULL;
}

extern codelist_t *CL_GetAll(codelist_t *link, codelist_t *prev)
{
  if (!prev) {
    prev = link;
    return prev;
  }
  return prev->next;
}

extern void Show_CodeList(codelist_t *list)
{
  while (list)
  {
    if      (list->byte > 99  ) printf(" %d'%c'", list->byte, list->byte);
    else if (list->byte > 9   ) printf(" %d '%c'", list->byte, list->byte);
    else if (list->byte > -1  ) printf(" %d  '%c'", list->byte, list->byte);
    else if (list->byte > -10 ) printf("%d  '%c'", list->byte, list->byte);
    else if (list->byte > -100) printf("%d '%c'", list->byte, list->byte);
    else                        printf("%d'%c'", list->byte, list->byte);

    printf(" = ");
    Print_strcode(list);
    printf("(%d)\n", list->length);

    list = list->next;
  }
}

extern void Free_codelist(codelist_t *list)
{
  codelist_t *tmp = NULL;

  while (list)
  {
    tmp  = list;
    list = list->next;
    free(tmp);
  }
}

extern uint16_t CL_LengthAverage(codelist_t *list, uint16_t n_codes)
{
  uint16_t average = 0;
  codelist_t *tmp = list;

  while (tmp)
  {
    average += strlen(tmp->code);
    tmp = tmp->next;
  }
  return ((average / n_codes) + 1);
}

extern uint16_t CL_SumLength(codelist_t *list)
{
  codelist_t *tmp = NULL;
  uint16_t sum = 0;
  
  while (tmp = CL_GetAll(list, tmp))
  {
    sum += tmp->length;
  }
  return sum;
}
