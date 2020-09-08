#include "randlib.h"
#include <stdio.h>
#include <stdlib.h>

static FILE *urandstream;

static void __attribute__((constructor))
software_rand64_init (void) 
{
  urandstream = fopen ("/dev/urandom", "r");
  if (! urandstream)
    abort ();
}

extern unsigned long long rand64 (void) 
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

static void __attribute__((destructor))
software_rand64_fini (void)
{
  fclose (urandstream);
}
