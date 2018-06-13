#include "../include/linked_list.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

node_t  *list_create(void *data) {
   node_t * head = NULL;
   head = malloc(sizeof(node_t));
   head->data = data;
   head->next = NULL;
   return head;
}

void list_destroy(node_t **head, void (*fp)(void *data)) {
    if (head == NULL) {
      return;
    }
    node_t *curr = *head;
    node_t *next;

    while (curr != NULL) {
         fp(curr->data);
         next = curr->next;
         free(curr);
         curr = next;
    }
    *head = NULL;
}

void list_push(node_t *head, void *data) {
  if (head == NULL)
    return;
  while(head->next) head = head->next;
  head->next = malloc(sizeof(node_t));
  head = head->next;
  head->next = NULL;
  head->data = data;
}

void list_unshift(node_t **head, void *data) {
     node_t   *new_node = malloc(sizeof(node_t));
     new_node->data = data;
     new_node->next = *head;
     *head = new_node;
}

void *list_pop(node_t **head){
    node_t *ptr, *pptr = *head;
    void *res;
    if(pptr->next) {
      ptr = pptr->next;
    } else {
      res = (*head)->data;
      free((*head)->data);
      free(*head);
      *head = NULL;
      return res;
    }
    while (ptr->next) {
      ptr = ptr->next;
      pptr = pptr->next;
    }
    res = ptr->data;
    free(ptr->data);
    free(ptr);
    pptr->next = NULL;
    return res;
}

void *list_shift(node_t **head) {
   void *res;
   node_t *temp = *head;
   *head = (*head)->next;
   res = temp->data;
   free(temp->data);
   free(temp);
   return res;
}

void *list_remove(node_t **head, int pos) {
  void *res;
  node_t *ptr = *head, *pptr;
  if (pos == 0) {
      res = ptr->data;
      (*head) = (*head)->next;
      free(ptr->data);
      free(ptr);
      return res;
  }
  pptr = ptr;
  ptr = ptr->next;
  while(pos > 1) {
      ptr = ptr->next;
      pptr = pptr->next;
      pos--;
   }
  pptr->next = ptr->next;
  res = ptr->data;
  free(ptr->data);
  free(ptr);
  return res;
}

void list_print(node_t *head) {
    node_t * current = head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

void list_visitor(node_t *head, void (*fp)(void *data)) {
    while (head != NULL) {
        (*fp)(head->data);
        head = head->next;
    }
}

