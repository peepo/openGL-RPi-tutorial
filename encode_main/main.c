#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bcm_host.h"
#include "encode.h"

#define NUMFRAMES 60

   int framecount = 0;

int main(int argc, char **argv)
{
   if (argc < 2) {
      printf("Usage: %s <filename>\n", argv[0]);
      exit(1);
   }
   bcm_host_init();
   openEncode(argv[1]);
   do {
      framecount++;
      encodeFrame();
   }
   while (framecount < NUMFRAMES);
   closeEncode();
}

