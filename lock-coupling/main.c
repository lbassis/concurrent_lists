#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "set.h"
#define N_THREAD 4   /* number of threads */
#define N_INIT 50     /* number of initial values in the set */
#define N_MAX 50     /* inserted integer are between 0 and N_MAX */ 
#define N_OP 10     /* number of operations per threads */
#define F_INSERT .1     /* fraction of insert operation  */
#define F_REMOVE .1     /* fraction of rmv operations    */
#define F_SEARCH .8     /* fraction of search operations */

typedef struct thread_arg{
  intset_t* set_ptr;
  int tid;
} thread_arg_t;


void* threadfunc(void* param){
  thread_arg_t* arg = (thread_arg_t*) param;
  intset_t* set_ptr = arg->set_ptr;
  int id            = arg->tid;

  srand(id);
  unsigned int seed = id;
  
  for(int i=0;i<N_OP;i++){
    double r = (double)rand() / (double)RAND_MAX ;
    int val = rand_r(&seed) % N_MAX;
    if (r < F_INSERT)
      fprintf(stderr,"%i\t inserted by thread %i: %i\n", val, id, set_add(set_ptr, val));
    else if(r< F_INSERT + F_REMOVE)
      fprintf(stderr,"%i\t removed by thread %i: %i\n", val, id, set_remove(set_ptr, val));
    else
      fprintf(stderr,"%i\t searched by thread %i: %i\n", val, id, set_contain(set_ptr, val));
  }
  pthread_exit(NULL);
}


int main(int argc,char* argv[]){
  intset_t set;
  pthread_t t_id[N_THREAD];
  thread_arg_t t_arg[N_THREAD];
  set_new(&set);
  unsigned int seed = time(NULL);
  srand(seed);
  /* populate the set */
  fprintf(stderr,"populating ... \n");
  for(int i=0; i < N_INIT; i++){
    //int r = rand_r(&seed) % N_MAX;
    int r = i;
    set_add(&set,r);
  }
  fprintf(stderr,"creating ... \n");
  /* thread creation */  
  for(int i=0; i<N_THREAD; i++){
    t_arg[i].tid = i;
    t_arg[i].set_ptr = &set;
    pthread_create(&(t_id[i]),NULL,threadfunc,(void*) &(t_arg[i]));
  }
  fprintf(stderr,"waiting ... \n");
  for(int i=0; i<N_THREAD; i++)
    pthread_join(t_id[i],NULL);

  pthread_exit(NULL);
}
