#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"


void random_string(char * string, unsigned length)
{
  /* Seed number for rand() */
  srand((unsigned int) time(0) + getpid());
   
  /* ASCII characters 48-57(digit) and 97-122(a-z)*/
  int i;  
  for (i = 0; i < length; ++i)
    {
      if(i<2){
          string[i] = rand()%(57-48+1) + 48;
      }else{
         string[i] = rand() % (122-97+1) + 97;
      }
    }
 
  string[i] = '\0';  
}

