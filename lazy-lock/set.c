#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "set.h"

int set_validate(intset_t* set, node_t *val){
  node_t* curr;
  node_t* pred;
  pred = set->head;
  curr = pred->next;

  while(pred->key <= val->key){
    if (pred == val) {
      return 1;
    }
    pred = curr;
    curr = pred->next;
  }

  return 0;
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
    // validation step
    pthread_mutex_unlock(&(pred->lock));
    pthread_mutex_unlock(&(curr->lock));

    if (set_validate(set, pred) && pred->next == curr) {
      pthread_mutex_unlock(&(pred->lock));
      pthread_mutex_unlock(&(curr->lock));
      return 1;
    }
  }
  pthread_mutex_unlock(&(pred->lock));
  pthread_mutex_unlock(&(curr->lock));
  return 0;
}

int set_add(intset_t* set, int val){
  node_t* curr;
  node_t* pred;
  int v;
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
    if (set_validate(set, pred) && pred->next == curr) {
      node_t* new = malloc(sizeof(node_t));
      new->key=val;
      pthread_mutex_init(&(new->lock),NULL);
      pred->next=new;
      new->next=curr;
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

  pthread_mutex_lock(&(curr->lock));
  pthread_mutex_lock(&(pred->lock));
  v = curr->key;
  
  if (set_validate(set, pred) && pred->next == curr) {
    if (curr->key == val){
      pred->next = curr->next;
    }
    else {
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
