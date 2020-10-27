/* Vindhya Banda, hw6.c, CS 24000, Spring 2020
 * Last updated March 2, 2020
 */

#include "hw6.h"

#include<stdio.h>
#include <assert.h>
#include<string.h>

password_t g_password_array[MAX_PASSWORDS];
int g_password_count = 0;

/*
 * read_passwords reads data from the specified
 * file and stores the password records into the
 * g_password_array
 */

int read_passwords(char *filename) {
  assert(filename != NULL);

  /* in_file is a pointer to the file to be read */

  FILE *in_file = fopen(filename, "r");
  if (in_file == NULL) {
    return NON_READABLE_FILE;
  }
  fseek(in_file, 0, SEEK_SET);
  char code_name[MAX_BUFF_LEN];
  char passcode_name[MAX_BUFF_LEN];
  char passcode_value[MAX_BUFF_LEN];
  g_password_count = 0;

  /* result stores the return value of the subsequent fscanf call */

  int result = 0;
  while ((result = fscanf(in_file, "%50[^&\n]&%50[^&\n]&%50[^\n]\n",
                          code_name, passcode_name, passcode_value)) != EOF) {
    if (result < 3) {
      fclose(in_file);
      in_file = NULL;
      return BAD_RECORD;
    }
    if (strlen(code_name) >= MAX_NAME_LEN) {
      fclose(in_file);
      in_file = NULL;
      return BAD_RECORD;
    }
    else if (strlen(passcode_name) >= MAX_NAME_LEN) {
      fclose(in_file);
      in_file = NULL;
      return BAD_RECORD;
    }
    else if (strlen(passcode_value) >= MAX_NAME_LEN) {
      fclose(in_file);
      in_file = NULL;
      return BAD_RECORD;
    }
    else {
      strncpy(g_password_array[g_password_count].code_name,
              code_name, MAX_NAME_LEN);
      strncpy(g_password_array[g_password_count].passcode_name,
              passcode_name, MAX_NAME_LEN);
      strncpy(g_password_array[g_password_count].passcode_value,
              passcode_value, MAX_NAME_LEN);
      g_password_count++;
    }
    if (g_password_count > MAX_PASSWORDS) {
      fclose(in_file);
      in_file = NULL;
      return TOO_MUCH_DATA;
    }
  }
  fclose(in_file);
  in_file = NULL;
  return g_password_count;
} /* read_passwords() */

/*
 * Checks for spies in the g_password_array whose
 * code names match those in the given list and
 * adhere to the dictated criteria (of spy qualities).
 */

int locate_spies(char **code_names, int num_names) {
  assert(code_names != NULL);
  assert(num_names > 0);

  /* spy_count counts the number of identified spies */

  int spy_count = 0;
  for (int i = 0; i < num_names; i++) {
    assert(code_names[i] != NULL);
  }
  /* subtr1 stores the first 3 letters of the passcode name */

  char substr1[4];

  /* substr2 stores the last 4 letters of the passcode name */

  char substr2[5];

  /* list_of_names contains those code_names of  */
  /* identified spies */

  char list_of_names[num_names * MAX_NAME_LEN];

  /* len stores the length of the passcode name */

  int len = 0;

  /* copy_count keeps track of any repetitions in the input list */

  int copy_count = 0;
  for (int i = 0; i < MAX_PASSWORDS; i++) {
    copy_count = 0;
    for (int j = 0; j < num_names; j++) {
      len = strlen(g_password_array[i].passcode_name);
      strncpy(substr1, g_password_array[i].passcode_name, 3);
      substr1[3] = '\0';
      strncpy(substr2, &g_password_array[i].passcode_name[len - 4], 4);
      if (strcmp(g_password_array[i].code_name, code_names[j]) == 0) {
        if ((strstr(g_password_array[i].passcode_value, "rooster") != 0) ||
           ((strcmp(substr1, "the") == 0) && (strcmp(substr2, "soup") == 0)) ||
           (strlen(g_password_array[i].passcode_value)
            < strlen(g_password_array[i].passcode_name))) {
          copy_count++;
          if (copy_count == 1) {
            spy_count++;
            strcat(list_of_names, code_names[j]);
          }
        }
      }
    }
  }
  return spy_count;
} /* locate_spies() */
