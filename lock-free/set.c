#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <limits.h>
#include <stdatomic.h>

#include "set.h"

node_t *delete_item(node_t *val) {
  long p = (long)val;
  p = p|0b1;

  return (node_t*)p;
}

int is_deleted(node_t *val) {
  long p = (long)val;
  p = p&0b1;

  return p;
}

node_t *get_address(node_t *val) {
  long p = (long)val;
  p = p&(LONG_MAX-1);

  return (node_t*)p;

}

int set_new(intset_t* set){
  node_t* head = malloc(sizeof(node_t));
  node_t* tail = malloc(sizeof(node_t));
  head->key = INT_MIN;
  head->next = tail;
  tail->key = INT_MAX;
  tail->next = NULL;
  set ->head = head;
  fprintf(stderr,"%d\n",set->head->key);
  fprintf(stderr,"%d\n",set->head->next->key);
  return 1; 
}

int set_contain(intset_t* set, int val){
  node_t* curr;
  node_t* pred;
  int v;
  pred = set->head;
  curr = pred->next;

  while(curr->key < val){
    pred = curr;
    curr = pred->next;
  }
  v = curr->key;
  
  if (v == val) {  
    if (!curr->deleted && !pred->deleted && pred->next == curr) {
      return 1;
    }
  }
  return 0;
}

int set_add(intset_t* set, int val){
  node_t* curr;
  node_t* pred;
  int v;
  int cas = 1;
  pred = set->head;
  curr = pred->next;
  
  while(curr->key < val){
    pred = curr;
    curr = pred->next;
  }
  
  pthread_mutex_lock(&(curr->lock));
  pthread_mutex_lock(&(pred->lock));
  v = curr->key;

  if (curr->key != val){
    if (!curr->deleted && !pred->deleted && pred->next == curr) {
      node_t* new = malloc(sizeof(node_t));
      new->key=val;

      do {
	//pred->next=new;
      new->next=curr;
      //atomic_compare_exchange_weak(pred->next, new->next, new);
      new->deleted = 0;
      } while (cas != 1);
      }

    else { // if it wasnt valid, the insertion wasnt valid
      v = -1;
    }
  }
  /* unlock the list */
  pthread_mutex_unlock(&(pred->lock));
  pthread_mutex_unlock(&(curr->lock));

  if (v == val)
    return 1;
  else return 0;
}


int set_remove(intset_t* set, int val){
  node_t* curr;
  node_t* pred;
  int v;

  pred = set->head;
  curr = pred->next;
  
  while(curr->key < val){
    pred = curr;
    curr = pred->next;
   }

  v = curr->key;

  
  if (curr->key == val){
    curr->deleted = 1;
    pred->next = curr->next;
  }
  else {
    v = -1;
  }

  /* unlock the list */
  pthread_mutex_unlock(&(pred->lock));
  pthread_mutex_unlock(&(curr->lock));

  if (v == val)
    return 1;
  else return 0;
}

/* not thread safe */ 
void set_print(intset_t* set){
  node_t* curr;
  node_t* pred;
  pred = set->head;
  curr = pred->next;
  while(curr->key < INT_MAX){
    printf("%d ",curr->key);
    pred = curr;
    curr = pred->next;
  }
  printf("\n");
}
