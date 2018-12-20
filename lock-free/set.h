#ifndef SET_H
#define SET_H

#include <pthread.h>
typedef struct node{
  int key;
  struct node* next;
  pthread_mutex_t lock;
} node_t;

typedef struct intset{
  node_t* head;
} intset_t;


node_t *delete_item(node_t *val);
int is_deleted(node_t *val);
node_t *get_address(node_t *val);
int set_validate(intset_t* set, node_t *val);
int set_add(intset_t* set,int val);
int set_contain(intset_t* set,int val);
int set_new(intset_t* set);
int set_remove(intset_t* set, int val);
void set_print(intset_t* set);



#endif /* SET_H */
