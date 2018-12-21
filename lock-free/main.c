#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


#include "set.h"
#define N_INIT 50     /* number of initial values in the set */
#define N_MAX 100     /* inserted integer are between 0 and N_MAX */ 
#define N_OP 1000     /* number of operations per threads */
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
      set_add(set_ptr, val);
    else if(r< F_INSERT + F_REMOVE)
      set_remove(set_ptr, val);
    else
      set_contain(set_ptr, val);
  }
  pthread_exit(NULL);
}

int main(int argc,char* argv[]){

  unsigned int seed = (unsigned int)atoi(argv[1]);
  unsigned int execution = seed;
  int n_threads = atoi(argv[2]);  
  intset_t set;
  pthread_t t_id[n_threads];
  thread_arg_t t_arg[n_threads];


  struct timeval  tv1, tv2;
  gettimeofday(&tv1, NULL);


 
  set_new(&set);
  srand(seed);
  /* populate the set */
  fprintf(stderr,"populating ... \n");
  for(int i=0; i < N_INIT; i++){
    int r = rand_r(&seed) % N_MAX;
    set_add(&set,r);
  }
  fprintf(stderr,"creating ... \n");
  /* thread creation */
  for(int i=0; i<n_threads; i++){
    t_arg[i].tid = i;
    t_arg[i].set_ptr = &set;
    pthread_create(&(t_id[i]),NULL,threadfunc,(void*) &(t_arg[i]));
  }
  fprintf(stderr,"waiting ... \n");
  for(int i=0; i<n_threads; i++)
    pthread_join(t_id[i],NULL);


  
  gettimeofday(&tv2, NULL);


  FILE *data = fopen("../data.txt", "a");
  fprintf (data, "LockFree, %d, %d, %f\n", execution, n_threads,
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));
  fclose(data);
  pthread_exit(NULL);

}
