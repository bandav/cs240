/* Vindhya Banda, hw10.c, CS 24000, Spring 2020
 * Last updated April 8rd, 2020
 */

#include "hw10.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>

/*
 * new_tab adds a new tab to the given list.
 */

void new_tab(tab_t **head) {
  assert(head != NULL);

  /* page holds the page_data for the tab to be added */

  page_data_t *page = malloc(sizeof(page_data_t));
  assert(page != NULL);
  page->current_page = true;
  page->page_no = 1;
  page->page_name = malloc(sizeof(char) * strlen("Purdue CS") + 1);
  assert(page->page_name != NULL);
  strcpy(page->page_name, "Purdue CS");
  page->url = malloc(sizeof(char) * strlen("cs.purdue.edu") + 1);
  assert(page->url != NULL);
  strcpy(page->url, "cs.purdue.edu");

  /* new is the tab to be added */

  tab_t *new = malloc(sizeof(tab_t));
  assert(new != NULL);
  new->page_info = page;
  new->prev_page = NULL;
  new->next_page = NULL;

  if (*head == NULL) {
    new->prev_tab = NULL;
    new->next_tab = NULL;
    new->tab_no = 1;
    *head = new;
    return;
  }
  /* largest_tab_no is the largest tab no in the given list */

  int largest_tab_no = 0;

  /* temp_tab_no is used to iterate through the given list */

  tab_t *temp_tab_no = *head;
  while (temp_tab_no != NULL) {
    if (temp_tab_no->tab_no > largest_tab_no) {
      largest_tab_no = temp_tab_no->tab_no;
    }
    temp_tab_no = temp_tab_no->next_tab;
  }

  new->tab_no = largest_tab_no + 1;

  /* temp is used to iterate through the given lsit */

  tab_t *temp = *head;
  while (temp->next_tab != NULL) {
    temp = temp->next_tab;
  }
  /* forward_temp iterated though the list from top to bottom */

  tab_t *forward_temp = temp;
  while (forward_temp->next_page != NULL) {
    forward_temp->next_tab = new;
    forward_temp = forward_temp->next_page;
  }
  /* backward_temp iterates through the list from bottom up */

  tab_t *backward_temp = forward_temp;
  while (backward_temp != NULL) {
    backward_temp->next_tab = new;
    backward_temp = backward_temp->prev_page;
  }

  new->next_tab = NULL;
  new->prev_tab = temp;

} /* new_tab() */

/*
 * close_tab iterates through the given list and closes
 * the tab with the specified tab number.
 */

int close_tab(tab_t **head, int tab_no) {
  assert(head != NULL);
  assert(tab_no > 0);

  /* temp references the head and is used ot traverse the given list */

  tab_t *temp = *head;
  while (temp!= NULL) {
    if (temp->tab_no == tab_no) {
      if (temp == *head) {
        *head = temp->next_tab;
      }
      /* next points to temp's next_tab and is used to traverse through the list */

      tab_t *next = temp->next_tab;
      if (next != NULL) {
        while (next->prev_page != NULL) {
          if (next->page_info->page_no == 1) {
            break;
          }
          next = next->prev_page;
        }
        while (next != NULL) {
          next->prev_tab = temp->prev_tab;
          next = next->next_page;
        }
      }
      /* prev points to temp's prev_tab and is used to traverse through the list */
      tab_t *prev = temp->prev_tab;
      if (prev != NULL) {
        while (prev->prev_page != NULL) {
          if (prev->page_info->page_no == 1) {
            break;
          }
          prev = prev->prev_page;
        }
        while (prev != NULL) {
          prev->next_tab = temp->next_tab;
          prev = prev->next_page;
        }
      }
      /* temp_2 references temp and is used to traverse through the list */

      tab_t *temp_2 = temp;
      while (temp_2->prev_page != NULL) {
        if (temp_2->page_info->page_no == 1) {
          break;
        }
        temp_2 = temp_2->prev_page;
      }
      while (temp_2 != NULL) {
        temp = temp_2;
        temp_2 = temp_2->next_page;
        free(temp->page_info->page_name);
        free(temp->page_info->url);
        free(temp->page_info);
        free(temp);
      }
      return SUCCESS;
    }
    temp = temp->next_tab;
  }
  return NO_TAB;
} /* close_tab() */

/*
 * close_browser closes all the tabs in the given list.
 */

void close_browser(tab_t **head) {
  assert(head != NULL);

  while (*head != NULL) {
    close_tab(head, (*head)->tab_no);
  }

} /* close_browser() */

/*
 * page_go_prev iterates through the list and changes the current_page
 * of the specified tab to the page preceeding it.
 */

int page_go_prev(tab_t **head, int tab_no) {

  assert(head != NULL);
  assert(tab_no > 0);

  /* tab_found is 0 if no tab with the specified tab_no is found */
  /* Otherwise, it is 1                                          */

  int tab_found = 0;

  /* temp reference the head pointer        */
  /* It is used to iterate through the list */

  tab_t *temp = *head;

  while (temp != NULL) {
    if (temp->tab_no == tab_no) {
      tab_found = 1;
      if (temp->prev_page == NULL) {
        return NO_PAGE;
      }
      temp->prev_page->page_info->current_page = true;
      if (*head == temp) {
        *head = temp->prev_page;
      }
      temp->page_info->current_page = false;
      break;
    }
    temp = temp->next_tab;
  }

  if (tab_found != 1) {
    return NO_TAB;
  }

  /* prev is the tab preceding the temp tab */
  /* temp tab here is the tab whose previous page is the current page */

  tab_t *prev = temp->prev_tab;
  if (prev != NULL) {
    if (temp == NULL) {
      printf("\n--TEMP IS NULL--\n");
    }
    while (prev->next_page != NULL) {
      prev = prev->next_page;
    }
    while (prev != NULL) {
      prev->next_tab = temp->prev_page;
      prev = prev->prev_page;
    }
  }

  /* next is the tab succeeding the temp tab */
  /* temp tab here is the tab whose previous page is the current page */

  tab_t *next = temp->next_tab;
  if (next != NULL) {
    if (temp == NULL) {
      printf("\n--2ND CHECK TEMP IS NULL--\n");
    }
    while (next->next_page != NULL) {
      next = next->next_page;
    }
    while (next != NULL) {
      next->prev_tab = temp->prev_page;
      next = next->prev_page;
    }
  }

  return SUCCESS;
} /* page_go_prev() */

/*
 * page_go_next iterates through the list and changes the current_page
 * of the specified tab to the page succeeding it.
 */

int page_go_next(tab_t **head, int tab_no) {
  assert(head != NULL);
  assert(tab_no > 0);

  /* tab_found is 0 if no tab with the specified tab_no is found */
  /* Otherwise, it is 1                                          */

  int tab_found = 0;

  /* temp reference the head pointer        */
  /* It is used to iterate through the list */

  tab_t *temp = *head;

  while (temp != NULL) {
    if (temp->tab_no == tab_no) {
      tab_found = 1;
      if (temp->next_page == NULL) {
        return NO_PAGE;
      }
      temp->next_page->page_info->current_page = true;
      if (*head == temp) {
        *head = temp->next_page;
      }
      temp->page_info->current_page = false;
      break;
    }
    temp = temp->next_tab;
  }

  if (tab_found != 1) {
    return NO_TAB;
  }

  /* prev is the tab preceding the temp tab */
  /* temp tab here is the tab whose next page is the current page */

  tab_t *prev = temp->prev_tab;
  if (prev != NULL) {
    while (prev->next_page != NULL) {
      prev = prev->next_page;
    }
    while (prev != NULL) {
      prev->next_tab = temp->next_page;
      prev = prev->prev_page;
    }
  }

  /* next is the tab succeeding the temp tab */
  /* temp tab here is the tab whose next page is the current page */

  tab_t *next = temp->next_tab;
  if (next != NULL) {
    while (next->next_page != NULL) {
      next = next->next_page;
    }
    while (next != NULL) {
      next->prev_tab = temp->next_page;
      next = next->prev_page;
    }
  }

  return SUCCESS;


} /* page_go_next() */

/*
 * open_page opens a new page with the specified attributes
 * after the current page of the tab with the give tab number.
 */

int open_page(tab_t **head, int tab_no, char* page_name, char* url) {

  assert(head != NULL);
  assert(tab_no > 0);
  assert(page_name != NULL);
  assert(url != NULL);

  /* new_page is the page to be added */

  page_data_t *new_page = malloc(sizeof(page_data_t));
  assert(new_page != NULL);
  new_page->current_page = true;
  new_page->page_name = malloc(sizeof(char) * strlen(page_name) + 1);
  assert(new_page->page_name != NULL);
  strcpy(new_page->page_name, page_name);
  new_page->url = malloc(sizeof(char) * strlen(url) + 1);
  assert(new_page->url != NULL);
  strcpy(new_page->url, url);

  /* new_tab is the new tab to be allocated for */

  tab_t *new_tab = malloc(sizeof(tab_t));
  assert(new_tab != NULL);
  new_tab->next_tab = NULL;
  new_tab->prev_tab = NULL;
  new_tab->page_info = new_page;
  new_tab->next_page = NULL;
  new_tab->prev_page = NULL;

  /* largest_page_no is the largest page number in a list of pages */

  int largest_page_no = 0;

  /* temp is used to iterate through the list of tabs */

  tab_t *temp = *head;
  while (temp != NULL) {
    while (temp->next_page != NULL) {
      temp = temp->next_page;
    }
    while (temp != NULL) {
      if (temp->page_info->page_no > largest_page_no) {
        largest_page_no = temp->page_info->page_no;
      }
      temp = temp->prev_page;
    }
    temp = temp->next_tab;
  }

  new_page->page_no = largest_page_no + 1;

  /* found holds a value of 1 if a tab with the given tab number is found */
  /* Otherwise, it is 0                                                   */

  int found = 0;

  /* temp_2 is used to iterate through the list */

  tab_t *temp_2 = *head;

  while (temp_2 != NULL) {
    if (temp_2->tab_no == tab_no) {
      found = 1;
      break;
    }
    temp_2 = temp_2->next_tab;
  }

  if (found) {
    while (temp_2->next_page != NULL) {
      temp_2 = temp_2->next_page;
    }
    while (temp_2 != NULL) {
      if (temp_2->page_info->current_page) {
        temp_2->page_info->current_page = false;
        temp_2->next_page = new_tab;
        new_tab->prev_page = temp_2;
      }
      temp_2 = temp_2->prev_page;
    }
  }
  else {
    /*free(new_page->page_name);
    new_page->page_name = NULL;
    free(new_page->url);
    new_page->url = NULL;
    free(new_page);
    new_page = NULL;

    free(new_tab);
    new_tab = NULL;*/
    return NO_TAB;
  }

  return SUCCESS;

} /* open_page() */

/*
 * num_pages returns the total number of pages across
 * all the tabs present in the given list.
 */

int num_pages(tab_t **head) {
  assert(head != NULL);

  /* temp is used to iterate through the tab list */
  tab_t *temp = *head;

  /* tab_temp is used to iterate through the pages of a tab */

  tab_t *tab_temp = NULL;

  /* count is the number of pages across all tabs in the list */

  int count = 0;

  while (temp != NULL) {
    tab_temp = temp;
    while (tab_temp->next_page != NULL) {
      tab_temp = tab_temp->next_page;
    }
    while (tab_temp != NULL) {
      count++;
      tab_temp = tab_temp->prev_page;
    }
    temp = temp->next_tab;
  }

  return count;

} /* num_pages() */

/*
 * write_all_tabs prints the specifics of all the pages
 * of the tabs present in the provided linked list to
 * the file whose file name is given.
 */

int write_all_tabs(tab_t **head, char* file) {
  assert(head != NULL);
  assert(file != NULL);

  /* fp_out is a file pointer used to write to the file */

  FILE *fp_out = fopen(file, "w");
  if (fp_out == NULL) {
    return NON_WRITABLE_FILE;
  }
  /* temp_tab points to the head and is used to iterate through tabs */

  tab_t *temp_tab = *head;

  /* temp_page is used to iterate through the pages in a tab */

  tab_t *temp_page = *head;
  while (temp_tab != NULL) {
    temp_page = temp_tab;
    while (temp_page->next_page != NULL) {
      temp_page = temp_page->next_page;
    }
    while (temp_page != NULL) {
      if (temp_page->page_info->current_page) {
        fprintf(fp_out, "TAB %d\n", temp_page->tab_no);
        fprintf(fp_out, "Page number = %d\n", temp_page->page_info->page_no);
        fprintf(fp_out, "Page name = %s\n", temp_page->page_info->page_name);
        fprintf(fp_out, "Link = %s\n\n", temp_page->page_info->url);
      }
      temp_page = temp_page->prev_page;
    }
    temp_tab = temp_tab->next_tab;
  }

  fclose(fp_out);
  fp_out = NULL;
  return SUCCESS;
} /* write_all_tabs() */


/* Remember, you don't need a main function!
 * it is provided by hw10_main.c or hw10_test.o
 */
