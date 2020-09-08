#include "randcpuid.h"
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool
writebytes (unsigned long long x, int nbytes)
{ 
  int ndigits = nbytes * 2;
  do
    { 
      if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
        return false;
      x >>= 4;
      ndigits--;
    }
  while (0 < ndigits);

  return 0 <= putchar ('\n');
}

int
main (int argc, char **argv)
{
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
        perror (argv[1]);
      else
        valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }
  if (nbytes == 0)
    return 0;

//  void (*initialize) (void);
  unsigned long long (*rand64) (void);
//  void (*finalize) (void);
  void *sort; 
  if (rdrand_supported ())
    { 
      sort = dlopen("./randlibhw.so", RTLD_LAZY); 
      if(!sort){
	fprintf(stderr, "Error: Library failed to open"); 
	exit(1); 
	}
    
      //initialize = hardware_rand64_init;
      rand64 = dlsym(sort, "rand64"); 
      if(dlerror() != NULL){
	fprintf(stderr, "Error: dlerror"); 
	exit(1);
	}

      //finalize = hardware_rand64_fini;
    }
  else
    {
     // initialize = software_rand64_init;
      sort = dlopen("./randlib.so", RTLD_LAZY);
      if(!sort){
	fprintf(stderr, "Error: Library failed to open");
	exit(1);
	}
      rand64 = dlsym(sort, "rand64");
     // finalize = software_rand64_fini;
      if(dlerror() != NULL) {
		fprintf(stderr, "Error: dlerror"); 
		exit(1); 
	}
    }

  //initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
        {
          output_errno = errno;
          break;
        }
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    //  finalize ();
      return 1;
    }
 // finalize ();
   if(dlclose(sort)){
	fprintf(stderr, "Error: Closing"); 
	exit(1); 
	}
   return 0;
}
