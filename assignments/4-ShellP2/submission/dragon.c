#include <stdio.h>

// EXTRA CREDIT - print the drexel dragon from the readme.md
extern void print_dragon(){
  // TODO implement
  FILE *file = fopen("dragon.txt", "r");
  if (file == NULL) {
      printf("Could not open file\n");
  }

  char buffer[2056];
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
      printf("%s", buffer);
  }
  printf("\n");

  fclose(file);
}
