#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "functions.h"

extern void AddToString(char *s1, const char *s2, const char *s3, const char *s4, uint16_t *poszero)
{
  while ((*s1++ = *s2++) != '\0') (*poszero)++;
  if (!s3) return;
  *s1--;
  while ((*s1++ = *s3++) != '\0') (*poszero)++;
  if (!s4) return;
  *s1--;
  while ((*s1++ = *s4++) != '\0') (*poszero)++;
}
