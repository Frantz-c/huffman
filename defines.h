#ifndef FC_DEFINES_151107_H
#define FC_DEFINES_151107_H

#define int            int32_t
#define uchar          unsigned char
#define bool           char
#define true           1
#define false          0

#define COMPRESS       0
#define DECOMP         1

#define ERR           -1
#define ERR_NOT_FATAL  0
#define ERR_FATAL      1
#define MATCH          0
#define NO_MATCH       1

#define BIT1           1
#define BIT2           2
#define BIT3           4
#define BIT4           8
#define BIT5           16
#define BIT6           32
#define BIT7           64
#define BIT8           (64 << 1)

#define STR_ALLOC_N_COPY(dest, org) \
{\
  dest = calloc(1 + strlen(org), sizeof(char));\
  strcpy(dest, org);\
}

/* OpenFile(const char *filename, const char *mode, FILE *f, flag)*/
#define OPENFILE(filename, mode, file, flag) \
{\
  if ( !(file = fopen(filename, mode)) ) \
  {\
      printf("\e[1;31mErreur lors de l'ouverture de '%s' en mode \"%s\"\e[0m\n", filename, mode);\
      if (flag == ERR_FATAL) exit(EXIT_FAILURE);\
  }\
}

/* CloseFile(const char *filename, FILE *file, char flag)*/
#define CLOSEFILE(filename, file, flag) \
{\
  if ( fclose(file) == EOF ) \
  {\
    printf("\e[1;31mErreur lors de la fermeture du fichier '%s'\e[0m\n", filename);\
    if (flag == ERR_FATAL) exit(EXIT_FAILURE);\
  }\
}

#endif
