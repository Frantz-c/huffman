#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  FILE *file = fopen("file", "w");

  char wbyte[7] = {1, 3, 5, 1, 1, 5, 1};
  fwrite(wbyte, sizeof(char), 7, file);
  fclose(file);
  return 0;
}
