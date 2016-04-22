#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
void*  printStuff(void* args)
{
  srand((unsigned int)time(NULL));
  int i = 0;
  int j = 0;
  double result = 0.0;
  int myNum = *( (int *)args );
  while( i <= 5 )
  {
    printf("Thead #%d, iter #%d\n", myNum, i);
    ++i;
    j = rand()%100;
    while( j > 0 )
    {
      result = result + sin(j) * tan(j);
      --j;
    }
  }
  pthread_exit(0);
}
int main( int argc, char* argv[])
{
  //create some arguments to pass to the threads
  // ... note they are malloced because they must be on the heap,
  //     so that they are visible to the threads
  int * threadArgs0 = (int*)malloc(sizeof(int));
  int * threadArgs1 = (int*)malloc(sizeof(int));
  *threadArgs0 = 0;
  *threadArgs1 = 1;

  // build thread status variables for pthread_exit to use later
  void* threadStatus0;
  void* threadStatus1;

  // build thread handles for pthread_create
  pthread_t thread0;
  pthread_t thread1;
  pthread_t* threadHandle0 = &thread0;
  pthread_t* threadHandle1 = &thread1;

  // build blank pthread attribute structs and initialize them
  pthread_attr_t threadAttr0;
  pthread_attr_t threadAttr1;
  pthread_attr_init(&threadAttr0);
  pthread_attr_init(&threadAttr1);

  // set the initialized attribute struct so that the pthreads created will be joinable
  pthread_attr_setdetachstate(&threadAttr0, PTHREAD_CREATE_JOINABLE);
  pthread_attr_setdetachstate(&threadAttr1, PTHREAD_CREATE_JOINABLE);

  // set the initialized attribute struct so that the pthreads created will be kernel threads
  pthread_attr_setscope(&threadAttr0, PTHREAD_SCOPE_SYSTEM);
  pthread_attr_setscope(&threadAttr1, PTHREAD_SCOPE_SYSTEM);
/*
  pthread_attr_setdetachstate(&threadAttr0, PTHREAD_CREATE_DETACHED);
  pthread_attr_setdetachstate(&threadAttr1, PTHREAD_CREATE_DETACHED);
*/
  // build the pthreads
  pthread_create(&thread0, &threadAttr0, printStuff, (void*)threadArgs0);
  pthread_create(&thread1, &threadAttr1, printStuff, (void*)threadArgs1);
  // destroy the pthread attribute structs, we're done creating the threads,
  //   we don't need them anymore
  pthread_attr_destroy(&threadAttr0);
  pthread_attr_destroy(&threadAttr1);

  // wait for the threads to finish .. make the threadStatus variables point to
  //    the value of pthread_exit() called in each
  pthread_join( thread0, &threadStatus0);
  pthread_join( thread1, &threadStatus1);
  return 0;
}
