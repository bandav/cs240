/* Vindhya, hw12.c, CS 24000, Spring 2020
 * Last updated April 22, 2020
 */

#include "hw12.h"

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>

/*
 * create_cpu_data allocates memory for a new cpu_t
 * struct and assigns data to it.
 */

void create_cpu_data(void **new_node, const char *model,
                        const char *manfct, int speed, int cores){
  assert(new_node != NULL);
  assert(model != NULL);
  assert(manfct != NULL);
  assert(*new_node == NULL);

  /* new is the cpu_t struct to which new_node will point to */

  cpu_t *new = malloc(sizeof(cpu_t));
  assert(new != NULL);

  new->model = malloc(sizeof(char) * strlen(model) + 1);
  assert((new)->model != NULL);
  strcpy((new)->model, model);

  (new)->manufacturer = malloc(sizeof(char) * strlen(manfct) + 1);
  assert((new)->manufacturer != NULL);
  strcpy((new)->manufacturer, manfct);

  (new)->speed = speed;
  (new)->cores = cores;

  *new_node = (void *) new;
} /* create_cpu_data() */

/*
 * delete_cpu_data deletes the provided struct as if
 * it were a cpu_t struct.
 */

void delete_cpu_data(void **node) {
  assert(node != NULL);
  assert(*node != NULL);

  /* new_node is the node to be deleted */

  cpu_t *new_node = (cpu_t *)(*node);
  free(new_node->model);
  new_node->model = NULL;
  free(new_node->manufacturer);
  new_node->manufacturer = NULL;
  free(new_node);
  new_node = NULL;
  *node = NULL;
} /* delete_cpu_data() */

/*
 * compare_cpu_data compares two structs and returns an
 * appropriate integer value.
 */

int compare_cpu_data(void *data_1, void *data_2) {
  assert((data_1 != NULL) && (data_2 != NULL));

  /* arg_1 is one the cpu_t structs being compared */

  cpu_t *arg_1 = (cpu_t *) data_1;

  /* arg_2 is one the cpu_t structs being compared */

  cpu_t *arg_2 = (cpu_t *) data_2;

  /* product_1 is the product of arg_1's speed and cores */

  int product_1 = arg_1->speed * arg_1->cores;

  /* product_2 is the product of arg_2's speed and cores */

  int product_2 = arg_2->speed * arg_2->cores;

  if (product_1 > product_2) {
    return 1;
  }
  else if (product_1 < product_2) {
    return -1;
  }
  return 0;
} /* compare_cpu_data() */

/*
 * create_memory_data allocates memory for a new memory_t
 * struct and assigns data to it.
 */

void create_memory_data(void **new_node, const char *model,
                         const char *manfct, int size, int speed, int ddr_gen) {
  assert(new_node != NULL);
  assert(model != NULL);
  assert(manfct != NULL);
  assert(*new_node == NULL);

  /* new is the memory_t struct to which new_node will point to */

  memory_t *new = malloc(sizeof(memory_t));
  assert(new != NULL);

  new->model = malloc(sizeof(char) * strlen(model) + 1);
  assert(new->model != NULL);
  strcpy(new->model, model);

  new->manufacturer = malloc(sizeof(char) * strlen(manfct) + 1);
  assert(new->manufacturer != NULL);
  strcpy(new->manufacturer, manfct);

  new->size = size;
  new->speed = speed;
  new->ddr_gen = ddr_gen;

  *new_node = (void *) new;

} /* create_memory_data() */

/*
 * delete_memory_data deletes the provided struct as if
 * it were a memory_t struct.
 */

void delete_memory_data(void **node) {
  assert(node != NULL);
  assert(*node != NULL);

  /* new_node is the node to be deleted */

  memory_t *new_node = (memory_t *)(*node);
  free(new_node->model);
  new_node->model = NULL;
  free(new_node->manufacturer);
  new_node->manufacturer = NULL;
  free(new_node);
  new_node = NULL;
  *node = NULL;
} /* delete_memory_data() */

/*
 * compare_memory_data compares two structs and returns an
 * appropriate integer value.
 */

int  compare_memory_data(void *data_1, void *data_2) {
  assert((data_1 != NULL) && (data_2 != NULL));

  /*arg_1 is one the memory_t structs being compared */

  memory_t *arg_1 = (memory_t *) data_1;

  /*arg_2 is one the memory_t structs being compared */

  memory_t *arg_2 = (memory_t *) data_2;

  if (strcmp(arg_1->model, arg_2->model) > 0) {
    return 1;
  }
  else if (strcmp(arg_1->model, arg_2->model) < 0) {
    return -1;
  }
  else {
    if (strcmp(arg_1->manufacturer, arg_2->manufacturer) > 0) {
      return 1;
    }
    else if (strcmp(arg_1->manufacturer, arg_2->manufacturer) < 0) {
      return -1;
    }
    else {
      return 0;
    }
  }
} /* compare_memory_data() */

/*
 * create_node allocates memory for a new struct node
 * and assigns data to it.
 */

void create_node(struct node **new_node, void *data, void (*print)(void *),
                 void (*delete)(void **), int (*compare)(void *, void *)) {
  assert(new_node != NULL);
  assert(*new_node == NULL);
  assert(data != NULL);
  assert((print != NULL) && (delete != NULL) && (compare != NULL));

  *new_node = (struct node *) malloc(sizeof(struct node));
  assert(*new_node != NULL);
  (*new_node)->data = data;
  (*new_node)->print = print;
  (*new_node)->delete = delete;
  (*new_node)->compare = compare;
} /* create_node() */

/*
 * delete_node deletes the provided struct node.
 */

void delete_node(struct node **new_node) {

  assert(new_node != NULL);
  assert(*new_node != NULL);

  assert(((*new_node)->left == NULL) && ((*new_node)->right == NULL));

  (*new_node)->delete(&(*new_node)->data);
  free(*new_node);
  *new_node = NULL;
} /* delete_node() */

/*
 * insert_node inserts the specified new_node into the given tree
 * at an appropriate position.
 */

void insert_node(struct node **root, struct node *new_node) {

  assert(root != NULL);
  assert(new_node != NULL);

  if (*root == NULL) {
    (*root) = new_node;
    return;
  }

  if ((*root)->compare(new_node->data, (*root)->data) == 1) {

    if ((*root)->right == NULL) {
      (*root)->right = new_node;
      return;
    }
    else {
      insert_node(&(*root)->right, new_node);
    }

  }
  else {

    if ((*root)->left == NULL) {
      (*root)->left = new_node;
      return;
    }
    else {
      if ((*root)->left == NULL) {
        (*root)->left = new_node;
        return;
      }
      else {
        insert_node(&(*root)->left, new_node);
      }
    }

  }

} /* insert_node() */

/*
 * find_nodes searches the given tree for duplicates and returns
 * an array of them (if found) or NULL (if none are found).
 */

struct node **find_nodes(struct node *root, void *data, int *rtn_value) {
  assert(root != NULL);
  assert(data != NULL);
  assert(rtn_value != NULL);

  /* temp is used to traverse the tree */

  struct node *temp = root;
  *rtn_value = 0;

  while (temp != NULL) {
    if (temp->compare(data, temp->data) == -1) {
      temp = temp->left;
    }
    else if (temp->compare(data, temp->data) == 1) {
      temp = temp->right;
    }
    else {
      temp = temp->left;
      (*rtn_value)++;
    }
  }
  if (*rtn_value != 0) {
    struct node **duplicate_arr = malloc((*rtn_value) * sizeof(struct node*));
    assert(duplicate_arr != NULL);
    temp = root;

    /* dup_count is the number of duplicate nodes present */

    int dup_count = 0;
    while (temp != NULL) {
      if (temp->compare(data, temp->data) == -1) {
        temp = temp->left;
      }
      else if (temp->compare(data, temp->data) == 1) {
        temp = temp->right;
      }
      else {
        duplicate_arr[dup_count] = temp;
        dup_count++;
        temp = temp->left;
      }
    }
    if (duplicate_arr[0] == NULL) {
      return NULL;
    }
    else {
      return duplicate_arr;
    }
  }
  else {
    return NULL;
  }
} /* find_nodes() */

/*
 * remove_node removes the given node from the specified tree.
 */

void remove_node(struct node **root, struct node *rmv_node) {
  assert(root != NULL);
  assert(rmv_node != NULL);
  if (*root == NULL) {
    return;
  }
} /* remove_node() */

/*
 * delete_tree deletes the given tree and it's child nodes
 */

void delete_tree(struct node **root) {
  assert(root != NULL);

  if (*root == NULL) {
    return;
  }
  else {
    delete_tree(&(*root)->left);
    delete_tree(&(*root)->right);
    delete_node(root);
  }

} /* delete_tree() */
