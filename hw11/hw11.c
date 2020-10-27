/* Vindhya Banda, hw11.c, CS 24000, Spring 2020
 * Last updated April 16, 2020
 */

#include "hw11.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

/*
 * create_node allocates memory for a new tree_node_t
 * node and assigns the specified hash_value to it.
 */

tree_node_t *create_node(int hash_value) {
  assert(hash_value > 0);

  /* new is the newly create node to be returned */

  tree_node_t *new = malloc(sizeof(tree_node_t));
  assert(new != NULL);
  new->hash_value = hash_value;
  new->left_child_ptr = NULL;
  new->right_child_ptr = NULL;
  return new;
} /* create_node() */

/*
 * insert_node inserts the given new_node as a child to
 * the specified parent_node according to the value of
 * child_type. If child_type is 1, the new_node is set as
 * the left child of the aprent_node and right child otherwise.
 */

void insert_node(tree_node_t **parent_node, tree_node_t *new_node,
                  int child_type) {
  assert(parent_node != NULL);
  assert(new_node != NULL);
  assert((child_type == 0) || (child_type == 1));
  if (*parent_node == NULL) {
    *parent_node = new_node;
    return;
  }
  if (child_type == 0) {
    if ((*parent_node)->left_child_ptr == NULL) {
      (*parent_node)->left_child_ptr = new_node;
      return;
    }
    delete_tree((*parent_node)->left_child_ptr);
    (*parent_node)->left_child_ptr = new_node;
    return;
  }
  else {
    if ((*parent_node)->right_child_ptr == NULL) {
      (*parent_node)->right_child_ptr = new_node;
      return;
    }
    delete_tree((*parent_node)->right_child_ptr);
    (*parent_node)->right_child_ptr = new_node;
    return;
  }
} /* insert_node() */

/*
 * search_node searches the tree for a node with the given
 * hash value. If found, it returns the pointer of the node
 * and NULL otherwise.
 */

tree_node_t *search_node(tree_node_t *node, int hash_value) {
  assert(hash_value > 0);
  if (node == NULL) {
    return NULL;
  }
  if (node->hash_value == hash_value) {
    return node;
  }
  /* temp holds the return value of recursive calls to search_node */

  tree_node_t *temp = search_node(node->left_child_ptr, hash_value);
  if (temp != NULL) {
    return temp;
  }
  temp = search_node(node->right_child_ptr, hash_value);
  if (temp != NULL) {
    return temp;
  }
  return NULL;
} /* search_node() */

/*
 * delete_tree deletes the given tree and frees the
 * concerning memory.
 */

void delete_tree(tree_node_t *node) {
  if (node == NULL) {
    return;
  }
  delete_tree(node->left_child_ptr);
  delete_tree(node->right_child_ptr);
  free(node);
} /* delete_tree() */

/*
 * get_hash_list_prefix traverse the given tree
 * based on a PREFIX traversal order and stores the
 * encountered hash_values in a hash_list in an
 * appropriate order.
 */

hash_list_t *get_hash_list_prefix(tree_node_t *root_node) {
  assert(root_node != NULL);

  /* new_hash_list is the newly created hash_list_t to be returned */

  hash_list_t *new_hash_list = malloc(sizeof(hash_list_t));
  assert(new_hash_list != NULL);

  new_hash_list->hash_value = root_node->hash_value;
  new_hash_list->next_hash_ptr = NULL;

  if (root_node->left_child_ptr != NULL) {
    new_hash_list->next_hash_ptr = get_hash_list_prefix(
                                   root_node->left_child_ptr);
  }
  /* temp is used to traverse new_hash_list */

  hash_list_t *temp = new_hash_list;
  while (temp->next_hash_ptr != NULL) {
    temp = temp->next_hash_ptr;
  }

  if (root_node->right_child_ptr != NULL) {
    temp->next_hash_ptr = get_hash_list_prefix(root_node->right_child_ptr);
  }

  return new_hash_list;

} /* get_hash_list_prefix() */

/*
 * get_hash_list_postfix traverse the given tree
 * based on a POSTFIX traversal order and stores the
 * encountered hash_values in a hash_list in an
 * appropriate order.
 */

hash_list_t *get_hash_list_postfix(tree_node_t *root_node) {
  assert(root_node != NULL);

  /* new_hash_list is the newly created hash_list_t to be returned */

  hash_list_t *new_hash_list = NULL;

  if (root_node->left_child_ptr != NULL) {
    new_hash_list = get_hash_list_postfix(root_node->left_child_ptr);
  }
  /* temp_1 is used to traverse new_hash_list */

  hash_list_t *temp_1 = new_hash_list;
  if (new_hash_list != NULL) {
    while (temp_1->next_hash_ptr != NULL) {
      temp_1 = temp_1->next_hash_ptr;
    }
  }

  if (root_node->right_child_ptr != NULL) {
    if (temp_1 == NULL) {
      new_hash_list = get_hash_list_postfix(root_node->right_child_ptr);
    }
    else {
      temp_1->next_hash_ptr = get_hash_list_postfix(root_node->right_child_ptr);
    }
  }
  /* temp is used to traverse new_hash_list */

  hash_list_t *temp = new_hash_list;
  if (new_hash_list != NULL) {
    while (temp->next_hash_ptr != NULL) {
      temp = temp->next_hash_ptr;
    }
    temp->next_hash_ptr = malloc(sizeof(hash_list_t));
    assert(temp->next_hash_ptr != NULL);
    temp->next_hash_ptr->hash_value = root_node->hash_value;
    temp->next_hash_ptr->next_hash_ptr = NULL;
  }
  else{
    new_hash_list = malloc(sizeof(hash_list_t));
    assert(new_hash_list != NULL);
    new_hash_list->hash_value = root_node->hash_value;
    new_hash_list->next_hash_ptr = NULL;
  }
  return new_hash_list;
} /* get_hash_list_postfix() */

/*
 * get_hash_list_forward traverse the given tree
 * based on a FORWARD traversal order and stores the
 * encountered hash_values in a hash_list in an
 * appropriate order.
 */

hash_list_t *get_hash_list_forward(tree_node_t *root_node) {

  assert(root_node != NULL);

  /* new_hash_list is the newly created hash_list_t to be returned */

  hash_list_t *new_hash_list = NULL;

  if (root_node->left_child_ptr != NULL) {
    new_hash_list = get_hash_list_forward(root_node->left_child_ptr);
  }
  /* temp is used to traverse new_hash_list */

  hash_list_t *temp = new_hash_list;
  if (new_hash_list != NULL) {
    while (temp->next_hash_ptr != NULL) {
      temp = temp->next_hash_ptr;
    }
    temp->next_hash_ptr = malloc(sizeof(hash_list_t));
    assert(temp->next_hash_ptr != NULL);
    temp->next_hash_ptr->hash_value = root_node->hash_value;
    temp->next_hash_ptr->next_hash_ptr = NULL;
  }
  else{
    new_hash_list = malloc(sizeof(hash_list_t));
    assert(new_hash_list != NULL);
    new_hash_list->hash_value = root_node->hash_value;
    new_hash_list->next_hash_ptr = NULL;
  }
  /* temp_1 is used to traverse new_hash_list */

  hash_list_t *temp_1 = new_hash_list;
  if (new_hash_list != NULL) {
    while (temp_1->next_hash_ptr != NULL) {
      temp_1 = temp_1->next_hash_ptr;
    }
  }

  if (root_node->right_child_ptr != NULL) {
    if (temp_1 == NULL) {
      new_hash_list = get_hash_list_forward(root_node->right_child_ptr);
    }
    else {
      temp_1->next_hash_ptr = get_hash_list_forward(root_node->right_child_ptr);
    }
  }
  return new_hash_list;
} /* get_hash_list_forward() */

/*
 * get_hash_list_reverse traverse the given tree
 * based on a REVERSE traversal order and stores the
 * encountered hash_values in a hash_list in an
 * appropriate order.
 */

hash_list_t *get_hash_list_reverse(tree_node_t *root_node) {

  assert(root_node != NULL);

  /* new_hash_list is the newly created hash_list_t to be returned */

  hash_list_t *new_hash_list = NULL;

  if (root_node->right_child_ptr != NULL) {
    new_hash_list = get_hash_list_reverse(root_node->right_child_ptr);
  }
  /* temp is used to traverse new_hash_list */

  hash_list_t *temp = new_hash_list;
  if (new_hash_list != NULL) {
    while (temp->next_hash_ptr != NULL) {
      temp = temp->next_hash_ptr;
    }
    temp->next_hash_ptr = malloc(sizeof(hash_list_t));
    assert(temp->next_hash_ptr != NULL);
    temp->next_hash_ptr->hash_value = root_node->hash_value;
    temp->next_hash_ptr->next_hash_ptr = NULL;
  }
  else{
    new_hash_list = malloc(sizeof(hash_list_t));
    assert(new_hash_list != NULL);
    new_hash_list->hash_value = root_node->hash_value;
    new_hash_list->next_hash_ptr = NULL;
  }
  /* temp_1 is used to traverse new_hash_list */

  hash_list_t *temp_1 = new_hash_list;
  if (new_hash_list != NULL) {
    while (temp_1->next_hash_ptr != NULL) {
      temp_1 = temp_1->next_hash_ptr;
    }
  }

  if (root_node->left_child_ptr != NULL) {
    if (temp_1 == NULL) {
      new_hash_list = get_hash_list_reverse(root_node->left_child_ptr);
    }
    else {
      temp_1->next_hash_ptr = get_hash_list_reverse(root_node->left_child_ptr);
    }
  }
  return new_hash_list;
} /* get_hash_list_reverse() */

/*
 * delete_hash_list deletes the given hash_list_t and
 * frees the concerned memory.
 */

void delete_hash_list(hash_list_t *hash_head) {
  if (hash_head == NULL) {
    return;
  }
  delete_hash_list(hash_head->next_hash_ptr);
  free(hash_head);
} /* delete_hash_list() */
