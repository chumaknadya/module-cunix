#include "linked_list.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

node_t *list_create(void *data,void *key) {
  node_t *head = malloc(sizeof(node_t));
  head->key = key;
  head->data = data;
  head->next = NULL;

  return head;
}

void list_destroy(node_t **head) {
  if(*head == NULL) return;
  node_t *ptr = *head, *pptr;

  while(ptr != NULL) {
    free(ptr->key);
    free(ptr->data);
    pptr = ptr->next;
    free(ptr);
    ptr = pptr;
  }
  (*head) = NULL;
}

void list_push(node_t *head, void *data, void *key) {
  if (head == NULL) return;
  while (head->next) {
    if (strcmp(key, (char*)head->key) == 0) {
      head->data = data;
      return;
    }
    head = head->next;
  }

  if (strcmp(key, (char*)head->key) == 0) {
    head->data = data;
    return;
  }

  head->next = malloc(sizeof(node_t));

  head = head->next;
  head->next = NULL;
  head->data = data;
  head->key  = key;
}

void list_unshift(node_t **head, void *data, void *key) {
  node_t *ptr = *head;

  *head = malloc(sizeof(node_t));

  (*head)->next =  ptr;
  (*head)->data = data;
  (*head)->key  = key;
}

void *list_pop(node_t **head) {
  node_t *ptr, *pptr = *head;
  void *rv;

  if(pptr->next) {
    ptr = pptr->next;
  }
  else {
    rv = (*head)->data;
    free((*head)->data);
    free(*head);
    *head = NULL;
    return rv;
  }
  while (ptr->next) {
    ptr = ptr->next;
    pptr = pptr->next;
  }
  rv = ptr->data;
  free(ptr->data);
  free(ptr);
  pptr->next = NULL;
  return rv;
}

void *list_shift(node_t **head) {
  if (*head == NULL) return NULL;
  void *rv;
  node_t *ptr = *head;

  (*head) = (*head)->next;

  rv = ptr->data;
  free(ptr->data);
  free(ptr);

  return rv;
}

void *list_remove(node_t **head, int pos) {
  void *rv;
  node_t *ptr = *head, *pptr;
  if (pos == 0) {
    rv = ptr->data;
    (*head) = (*head)->next;
    free(ptr->data);
    free(ptr);
    return rv;
  }

  pptr = ptr;
  ptr = ptr->next;

  while(pos > 1) {
    ptr = ptr->next;
    pptr = pptr->next;
    pos--;
  }

  pptr->next = ptr->next;
  rv = ptr->data;
  free(ptr->data);
  free(ptr);

  return rv;
}

void list_print(node_t *head) {
  while(head) {
    write(1, "\n", 1);
    write(1, head->key, strlen(head->key));
    write(1, ": ", 2);
    write(1, head->data, strlen(head->data));
    head = head->next;
  }
}

const void *find_by_key(node_t *head,char *key) {
   while(head) {
     if(strcmp(key,(char*)head->key) == 0)
       return head->data;
     head = head->next;
   }
   return NULL;
}
