#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct codelist_t
{
  uint32_t *code;
  char      byte;

  struct codelist_t *next;
}
codelist_t;

int main(int argc, char *argv[])
{
  
  return 0;
}

char *WriteBytes(char byteArray[], int16_t size, int16_t *retsize)
{

}
