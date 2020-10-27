/* Vindhya Banda, hw8.c, CS 24000, Spring 2020
 * Last updated March 23, 2020
 */

#include "hw8.h"

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>

/*
 * add_new_operation takes in text and a number as input
 * and stores these values in their appropriate fields in a
 * new operation_t struct that is appended at the end of the
 * list of structs.
 */

operation_t *add_new_operation(operation_t *operation_list,
                               char *input_text, int new_line_num) {
  assert(input_text != NULL);
  assert(new_line_num >= 0);

  /* newNode is the node to be appended at the end */

  operation_t *new_node = NULL;
  new_node = malloc(sizeof(operation_t));
  assert(new_node != NULL);
  new_node->new_text = malloc(strlen(input_text) * sizeof(char) + 1);
  assert(new_node->new_text != NULL);

  new_node->line_num = new_line_num;
  strcpy(new_node->new_text, input_text);
  new_node->next_operation = NULL;

  if (operation_list == NULL) {
    return new_node;
  }
  else {
    while (operation_list->next_operation != NULL) {
      operation_list = operation_list->next_operation;
    }
    operation_list->next_operation = new_node;
  }
  return new_node;
} /* add_new_operation() */

/*
 * list_len returns the number of operation_t
 * structs in the given list.
 */

int list_len(operation_t *head) {
  if (head == NULL) {
    return 0;
  }
  /* counter keeps track of the number of */
  /* operation_t structs in the list      */

  int counter = 1;

  /* temp is employed as a temporary node */
  /* used while traversing the list       */

  operation_t *temp = head;
  while (temp->next_operation != NULL) {
    counter++;
    temp = temp->next_operation;
  }
  return counter;
} /* list_len() */

/*
 * get_nth_operation returns a pointer to the
 * operation in the list at the specified
 * position.
 */

operation_t *get_nth_operation(operation_t *head, int position) {
  assert(head != NULL);
  assert(position >= 0);

  /* temp is an operation that references head */

  operation_t *temp = head;
  if (position == 0) {
    return head;
  }
  if (position > list_len(head)) {
    return NULL;
  }
  /* counter counts the number of operations that */
  /* have been traversed                          */

  int counter = 0;
  while (temp->next_operation != NULL) {
    counter++;
    temp = temp->next_operation;
    if (counter == position) {
      return temp;
    }
  }
  return temp;
} /* get_nth_operation() */

/*
 * undo_nth_operations returns the list in reverse
 * order from the specified position. The preceeding
 * elements are sorted into a seperate list.
 */

operation_t *undo_nth_operation(operation_t *head,
                                int position) {
  assert(head != NULL);
  assert(position >= 0);
  if (list_len(head) < position) {
    return NULL;
  }
  if (position == 0) {
    /* current_0 is a placeholder for the head postion in the */
    /* resultant list                                         */

    operation_t *current_0 = head;

    /* prev_0 is a placeholder for the previous position in the */
    /* resultant list                                           */

    operation_t *prev_0 = NULL;

    /* next_0 is a placeholder for the next position in the */
    /* resultant list                                       */

    operation_t *next_0 = NULL;
    while (current_0 != NULL) {
      next_0 = current_0->next_operation;
      current_0->next_operation = prev_0;
      prev_0 = current_0;
      current_0 = next_0;
    }
    head = prev_0;
    return head;
  }
  /* new_head is the head of the resultant list */

  operation_t *new_head = get_nth_operation(head, position);
  get_nth_operation(head, position - 1)->next_operation = NULL;

  /* current is a placeholder for the head postion in the resultant list */

  operation_t *current = new_head;

  /* prev is a placeholder for the previous postion in the resultant list */

  operation_t *prev = NULL;

  /* next is a placeholder for the next postion in the resultant list */

  operation_t *next = NULL;
  while (current != NULL) {
    next = current->next_operation;
    current->next_operation = prev;
    prev = current;
    current = next;
  }
  new_head = prev;
  return new_head;
} /* undo_nth_operation() */

/*
 * red_n_operations appends the specified number
 * of operations from list 2 to list 1 in
 * reverse order.
 */

void redo_n_operations(operation_t *list1,
                       operation_t *list2,
                       int num) {
  assert(list1 != NULL);
  assert(list2 != NULL);
  assert(num >= 0);
  assert(num <= list_len(list2));

  /* len is the position from which the subsequent elements of */
  /* list 2 should be appended to list 1 in the reverse order  */

  int len = list_len(list2) - num;

  if (num != 0) {
    /* to_append is the head of the list to be appended to list 1 */

    operation_t *to_append = undo_nth_operation(list2, len);
    while (list1->next_operation != NULL) {
      list1 = list1->next_operation;
    }
    list1->next_operation = to_append;
  }
} /* redo_n_operations() */

/*
 * free_list frees all the operations in the
 * specified list.
 */

void free_list(operation_t *head) {
  if (head == NULL) {
    return;
  }
  operation_t *temp = 0;
  while (head != NULL) {
    temp = head;
    head = head->next_operation;
    free(temp->new_text);
    temp->new_text = NULL;
    free(temp);
    temp->next_operation = NULL;
  }
  head = NULL;
} /* free_list() */

/*
 * doc_last_line iterates through the list to find the
 * operation with the largest line_num and returns a
 * pointer to that operation.
 */

operation_t *doc_last_line(operation_t *head) {
  assert(head != NULL);

  /* temp is a temporary pointer used to iterate through the list */

  operation_t *temp = head;

  /* max stores the value of the maximum line number in the list  */

  int max = head->line_num;
  while (head->next_operation != NULL) {
    head = head->next_operation;
    if ((head->line_num) > max) {
      max = head->line_num;
      temp = head;
    }
  }
  return temp;
} /* doc_last_line() */

/*
 * interleave_operations interleaves the elements of
 * the first list and the second list and returns the
 * resultant list.
 */

operation_t *interleave_operations(operation_t *list1, operation_t *list2) {
  assert(list1 != NULL);
  assert(list2 != NULL);

  /* temp1 is used to access list1 */

  operation_t *temp1 = list1;

  /* temp2 is used to access list2 */

  operation_t *temp2 = list2;

  /* next_list1 is used to store the address of the next node of list1 */

  operation_t *next_list1 = NULL;

 /* next_list2 is used to store the address of the next node of list2 */

  operation_t *next_list2 = NULL;

  while ((temp1 != NULL) && (temp2 != NULL)) {
    next_list1 = temp1->next_operation;
    temp1->next_operation = temp2;
    if (next_list1 != NULL) {
      next_list2 = temp2->next_operation;
      temp2->next_operation = next_list1;
    }
    temp1 = next_list1;
    temp2 = next_list2;
  }
  return list1;
} /* interleave_operations() */

/*
 * longest_line is a helper method used to find the
 * length of the longest line (text value) in a
 * list of operations.
 */

int longest_line(operation_t *head) {
  /* mac holds the value of the largest line length value */

  int max = 0;
  while (head != NULL) {
    if (strlen(head->new_text) > max) {
      max = strlen(head->new_text);
    }
    head = head->next_operation;
  }
  return max;
} /* longest_line() */

/*
 * write_document writes the text stored in the operations
 * as indicated by the specified head into the given file.
 */

int write_document(char* file, operation_t *head) {

  assert(file != NULL);
  assert(head != NULL);

  /* out_file is the file to be written into */

  FILE *out_file = fopen(file, "w");
  if (out_file == NULL) {
    return NON_WRITABLE_FILE;
  }
  /* last_line is used to find the largest line number value in the list */

  operation_t *last_line = doc_last_line(head);

  /* largest_line_no holds the largest line number value in the list */

  int largest_line_no = last_line->line_num;

  /* buffer_len is the length of the buffer declared subsequently */

  int buffer_len = longest_line(head) + 1;

  /* buffer is stores the new_text value of the operation each iteration */

  char buffer[buffer_len];

  /* ops_count keeps track of the number of operations written to the file */

  int ops_count = 0;

  /* bool_write indicates whether an operation has been written */
  /* or not for a specific line number                          */

  int bool_write = 0;

  /* copy_count keeps track of whether a specific operation has */
  /* been written before or not                                 */

  int copy_count = 0;

  /* temp is a operation that references the given head pointer */

  operation_t *temp = head;

  for (int i = 0; i <= largest_line_no; i++) {
    copy_count = 0;
    temp = head;
    bool_write = 0;
    while (temp != NULL) {
      if (temp->line_num == i) {
        strcpy(buffer, temp->new_text);
        if (copy_count == 0) {
          ops_count++;
        }
        copy_count++;
        bool_write = 1;
      }
      temp = temp->next_operation;
    }
    if (bool_write == 0) {
      fprintf(out_file, "\n");
    }
    else {
      fprintf(out_file, "%s\n", buffer);
    }
  }

  fclose(out_file);
  return ops_count;
} /* write_document() */
