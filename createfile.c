#include <stdio.h>
#include <stdlib.h>

#define TAB_SIZE 100

void Init_tab(char tab[]);
void WriteTab(FILE *file, char tab[]);

int main(int argc, char *argv[])
{
  char c[TAB_SIZE] = {0};
  FILE *file = fopen("comptest", "w");

  Init_tab(c);
  WriteTab(file, c);

  fclose(file);

  return 0;
}

void Init_tab(char tab[])
{
  int i;

  for (i = 0; i < TAB_SIZE; i++)
  {
    if (i < 2) {
      tab[i] = -128;
    }
    else if (i < 7) {
      tab[i] = -99;
    }
    else if (i < 13) {
      tab[i] = -53;
    }
    else if (i < 20) {
      tab[i] = -22;
    }
    else if (i < 29) {
      tab[i] = 0;
    }
    else if (i < 40) {
      tab[i] = 43;
    }
    else if (i < 54) {
      tab[i] = 86;
    }
    else if (i < 72) {
      tab[i] = 101;
    }
    else if (i < 92) {
      tab[i] = 113;
    }
    else {
      tab[i] = 127;
    }
  }
}

void WriteTab(FILE *file, char tab[])
{
  if (!file) return;

  printf("fwrite = %ld\n", fwrite(tab, sizeof(char), TAB_SIZE, file));
}
