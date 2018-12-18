#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "set.h"



int set_new(intset_t* set){
  node_t* head = malloc(sizeof(node_t));
  node_t* tail = malloc(sizeof(node_t));
  head->key = INT_MIN;
  head->next = tail;
  tail->key = INT_MAX;
  tail->next = NULL;
  set ->head = head;
  pthread_mutex_init(&(set->lock),NULL);
  //  set ->lock = PTHREAD_MUTEX_INITIALIZER;
  fprintf(stderr,"%d\n",set->head->key);
  fprintf(stderr,"%d\n",set->head->next->key);
  return 1; 
}

int set_contain(intset_t* set, int val){
  node_t* curr;
  node_t* pred;
  int v;
  /* lock the list */
  pthread_mutex_lock(&(set->lock));
  pred = set->head;
  curr = pred->next;
  while(curr->key < val){
    pred = curr;
    curr = pred->next;
  }
  v = curr->key;
  /* unlock the list */
  pthread_mutex_unlock(&(set->lock));
  if (v == val)
    return 1;
  else return 0;
}

int set_add(intset_t* set, int val){
  node_t* curr;
  node_t* pred;
  int v;
  /* lock the list */
  pthread_mutex_lock(&(set->lock));
  pred = set->head;
  curr = pred->next;
  while(curr->key < val){
    pred = curr;
    curr = pred->next;
  }
  v = curr->key ;
  if (curr->key != val){
    node_t* new = malloc(sizeof(node_t));
    new->key=val;
    pred->next=new;
    new->next=curr;
  }
  /* unlock the list */
  pthread_mutex_unlock(&(set->lock));
  if (v == val)
    return 1;
  else return 0;
}


int set_remove(intset_t* set, int val){
  node_t* curr;
  node_t* pred;
  int v;
  /* lock the list */
  pthread_mutex_lock(&(set->lock));
  pred = set->head;
  curr = pred->next;
  while(curr->key < val){
    pred = curr;
    curr = pred->next;
  }
  v = curr->key;
  if (curr->key == val){
    pred->next = curr->next;
  }
  /* unlock the list */
  pthread_mutex_unlock(&(set->lock));
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
