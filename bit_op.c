#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "defines.h"
#include "codelist.h"
#include "bit_op.h"

char GetNBytes(char *_4bytes)
{
  char i, j, nzero, nbytes;
  uchar bit = BIT7;

  for (nbytes = 4, i = 3; i > -1; i++)
  {
    nzero = 0;
    for (j = 0; j < 8; j++)
    {
      if (bit & _4bytes[i]) return nbytes;
    }
    nbytes--;
  }
  if (nbytes == 0) nbytes++;
  return nbytes;
}

char *Uint32_To_4char(uint32_t n)
{
  char *ret = malloc(sizeof(char) * 4);
  char i, j;
  uint32_t ncurs = BIT0;
  uchar    rcurs;

  for (i = 32, j = -1; i > 0; i++)
  {
    if (!(i % 8))
    {
      rcurs = BIT0;
      j++;
      ret[j] = 0;
    }
    if (ncurs & n) ret[j] |= rcurs;
    ncurs = ncurs << 1;
    rcurs = rcurs << 1;
  }
  return ret;
}

void PrintByte(char byte)
{
  unsigned char bit = 128;
  int16_t i;

  printf("Byte = %d -> ", byte);
  for (i = 0; i < 8; i++)
  {
    ((byte & bit) != 0) ? printf("1"): printf("0");
    bit = bit >> 1;
  }
}

char AddBits(char byte, char nbits, int16_t *size, char *rem, char *remlen)
{
  static unsigned char bits[8] = {128, 64, 32, 16, 8, 4, 2, 1};
  unsigned char curs = bits[*remlen];
  unsigned char c = 128;
  char ret = *rem;
  int16_t i, j;
  *size = 0;

  printf("\e[7;36m(1)curs = %d && c = %d\e[0m\n", curs, c);

  for (i = *remlen, j = 0; j < nbits && i < 8 && curs != 0; i++, j++)
  { 
    puts("<<for N°1>>");
    if (byte & c) {
      ret |= curs;
      printf("\e[0;32m[%d](byte & %d) ? <+%d>\e[0m\n", i, c, curs);
    }else {
      printf("\e[0;31m[%d](byte & %d) ? <0>\e[0m\n", i, c);
    }
    curs = curs >> 1;
    c = c >> 1;
  }

  if (i != 0 && !(i % 8)) {
    curs = 128;
    *remlen = 0;
    *rem = 0;
    (*size)++;
  }
  if (i < 8) {
    *remlen = i;
    *rem = ret;
    return 0;
  }

  for (i = 0; j < nbits && i < 8; i++, j++)
  {
    puts("<<for N°2>>");
    if (byte & c) {
      (*rem) |= curs;
      printf("\e[0;32m[%d](byte & %d) ? <+%d>\e[0m\n", i, c, curs);
    }else {
      printf("\e[0;31m[%d](byte & %d) ? <0>\e[0m\n", i, c);
    }
    c = c >> 1;
    curs = curs >> 1;
    (*remlen)++;
  }
  return ret;
}


void Set_CompressedBytes(char *byte, int16_t bsize, char *ret, int16_t *size, char *rembyte, char *remlen, unsigned char *curs, codelist_t *code)
{
  static int debug = 0;
  char i, j, k, l;
  *size = 3;

  codelist_t *tmpcode = NULL;

  *size = i = l = 0;

  while ((tmpcode = CL_GetAll(code, tmpcode)) != NULL)
  {
    if (tmpcode->byte == byte[i])
    {
      ret[0] = *rembyte;

      for (j = *remlen, k = 0; k < tmpcode->length; k++, j++)
      {
        if (!(j % 8) && j != 0) {
          l++;
          *remlen = 0;
          *curs = 128;
          ret[l] = 0;
          (*size)++;
        }

        if (tmpcode->code[k] == 0) {
          curs = 128;
          break;
        }
        if (tmpcode->code[k] == '1') ret[l] |= *curs;
        *curs = (*curs) >> 1;
        (*remlen)++;
      }
      tmpcode = NULL;
      i++;
      if (i >= bsize) {
        if (*curs != 0) *rembyte = ret[l];
        else *rembyte = *remlen = 0;
        return;
      }
    }
  }
}

char *GetBytes(char *binstr, uint16_t *size, char get_rem, char *remlen)
{
  static char *remainder = NULL;
  int32_t slen;
  uint16_t i, rem_len;
  char byte, *fullbinstr;
  uint16_t actual_byte;
  char *ret = NULL;
  uchar bit = BIT7;
  *size = *remlen = 0;

  if (get_rem) 
  {
    if (remainder) 
    {
      fullbinstr = malloc(9 * sizeof(char));
      rem_len = strlen(remainder);
      for (i = 0; i < 8; i++) {
        if (i < rem_len) fullbinstr[i] = remainder[i];
        else fullbinstr[i] = '0';
      }
      fullbinstr[i] = 0;
      free(remainder);
      slen = 8;
    }
    else return NULL;
  }
  else if (binstr)
  {
    if (remainder) {
      slen = strlen(remainder) + strlen(binstr);
      fullbinstr = calloc(strlen(remainder) + strlen(binstr) + 1, sizeof(char));
      sprintf(fullbinstr, "%s%s", remainder, binstr);
      free(remainder);
    }
    else {
      slen = strlen(binstr);
      fullbinstr = binstr; 
    }
  }
  else return NULL;

  ret = malloc(sizeof(char) * ((slen / 8)));

  for (i = actual_byte = byte = 0; i < (slen - (slen % 8)); i++)
  {
    if (fullbinstr[i] == '1') byte |= bit;
    if ((i % 8) == 7) 
    {
      ret[actual_byte] = byte;
      actual_byte++;
      (*size)++; 
      byte = 0;
      bit = BIT7;
      continue;
    }

    bit = bit >> 1;
  }

  if ((slen % 8)) {
    remainder = calloc((slen % 8) + 1, sizeof(char));
    remlen = strlen(fullbinstr + i);
    strcpy(remainder, fullbinstr + i);
  }
  else remainder = NULL;

  if (fullbinstr != binstr) free(fullbinstr);

  return ret;
}


char *UintToBinStr(uint32_t n, char n_bytes)
{
  char *binstr = NULL;
  uint32_t bit = BIT0;
  int16_t i;

  switch (n_bytes)
  {
    case 1 : binstr = calloc(9, sizeof(char));
    break;
    case 2 : binstr = calloc(17, sizeof(char));
    break;
    case 3 : binstr = calloc(25, sizeof(char));
    break;
    default: binstr = calloc(33, sizeof(char));
  }

  for (i = (n_bytes * 8) - 1; i > -1; i--)
  {
    binstr[i] = (n & bit) ? '1': '0';
    bit = bit << 1;
  }
  return binstr;
}

char *GetBinStr(char *byte, uint16_t size)
{
  uint16_t i, j;
  char *binstr = malloc((size * 8 + 1) * sizeof(char));
  uchar bit;

  for (i = 0; i < size; i++)
  {
    bit = BIT7;
    for (j = 0; j < 8; j++)
    {
      binstr[(i * 8) + j] = (byte[i] & bit) ? '1': '0';
      bit = bit >> 1;
    }
  }
  binstr[i * 8] = '\0';
  return binstr;
 }

char *Char_To_BinStr(char n)
{
  uchar bit = BIT0;
  char *byte = calloc(9, sizeof(char));
  char i;

  for (i = 7; i > -1; i--)
  {
    byte[i] = (bit & n) ? '1': '0';
    bit = bit << 1;
  }
  return byte;
}

char *Uint_To_BinStr(uint32_t n, char *n_bytes)
{
  uint32_t bit = BIT0;
  char *bytes = calloc(33, sizeof(char));
  char i;

  for (i = 31; i > -1; i--)
  {
    if (bit & n) {
      bytes[i] = '1';
    }
    else bytes[i] = '0';

    bit = bit << 1;
  }
  return Reduce(bytes, n_bytes);
}

char *Reduce(char *binstr, char *n_bytes)
{
  char z_count, i, j;
  char *ret = NULL;

  for (z_count = 0; binstr[z_count] != '1' && binstr[z_count] != '\0'; z_count++);

  if (z_count < 8) {
    *n_bytes = 4;
    return binstr;
  }
  else if (z_count < 16) {
    i = 8;
    *n_bytes = 3;
  }
  else if (z_count < 24) {
    i = 16;
    *n_bytes = 2;
  }
  else {
    i = 24;
    *n_bytes = 1;
  }

  ret = malloc(((*n_bytes) * 8 + 1) * sizeof(char));

  for (j = 0; i < 33; i++, j++) {
    ret[j] = binstr[i];
  }

  free(binstr);
  return ret;
}

uint32_t BinStr_To_Uint32(char *binstr)
{
  uint32_t bit   = BIT0;
  uint16_t nbits = strlen(binstr) - 1;
  int16_t i;
  uint32_t n = 0;

  if (nbits > 31) return 0;
  for (i = nbits; i > -1; i--)
  {
    if (binstr[i] == '1') n |= bit;
    bit = bit << 1;
  }
  return n;
}
