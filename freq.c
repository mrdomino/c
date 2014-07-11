#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void
usage(const char* argv0)
{
  printf("usage: %s\n"
         "freq produces a report on the frequency of ASCII characters read "
         "from stdin.\n", argv0);
}

int
main(int argc, char* argv[])
{
  int c;
  uint64_t cs[256];
  uint32_t i;

  if (argc > 1) {
    usage(argv[0]);
    exit(1);
  }
  else {
    for (i = 0; i < 256; i++) {
      cs[i] = 0;
    }
    while ((c = getchar()) != EOF) {
      cs[(unsigned char)c]++;
    }
    for (i = 0; i < 256; i++) {
      if (i < 127 && i > 31) {
        printf("'%c'\t0x%x\t%lu\n", (unsigned char)i, i, cs[i]);
      }
      else printf(" - \t0x%02x\t%lu\n", i, cs[i]);
    }
  }
  return 0;
}
