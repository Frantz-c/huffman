#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BIT0 1
#define uchar unsigned char
#define EQUAL 0

char *Char_To_BinStr(char n);
char *Char_To_HexStr(char sn);

int main(int argc, char *argv[])
{
  FILE *file = NULL;
  char tmp[100] = {0};
  char *ptr = NULL;
  uint16_t n_bytes = 0;
  int i;
  uchar val = 0;
  uchar offset = 0;

  if (argc == 3) 
  {
    if ((file = fopen(argv[2], "r")) == NULL) return -1;

    if (strcmp(argv[1], "--bin") == EQUAL)
    {
      while ((n_bytes = fread(tmp, sizeof(char), 64, file))) 
      {
        for (i = 0; i < n_bytes; i++) 
        {
          if (!(i % 8)) puts("");
          ptr = Char_To_BinStr(tmp[i]);
          printf("%s ", ptr);
          free(ptr);
        }
      }
    }
    else if (strcmp(argv[1], "--hex") == EQUAL)
    {
      while ((n_bytes = fread(tmp, sizeof(char), 96, file))) 
      {
        for (i = 0; i < n_bytes; i++) 
        {
          if (!(i % 12)) puts("");
          ptr = Char_To_HexStr(tmp[i]);
          printf("%s ", ptr);
          free(ptr);
        }
      }
    }
    /*
    else if (strcmp(argv[1], "--zone6") == EQUAL)
    {
      ptr = malloc(sizeof(char) * 100);

      while ((n_bytes = fread(tmp, sizeof(char), 96, file))) 
      {
        for (i = 0; i < n_bytes; i++) 
        {
          if (offset = 0 && val != offset)
          val ^= tmp[i]; 
          if (j > 99) {
            PrintVals(ptr, j);
            for (j = 0; j < 100; j++)ptr[j] = 0;
            offset = val;
          }
          if (offset > val) {
            if (offset - val)
          }
          else {
            if (val - offset < 64) {
              ptr[j] = val;
              j++;
            }
          }
      }
    }
  }*/
  else if (argc == 2)
  {
    if ((file = fopen(argv[1], "r")) == NULL) return -1;
    while ((n_bytes = fread(tmp, sizeof(char), 100, file))) 
    {
      for (i = 0; i < n_bytes; i++) 
      {
        if (!(i % 10)) puts("");
        if (tmp[i] > 99) printf(" %d ", tmp[i]);
        else if (tmp[i] > 9) printf(" %d  ", tmp[i]);
        else if (tmp[i] < -99) printf("%d ", tmp[i]);
        else if (tmp[i] < -9) printf("%d  ", tmp[i]);
        else if (tmp[i] < 0) printf("%d   ", tmp[i]);
        else printf(" %d   ", tmp[i]);
      }
    }
  }
  puts("");

  fclose(file);

  return 0;
}

char *Char_To_HexStr(char sn)
{
  uchar n = sn + 128;
  uchar tmp = 0;
  char *byte = malloc(sizeof(char) * 3);

  tmp = n / 16;
  if (tmp > 9) byte[0] = tmp + 55;
  else byte[0] = tmp + 48;

  tmp = n % 16;
  if (tmp > 9) byte[1] = tmp + 55;
  else byte[1] = tmp + 48;

  byte[2] = 0;
  return byte;
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

