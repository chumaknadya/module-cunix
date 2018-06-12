#include "binary_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
node_t  *allocnode(){
  node_t  *node = malloc(sizeof(node_t));
  node->data = NULL;
  node->key = NULL;
  node->left = NULL;
  node->right = NULL;
  return node;
}
node_t *insert(node_t *root, char *key, void *data){
  if(root == NULL) {
    root = malloc(sizeof(node_t));
    root->key = key;
    root->data = data;
    root->left = NULL;
    root->right = NULL;
    return root;
  }
  if (strcmp(root->key,key) > 0)
        root->left  = insert(root->left, key,data);
  else
        root->right = insert(root->right, key,data);
  return root;
}
void print_node(node_t *node){
  if (node == NULL)
     return;
  printf("%s:  %s ", node->data,(char*)node->key);
}
void visit_tree(node_t *node, void (*fp)(node_t *root)){
  if(node == NULL)
    return;
  fp(node);
  visit_tree(node->left,fp);
  visit_tree(node->right,fp);
}
void destroy_tree(node_t *node, void (*fdestroy)(node_t *root)){
  if(node == NULL)
    return;
  destroy_tree(node->left,fdestroy);
  destroy_tree(node->right,fdestroy);
  fdestroy(node);
  free(node);
}
