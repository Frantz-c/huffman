#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "defines.h"
#include "occlist.h" 
#include "codelist.h"
#include "huffmantree.h"
#include "compress.h"
#include "decompress.h"


static char Get_Action(int argc, char *argv[]);
static void Set_FileNames(int argc, char **org, char **dest, char *argv[], char action);


int main(int argc, char *argv[])
{
  char *org, *dest, action;

  if ((action = Get_Action(argc, argv)) == -1) return EXIT_FAILURE;
  Set_FileNames(argc, &org, &dest, argv, action);

  switch (action)
  {
    case COMPRESS: Compress(org,   dest); break;
    case DECOMP  : Decompress(org, dest);
  }

  free(org);
  free(dest);
  
  return 0;
}

void Set_FileNames(int argc, char **org, char **dest, char *argv[], char action)
{
  char *filecomp = "file.comp";
  char *filedcmp = "file.dcmp";

  if      (argc   == 4)        STR_ALLOC_N_COPY(*dest, argv[3])
  else if (action == COMPRESS) STR_ALLOC_N_COPY(*dest, filecomp)
  else                         STR_ALLOC_N_COPY(*dest, filedcmp)

  STR_ALLOC_N_COPY(*org, argv[2]);
}

char Get_Action(int argc, char *argv[])
{
  if (argc < 3 || argc > 4)
  {
    printf("%s [param] \"origin\" \"dest\"?\n"
           "param = \n-c (compress)\n-d (decompress)\n", argv[0]);
    return -1;
  }

  if      (strcmp(argv[1], "-c") == 0) return COMPRESS;
  else if (strcmp(argv[1], "-d") == 0) return DECOMP;

  fprintf(stderr, "paramètre '%s' invalide\n", argv[1]);
  return -1;
}


