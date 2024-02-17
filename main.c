#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *pickword(char *file)
{
  FILE *fp = fopen(file, "r");
  if (!fp) perror(file), exit(1);
  fseek(fp, 0, SEEK_END);
  long end = ftell(fp);
  if (RAND_MAX < end) puts("RAND_MAX too small"), exit(1);
  int c;
  do
    fseek(fp, rand()%end, SEEK_SET),
      fscanf(fp, "%*[^\n]%*c"),           // skip partial line
      c = getc(fp);
  while (c <= ' ');                      // header lines start with space
  ungetc(c, fp);
  static char word[72];
  fscanf(fp, "%71s", word);
  fclose(fp);
  return word;
}

int main()
{
  // nouns: 117827, verbs: 11558, adverbs: 4510, adjectives: 21508
  // total: 155403
  size_t weights[4] = {117827,11558,4510,21508};

  // Randomly sample for a part of speech
  srand(time(NULL));
  size_t num = (size_t) rand() % 155403;
  size_t part = 0;
  while (num > weights[part]){
      num -= weights[part];
      part++;
  }

  // Randomly sample a word from the part of speech using pickword()
  switch(part){
  case 0:
    puts(pickword("/usr/share/wordnet/index.noun"));
    break;
  case 1:
    puts(pickword("/usr/share/wordnet/index.verb"));
    break;
  case 2:
    puts(pickword("/usr/share/wordnet/index.adv"));
    break;
  case 3:
    puts(pickword("/usr/share/wordnet/index.adj"));
    break;
  default:
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
