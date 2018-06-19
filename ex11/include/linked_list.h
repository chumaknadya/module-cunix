#ifndef   LINKED_LIST_H_
# define  LINKED_LIST_H_

typedef struct  node {
    void        *data;
    void        *key;
    struct node *next;
}              node_t;

node_t  *list_create(void *data, void *key);

void    list_destroy(node_t **head);

void    list_push(node_t *head, void *data, void *key);
void    list_unshift(node_t **head, void *data, void *key);

void    *list_pop(node_t **head);
void    *list_shift(node_t **head);
void    *list_remove(node_t **head, int pos);

void    list_print(node_t *head);
const void *find_by_key(node_t *head,char *key);

#endif /* LINKED_LIST_H_ */
