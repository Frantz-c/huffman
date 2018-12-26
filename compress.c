#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "defines.h"
#include "functions.h"
#include "occlist.h" 
#include "codelist.h"
#include "huffmantree.h"
//#include "bit_op.h"
#include "compress.h"

void Compress(const char *org, const char *dest)
{
  uint64_t    n_occurrences;
  uint64_t    occ_table[256] = {0};
  uint16_t    n_characters;
  occlist_t  *list_of_occ    = NULL;
  codelist_t *list_of_codes  = NULL;
  FILE       *orgfile        = NULL;
  FILE       *destfile       = NULL;

  if ((list_of_occ = Get_occurrences_list(org, occ_table, &n_occurrences, &n_characters)) == NULL)
  {
    fprintf(stderr, "\e[0;31miFichier \"%s\" trop volumineux !\e[0m\n", org);
    return;
  }
  /*Show_OccList(list_of_occ);*/
  list_of_codes = New_codelist(list_of_occ, n_occurrences, n_characters);
  puts("\e[0;32mConstitution de la liste chainée (codelist_t *) terminée !\e[0m");
  Show_CodeList(list_of_codes);

  OPENFILE(org,  "r", orgfile,  ERR_FATAL);
  OPENFILE(dest, "w", destfile, ERR_FATAL);

  puts("\e[0;37mÉcriture de l'en-tête...\e[0m");
  Write_header(list_of_occ, list_of_codes, occ_table, n_occurrences, n_characters, destfile);
  puts("\e[0;32mÉcriture de l'en-tête terminée !\e[0m");


  puts("\e[0;37mÉcriture du corps...\e[0m");
  Write_body(list_of_codes, n_occurrences, n_characters, orgfile, destfile);
  puts("\e[0;32mÉcriture du corps terminée !\e[0m");

  CLOSEFILE(org, orgfile, ERR_NOT_FATAL);
  CLOSEFILE(dest, destfile, ERR_FATAL);

  Free_occlist (list_of_occ);
  Free_codelist(list_of_codes);

  puts("Fin du programme.");
}

void Init_array(char *a, int32_t n)
{
  for (int32_t i = 0; i < n; i++, a++) *a = 0;
}

char *Get_byte_array_of(uint64_t n, char len)
{
  static char byte_array[8];

  uint64_t src_curs = 1;
  uint8_t  dst_curs;

  Init_array(byte_array, 8);

  for (char i = 0; i < len; i++)
  {
    for (dst_curs = 1; dst_curs != 128; dst_curs <<= 1, src_curs <<= 1)
    {
      if (n & src_curs) byte_array[i] |= dst_curs;
    }
  }

  return byte_array;
}

void *Get_int_from_byte_array(char *a, char len)
{
  //
}

void Write_body(
                 codelist_t *list_of_codes,
                 uint64_t    n_occurrences,
                 uint16_t    n_characters,
                 FILE       *org,
                 FILE       *dest
               )
{
  int   write_length;
  char *write;
  char c;

  printf("\e[1;36mftell(org) = %d\n\e[0m\n", ftell(org));
  while (fread(&c, sizeof(char), 1, org) == 1)
  {
    printf("c = '%c'\n", c);
    if ((write_length = Get_code(list_of_codes, c, &write)) != 0) {
      fwrite(&write, sizeof(char), write_length, dest);
      Show_writed_code(write, write_length);
      putchar(' ');
    }
    printf("write_length = %d\n", write_length);
  }
  if ((write_length = Get_code(NULL, 0, &write)) != 0) {
    fwrite(&write, sizeof(char), write_length, dest);
    Show_writed_code(write, write_length);
    puts(" {{END}}");
  }
}

int Get_code(codelist_t *list_of_codes, char c, char **write)
{
  static char    buf[8] = {0};
  static int     bit    = BIT1; // bit actuel
  static char    cur    = 0;    // octet actuel dans le tableau

  int            bit2   = BIT1;
  int            maxbit;
  
    
  // Initialisation du buffer en gardant en mémoire l'octet incomplet
  buf[0] = (bit == BIT1) ? 0: buf[cur];
  for (int i = (cur > 0) ? cur: 1; i < 8; i++) buf[i] = 0;
  cur = 0;

  // Si le pointeur sur codelist est NULL, on retourne le reste du buffer
  if (list_of_codes) 
  {
    // Recherche du code à enregistrer
    for (; list_of_codes && list_of_codes->byte != c; list_of_codes = list_of_codes->next);

    maxbit = bit2 << list_of_codes->length;
  }
  else
  {
    if (bit > BIT1) cur = 1;
    goto end;
  }

  // Écriture du code dans le buffer
  while (cur < 8)
  {
    while (bit2 < maxbit)
    {
      if (list_of_codes->code & bit2) {
        buf[cur] |= bit;
      }

      bit2 <<= 1;
      bit  <<= 1;

      if (bit > BIT8) {
        bit = BIT1;
        cur++;
        break;
      }
    }

    if (bit2 == maxbit) {
      break;
    }
  }

  end:
  *write = buf;
  return cur; // longueur du buffer
}

void Show_writed_code(char *code, int length)
{
  for (int cur = 0; cur < length; cur++)
  {
    for (int bit = BIT1; bit <= BIT8; bit <<= 1)
    {
      (code[cur] & bit) ? putchar('1'): putchar('0');
    }
  }
}

void Write_header(
                  occlist_t  *list_of_occ,
                  codelist_t *list_of_codes,
                  uint64_t    occ_table[],
                  uint64_t    n_occurrences,
                  uint16_t    n_characters,
                  FILE       *destfile
                )
{
  uint32_t bigger_occ_number;
  uint8_t  longer_code_length;
  char     first_byte;   
  char    *byte_array;


  printf("n_occurrences = %ld\n", n_occurrences);

  longer_code_length = Get_longer_code_length(list_of_codes);
  bigger_occ_number  = Get_bigger_occurrences_number(list_of_occ);
  first_byte         = Get_header_write_method
                       (
                         longer_code_length, 
                         bigger_occ_number, 
                         n_occurrences,
                         list_of_occ,
                         list_of_codes
                       );

  byte_array = Get_byte_array_of(n_occurrences, first_byte & BIT8 ? 4: 8);
  fwrite(&first_byte, sizeof(char), 1, destfile);
  fwrite(byte_array, sizeof(char), first_byte & BIT8 ? 4: 8, destfile);


  if      (first_byte & BIT1) {
    Header_write_occurrences_array(first_byte, n_occurrences, occ_table, destfile);
  }
  else if (first_byte & BIT3) {
    Header_write_codes_array(first_byte, n_occurrences, list_of_codes, destfile);
  }

  if (first_byte & BIT2) {
    Header_write_occurrences_array_assoc(first_byte, n_occurrences, list_of_occ, destfile);
  }
  else {
    Header_write_codes_array_assoc(first_byte, n_occurrences, list_of_codes, destfile);
  }
  
}

void Header_write_codes_array(
                               char first_byte,
                               uint64_t n_occurrences,
                               codelist_t *list_of_codes,
                               FILE *dest
                             )
{
  int     code_length;
  char   *code;
  char    i           = 127; // retombera à -128 à la première instruction "i++"
  int64_t null        = 0;
  
  do
  {
    i++;

    fwrite(&i, sizeof(char), 1, dest);

    if (list_of_codes && list_of_codes->byte == i)
    {
      fwrite(&list_of_codes->length, sizeof(char), 1, dest);

      code_length = list_of_codes->length / 8 + (list_of_codes->length % 8) ? 1: 0;
      code = Get_byte_array_of(list_of_codes->code, code_length);
      fwrite(code, sizeof(char), code_length, dest);

      list_of_codes = list_of_codes->next;
    }
    else
    {
      fwrite(&null, sizeof(char), 1, dest);
    }
  }
  while (i != 127);

}
void Header_write_codes_array_assoc(
                                     char first_byte,
                                     uint64_t n_occurrences,
                                     codelist_t *list_of_codes,
                                     FILE *dest
                                   )
{
  int   code_length;
  char *code;

  for (; list_of_codes; list_of_codes = list_of_codes->next)
  {
    fwrite(&list_of_codes->byte, sizeof(char), 1, dest);

    fwrite(&list_of_codes->length, sizeof(char), 1, dest);

    code_length = list_of_codes->length / 8 + (list_of_codes->length % 8) ? 1: 0;
    code = Get_byte_array_of(list_of_codes->code, code_length);
    fwrite(code, sizeof(char), code_length, dest);
  }
}

void Header_write_occurrences_array(
                                     char first_byte,
                                     uint64_t n_occurrences,
                                     occlist_t *list_of_occ,
                                     FILE *dest
                                   )
{
  int     occ_length  = Get_length_of_occurrences(first_byte);
  char   *occurrences;
  char    i           = 127; // retombera à -128 à la première instruction "i++"
  int64_t null        = 0;
  
  do
  {
    i++;

    fwrite(&i, sizeof(char), 1, dest);

    if (list_of_occ && list_of_occ->byte == i)
    {
      occurrences = Get_byte_array_of(list_of_occ->occ, occ_length);
      list_of_occ = list_of_occ->next;
    }
    else if (occurrences != &null)
    {
      occurrences = &null;
    }

    fwrite(occurrences, sizeof(char), occ_length, dest);
  }
  while (i != 127);
}

void Header_write_occurrences_array_assoc(
                                           char first_byte,
                                           uint64_t n_occurrences,
                                           occlist_t *list_of_occ,
                                           FILE *dest
                                         )
{
  char *occurrences;
  int   occ_length = Get_length_of_occurrences(first_byte);

  for (; list_of_occ; list_of_occ = list_of_occ->next)
  {
    fwrite(&list_of_occ->byte, sizeof(char), 1, dest);

    occurrences = Get_byte_array_of(list_of_occ->occ, occ_length);
    fwrite(occurrences, sizeof(char), occ_length, dest);
  }
}

int Get_length_of_occurrences(char data)
{
  if (data & BIT5) return 1;
  if (data & BIT6) return 2;
  if (data & BIT7) return 3;
  return 0;
}

void Write_header_occurrences(
                               char      first_byte,
                               uint64_t  n_occurrences,
                               uint64_t  occ_table[],
                               FILE     *destfile
                             )
{
  char *tmp;
  char  size;

  if      (first_byte & BIT1) size = 1;
  else if (first_byte & BIT2) size = 2;
  else                        size = 3;


  // Write the method's byte
  if (fwrite(&first_byte, sizeof(char), 1, destfile) != 1) goto write_error;

  // Write the file length's bytes
  if (first_byte & BIT7) { 
    tmp = Get_byte_array_of(n_occurrences, 4);
    if (fwrite(tmp, sizeof(char), 4, destfile) != 4) goto write_error;
  }
  else {
    tmp = Get_byte_array_of(n_occurrences, 8);
    if (fwrite(tmp, sizeof(char), 8, destfile) != 8) goto write_error;
  }

  // Write the number of occurrences of each byte
  for (uint16_t i = 0; i < 256; i++)
  {
    tmp = Get_byte_array_of(occ_table[i], size);
    if (fwrite(tmp, sizeof(char), size, destfile) != size) goto write_error;
  }

  return;

  write_error:
  fprintf(stderr, "Erreur d'écriture Write_header_occurrences()\n");
  exit(EXIT_FAILURE);
}

char Get_header_write_method(
                              uint8_t     longer_code_length,
                              uint64_t    bigger_occ_number,
                              uint64_t    n_occurrences,
                              occlist_t  *occlist,
                              codelist_t *codelist
                            )
{
  uint32_t header_length = 0;
  uint32_t n;
  uint32_t codes_length  = CL_get_sum_codes_length(codelist);
  uint16_t count         = OL_get_element_count(occlist);
  char     write_method  = 0;

  printf("count = %d\n", count);

  // tableau de codes (uint8 + uintX)
  header_length = 256 + codes_length;
  write_method = BIT3;
  
  // tableau associatif de codes (char + uint8 + uintX)
  n = count * 2 + codes_length;
  if (header_length > n) {
    write_method = BIT4;
    header_length = n;
  }
  
 
  if (bigger_occ_number < 16777216)
  {
    // tableau d'occurrences (uintX)
    if      (bigger_occ_number > 65535)
    {
      n = 256 * 3;
      if (header_length > n) {
        write_method = BIT1 + BIT7;
        header_length = n;
      }
    }
    else if (bigger_occ_number > 255)
    {
      n = 256 * 2;
      if (header_length > n) {
        write_method = BIT1 + BIT6;
        header_length = n;
      }
    }
    else
    {
      n = 256 * 1;
      if (header_length > n) {
        write_method = BIT1 + BIT5;
        header_length = n;
      }
    }
    
    // tableau associatif d'occurrences (char + uintX)
    if (bigger_occ_number > 65535) {
      n = count * 4;
      if (header_length > n) {
        write_method = BIT2 + BIT7;
        header_length = n;
      }
    }
    else if (bigger_occ_number > 255) {
      n = count * 3;
      if (header_length > n) {
        write_method = BIT2 + BIT6;
        header_length = n;
      }
    }
    else {
      n = count * 2;
      if (header_length > n) {
        write_method = BIT2 + BIT5;
        header_length = n;
      }
    }
  }


  if (n_occurrences < 4294967296) write_method |= BIT8;
  Print_method(write_method);
  printf("header_length = %d\n", header_length + 1 + ((write_method & BIT8) ? 4: 8));

  return write_method;
}

void Print_method(char m)
{
  if      (m & BIT1) printf("Tableau d'occurrences : ");
  else if (m & BIT2) printf("Objets d'occurrences : ");
  if      (m & BIT3) printf("Tableau de codes : ");
  else if (m & BIT4) printf("Objets de codes : ");

  if (m & BIT8) printf("[uint32] ");
  else          printf("[uint64] ");

  if (m & BIT2 || m & BIT4) printf("[(byte) uint8] ");
  if      (m & BIT5) printf("[uint8]\n");
  else if (m & BIT6) printf("[uint16]\n");
  else if (m & BIT7) printf("[uint24]\n");
  else               printf("[uint8 + intX]\n");
}

uint8_t Get_longer_code_length(codelist_t *list_of_codes)
{
  uint8_t length = 0;

  while (list_of_codes)
  {
    if (length < list_of_codes->length)
    {
      length = list_of_codes->length;
    }
    list_of_codes = list_of_codes->next;
  }

  return length;
}


uint32_t Get_bigger_occurrences_number(occlist_t *list_of_occ)
{
  occlist_t *list = NULL;
  uint32_t bigger = 0;

  for (; list; list = list->next)
  {
    if (list->occ > bigger) {
      bigger = list->occ;
    }
  }
  return bigger;
}

occlist_t *Get_occurrences_list(const char *filename, uint64_t *occurrences_table, uint64_t *n_occurrences, uint16_t *n_characters)
{
  FILE      *input_file             = NULL;
  occlist_t *occurrences_list       = NULL;
  char       byte;

  *n_occurrences     = *n_characters = 0;


  OPENFILE(filename, "r", input_file, ERR_FATAL);

  for (*n_occurrences = 0; fread(&byte, sizeof(char), 1, input_file); (*n_occurrences)++) {
    if (*n_occurrences > 18446744000000000000) {
      *n_occurrences = 0;
      break;
    }
    occurrences_table[byte + 128]++;
  }
  CLOSEFILE(filename, input_file, ERR_NOT_FATAL);
  if (*n_occurrences == 0) return NULL;

  Set_occurrences_list     (occurrences_table, &occurrences_list);
  Set_occurrences_list_data(occurrences_list, n_occurrences, n_characters);

  return occurrences_list;
}

void Set_occurrences_list(uint64_t t[], occlist_t **list)
{
  int16_t i;

  for (i = 0; i < 256; i++, t++)
  {
    if (*t > 0) 
    {
      OL_add(list, i - 128, *t);
    }
  }
}

void Set_occurrences_list_data(occlist_t *list, uint64_t *n_occurrences, uint16_t *n_characters)
{
  while (list)
  {
    *n_occurrences += list->occ;
    (*n_characters)++;
    list = list->next;
  }
}

codelist_t *New_codelist(occlist_t *list, uint64_t n_occurrences, uint16_t n_characters)
{
  tree_t     *tree       = NULL;
  codelist_t *codelist   = NULL;

  puts("\e[0;37mConstruction de l'arbre...\e[0m");
  tree = NewTree(NO_FLAG);
  Set_HuffmanTree(n_occurrences, n_characters, list, tree);
  puts("\e[0;32mArbre construit !\e[0m");
  puts("\e[0;37mCréation de la liste chainée (codelist_t *)...\e[0m");
  Set_CodeList(tree, &codelist, 0, 1);
  puts("\e[0;32mListe chainée créée !\e[0m");

  Free_Tree(&tree);

  return codelist;
}
