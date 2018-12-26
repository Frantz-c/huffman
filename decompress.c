#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "defines.h"
#include "functions.h"
#include "codelist.h"
#include "bit_op.h"
#include "decompress.h"

extern void Decompress(const char *org, const char *dest)
{
  FILE     *orgfile, *destfile  ;
  char     *remainder     = NULL;
  char     *tmpheader     = NULL;
  codelist_t *list_of_codes = NULL;
  uint32_t  n_occurrences       ;
  uint16_t  n_characters,  curs ;

  OpenFile(org , "r", orgfile , ERR_FATAL);
  OpenFile(dest, "w", destfile, ERR_FATAL);

  if ((list_of_codes = Read_header(orgfile)) == NULL)
  {
    printf("\e[0;31m");
    fprintf(stderr, "'%s': Mauvais format.\n", org);
    printf("\e[0m");
    exit(EXIT_FAILURE);
  }

  if (Read_body(orgfile, destfile, list_of_codes) == -1)
  {
    printf("\e[0;31m");
    fprintf(stderr, "fichier '%s' corrompu.\n", org);
    printf("\e[0m");
    exit(EXIT_FAILURE);
  }

  Free_CodeList(list_of_codes);

  CloseFile(org , orgfile , ERR_NOT_FATAL);
  CloseFile(dest, destfile, ERR_FATAL);
}

/*
static void D_WriteBody(codelist_t *codelist, FILE *orgfile  , 
                      FILE     *destfile, char *remainder, uint32_t n_occurrences)
{
  int16_t size;
  int16_t read_size;
  uint32_t count = 0;
  int16_t i;
  char *readptr = malloc(32 * sizeof(char));
  char *bytes = TranslateCodes(codelist, remainder, &size, n_occurrences, &count);
  char *binstr = NULL;
  free(remainder);
  
  for (i = 0; i < size; i++)
  {
    if (!(fwrite(bytes + i, sizeof(char), 1, destfile)) ) {
      puts("Erreur lors de l'écriture du reste");
      exit(EXIT_FAILURE);
    }
  }
  free(bytes);

  while ( (read_size = fread(readptr, sizeof(char), 32, orgfile)) )
  {
    binstr = GetBinStr(readptr, read_size);
    bytes = TranslateCodes(codelist, binstr, &size, n_occurrences, &count);
    free(binstr);
    for (i = 0; i < size; i++)
    {
      if (!(fwrite(bytes + i, sizeof(char), 1, destfile)) ) {
        puts("Erreur lors de l'écriture du reste");
        exit(EXIT_FAILURE);
      }
    }
    free(bytes);
  }
  free(readptr);
  puts("\e[0;32mDécompression terminée !\e[0m\n");
}

static char *TranslateCodes(codelist_t *codelist, char     *binstr       , 
                            int16_t  *size    , uint32_t  n_occurrences, uint32_t *count)
{
  static char *remainder = NULL;
  codelist_t *tmpcode = NULL;
  char     *bs      = NULL;
  char     *tmp     = NULL;
  char     *ret     = NULL;
  int16_t  bs_size  = strlen(binstr); 
  int16_t  curs    =  0  ;
  *size = 0;

  if (remainder) {
    bs_size += strlen(remainder);
    bs = calloc(bs_size + 1, sizeof(char));
    sprintf(bs, "%s%s", remainder, binstr);
  }
  else bs = binstr;

  while (1)
  {
    while ((tmpcode = CL_GetAll(codelist, tmpcode)) != NULL)
    {
      if (CmpCodes(tmpcode->code, bs + curs) == MATCH)
      {
        AddToTab(&ret, tmpcode->byte, size);
        curs += strlen(tmpcode->code);
        if (++(*count) >= n_occurrences) tmpcode = NULL;
        break;
      }
    }
    if (!tmpcode) break;
    tmpcode = NULL;
  }
  if (curs < bs_size) {
    tmp = calloc(bs_size - curs + 1, sizeof(char));
    strcpy(tmp, bs + curs);
  }

  if (remainder) {
    free(bs);
    free(remainder);
  }
  remainder = tmp;
  return ret;
}

static char CmpCodes(const char *code, const char *s)
{
  if (!code || !s) return ERR;

  while (*code != 0) if (*code++ != *s++) return NO_MATCH;
  return MATCH;
}

static void AddToTab(char **tab, char value, int16_t *size)
{
  char *tmp = NULL;
  
  if (!*tab) {
    *tab = malloc(sizeof(char));
    **tab = value;
    *size = 1;
    return;
  }

  tmp = malloc(((*size) + 1) * sizeof(char));
  ArrayCopy(tmp, *tab, value, size);
  free(*tab);
  *tab = tmp;
}

static void ArrayCopy(char dest[], const char org[], char value, int16_t *size)
{
  int16_t i;

  for (i = 0; i < *size; i++) *dest++ = *org++;
  *dest = value;
  (*size)++;
}

static codelist_t *GetCodeList(char     **remainder   , char    *tmpheader, 
                              uint16_t   n_characters, FILE    *orgfile  , uint16_t curs)
{
  char     *readptr, *tmp, *tmp2, *header;
  uint16_t i, poszero;
  codelist_t *codelist;
  readptr = tmp = tmp2 = header = codelist = NULL;
  poszero = 0;


  header  = calloc(strlen(tmpheader + curs) + (n_characters * 24) + 50 + 1, sizeof(char)); 
  poszero = 0;
  AddToString(header, tmpheader + curs, NULL, NULL, &poszero);
  free(tmpheader);

  readptr = malloc((3 * n_characters) * sizeof(char));
  if (fread(readptr, sizeof(char), (3 * n_characters), orgfile) != 3 * n_characters) {
    puts("Erreur lors de la lecture des codes !");
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < 3 * n_characters; i++)
  {
    tmp = Char_To_BinStr(readptr[i]);
    AddToString(header + poszero, tmp, NULL, NULL, &poszero);
    free(tmp);
  }
  free(readptr);

  printf("%d bits >> %d bits\n", i * 8, (n_characters * 24) + 50);
  printf("header : %s\n", header);

  D_Set_CodeList(&codelist, header, n_characters, &curs);
  
  *remainder = calloc(strlen(header + curs) + 1, sizeof(char));
  strcpy(*remainder, header + curs);
  puts("codelist = ");
  Show_CodeList(codelist);
  free(header);

  return codelist;
}

static void D_Set_CodeList(codelist_t **codelist    , char     *header, 
                         uint16_t   n_characters, uint16_t *curs  )
{
  char  byte;
  char *tmp  = malloc(20 * sizeof(char));
  char *tmp2 = NULL;
  uint16_t codelen = 0;
  int16_t i, k;

  for (i = *curs = 0; i < n_characters; i++)
  {
    tmp[8] = '\0';
    for (k = 0; k < 8; (*curs)++, k++) tmp[k] = header[*curs];
    byte = (char)BinStr_To_Uint32(tmp);

    if (i == 0) 
    {
      for (k = 0; k < 3; k++, (*curs)++) tmp[k] = header[*curs];
      tmp[k] = '\0';
      codelen = (uint16_t)BinStr_To_Uint32(tmp) + 1;
    }
    else
    {
      for (; header[*curs] == '1'; (*curs)++, codelen++);
      (*curs)++;
    }

    for (k = 0; k < codelen; (*curs)++, k++) tmp[k] = header[*curs];
    tmp[k] = 0;

    tmp2 = calloc(strlen(tmp) + 1, sizeof(char));
    strcpy(tmp2, tmp);
    CL_AddAfter(codelist, byte, tmp2);
  }
  free(tmp);
}

static void Set_HeaderInfo(char    **tmpheader   , uint32_t *n_occurrences, 
                           uint16_t *n_characters, FILE     *orgfile      , uint16_t *curs)
{
  uint16_t endpos_size = 1;
  uint16_t poszero     = 0;
  int16_t i;
  char *tmp, *readptr;
  *curs = 0;

  readptr = malloc(sizeof(char));

  if( !(fread(readptr, sizeof(char), 1, orgfile)) ) {
    puts("Erreur lecture premier octet !");
    exit(EXIT_FAILURE);
  }

// Récupération de la taille en octets de n_occurrences (2 bits) 
  if (*readptr & 128) endpos_size += 2;
  if (*readptr & 64 ) endpos_size += 1;
  printf("taille nombre de caractères = %d octets\n", endpos_size);

  tmp = Char_To_BinStr(*readptr);
  free(readptr);
// Écriture des bits non-interprêtés dans le header temporaire
  *tmpheader = calloc( ((endpos_size + 1) * 8 + 6 + 1), sizeof(char) );
  AddToString(*tmpheader, tmp + 2, NULL, NULL, &poszero);
  free(tmp);

// Récupération de n_ocurrences & n_characters
  readptr = malloc((endpos_size + 1) * sizeof(char));
  if (fread(readptr, sizeof(char), (endpos_size + 1), orgfile) != endpos_size + 1) {
    puts("Erreur lecture n_occurrences & n_characters !");
    exit(EXIT_FAILURE);
  }

// Ajout des données dans le header temporaire
  for (i = 0; i < (endpos_size + 1); i++) {
    tmp = Char_To_BinStr(readptr[i]);
    AddToString((*tmpheader) + poszero, tmp, NULL, NULL, &poszero);
    free(tmp);
  }
  free(readptr);

  tmp = malloc( (endpos_size * 8 + 1) * sizeof(char));
// Récupération du nombre d'octets du fichier (1 à 4 octets)
  for (*curs = 0; *curs < endpos_size * 8; (*curs)++)
  {
    tmp[*curs] = (*tmpheader)[*curs];
  }
  tmp[*curs] = '\0';
  *n_occurrences = BinStr_To_Uint32(tmp) + 1;
  printf("nombre de caractères = %d (%s)\n", *n_occurrences, tmp);

// Récupération du nombre d'octets différents contenus dans le fichier (1 octet)
  for (i = 0; i < 8; (*curs)++, i++)
  {
    tmp[i] = (*tmpheader)[*curs];
  }
  tmp[i] = '\0';
  *n_characters = BinStr_To_Uint32(tmp) + 1;
  printf("nombre d'octets différents = %d\n", *n_characters);
  free(tmp);
}
*/
