/* Vindhya Banda, hw7.c, CS 24000, Spring 2020
 * Last updated March 9, 2020
 */

#include "hw7.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

password_t g_password_array[MAX_PASSWORDS];
int g_password_count = 0;

/*
 * read_passwords reads data from the specified
 * file and stores the password records into the
 * g_password_array
 */

int read_passwords(char *filename) {
  assert(filename != NULL);
  FILE *in_file = fopen(filename, "r");
  if (in_file == NULL) {
    return NON_READABLE_FILE;
  }
  fseek(in_file, 0, SEEK_END);
  if (ftell(in_file) == 0) {
    fclose(in_file);
    in_file = NULL;
    return NO_PASSWORDS;
  }
  fseek(in_file, 0, SEEK_SET);
  char code_name[MAX_BUFF_LEN];
  char passcode_name[MAX_BUFF_LEN];
  char passcode_value[MAX_BUFF_LEN];
  char next_member[MAX_BUFF_LEN];
  g_password_count = 0;

  /* result stores the return value of the subsequent fscanf call */

  int result = 0;

  while ((result = fscanf(in_file, "%50[^&\n]&%50[^&\n]&%50[^&\n]&%50[^\n]\n",
                          code_name, passcode_name, passcode_value,
                           next_member)) != EOF) {
    if (result < 4) {
      fclose(in_file);
      in_file = NULL;
      return BAD_RECORD;
    }
    if (strlen(code_name) >= MAX_BUFF_LEN) {
      fclose(in_file);
      in_file = NULL;
      return BAD_RECORD;
    }
    else if (strlen(passcode_name) >= MAX_BUFF_LEN) {
      fclose(in_file);
      in_file = NULL;
      return BAD_RECORD;
    }
    else if (strlen(passcode_value) >= MAX_BUFF_LEN) {
      fclose(in_file);
      in_file = NULL;
      return BAD_RECORD;
    }
    else if (strlen(next_member) >= MAX_BUFF_LEN) {
      fclose(in_file);
      in_file = NULL;
      return BAD_RECORD;
    }
    else {
      strncpy(g_password_array[g_password_count].code_name,
              code_name, MAX_NAME_LEN - 1);
      strncpy(g_password_array[g_password_count].passcode_name,
              passcode_name, MAX_NAME_LEN - 1);
      strncpy(g_password_array[g_password_count].passcode_value,
              passcode_value, MAX_NAME_LEN - 1);
      strncpy(g_password_array[g_password_count].next_member,
              next_member, MAX_NAME_LEN - 1);
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
 * Connect members assigns the next_member_ptr
 * of the password_t structs to an appropriate
 * value. By doing so, it 'connects' these structs.
 */

void connect_members() {
  /* found indicates whether the next_member has been identified */

  int found = 0;
  for (int i = 0; i < MAX_PASSWORDS; i++) {
    found = 0;
    for (int j = 0; j < MAX_PASSWORDS; j++) {
      if ((strcmp(g_password_array[i].next_member,
                 g_password_array[j].code_name))
          == 0) {
        if (i != j) {
          g_password_array[i].next_member_ptr = &g_password_array[j];
          found++;
        }
        else {
          g_password_array[i].next_member_ptr = NULL;
        }
        break;
      }
    }
    if (found == 0) {
      g_password_array[i].next_member_ptr = NULL;
    }
  }
} /* connect_members() */

/*
 * isolate_spy() determines if the specified member is
 * a spy. If that member proves to be a spy, they are
 * isolated from the organization.
 */

int isolate_spy(password_t *member) {
  assert(member != NULL);

  /* substr1 is a buffer for the first 3 characters of the passcode_name */

  char substr1[4];

  /* substr2 is a buffer for the last 4 characters of the passcode_name */

  char substr2[5];

  /* len holds the length of the passcode_name */

  int len = 0;

  /* is_a_spy is 0 if the member is not proven as a spy, 1 otherwise */

  int is_a_spy = 0;

  /* changes_count keeps track of the number of g_pass_array memebers */
  /* that are changed                                                 */

  int changes_count = 0;
  for (int i = 0; i < MAX_PASSWORDS; i++) {
    if (g_password_array[i].next_member_ptr == &g_password_array[i]) {
      g_password_array[i].next_member_ptr = NULL;
    }
    if (member == &g_password_array[i]) {
      len = strlen(g_password_array[i].passcode_name);
      strncpy(substr1, g_password_array[i].passcode_name, 3);
      substr1[3] = '\0';
      strncpy(substr2, &g_password_array[i].passcode_name[len - 4], 4);
      if ((strstr(g_password_array[i].passcode_value, "rooster") != 0) ||
         ((strcmp(substr1, "the") == 0) && (strcmp(substr2, "soup") == 0)) ||
         ((strlen(g_password_array[i].passcode_value)) <
          (strlen(g_password_array[i].passcode_name)))) {
        is_a_spy = 1;
      }
      if (is_a_spy == 1) {
        for (int j = 0; j < MAX_PASSWORDS; j++) {
          if (g_password_array[j].next_member_ptr == member) {
            strcpy(g_password_array[j].next_member,
                    g_password_array[i].next_member);
            if (g_password_array[j].next_member_ptr == &g_password_array[j]) {
              g_password_array[j].next_member_ptr = NULL;
            }
            else {
              g_password_array[j].next_member_ptr =
               g_password_array[i].next_member_ptr;
            }
            if (g_password_array[i].next_member_ptr == member) {
              g_password_array[i].next_member_ptr = NULL;
            }
            changes_count++;
          }
        }
        g_password_array[i].next_member_ptr = NULL;
        member->next_member_ptr = NULL;
      }
      else {
        return NOT_A_SPY;
      }
    }
  }
  return changes_count;
} /* isolate_spy() */

/*
 * send_message determines if it is viable to send
 * a message from the specified sender to the
 * recipient.
 */

int send_message(password_t *sender, password_t *recip) {
  assert((sender != NULL) && (recip != NULL));
  if (sender == recip) {
    return MESSAGE_NOT_SENT;
  }
  if (sender->next_member_ptr == sender) {
    return MESSAGE_NOT_SENT;
  }
  /* spy_count holds the number of spies found */

  int spy_count = 0;

  /* loop_count stores the number of iterations */

  int loop_count = 0;

  /* substr1 is a buffer for the first 3 characters of the passcode_name */

  char substr1[4];

  /* substr2 is a buffer for the last 4 characters of the passcode_name */

  char substr2[5];

  /* len stores the len of the passcode_name of the sender */

  int len = 0;
  for (int i = 0; i < g_password_count; i++) {
    loop_count++;
    len = strlen(sender->passcode_name);
    strncpy(substr1, sender->passcode_name, 3);
    substr1[3] = '\0';
    strncpy(substr2, &sender->passcode_name[len - 4], 4);
    substr2[4] = '\0';
    if ((strstr(sender->passcode_value, "rooster") != 0) ||
       ((strcmp(substr1, "the") == 0) && (strcmp(substr2, "soup") == 0)) ||
       ((strlen(sender->passcode_value) <
        (strlen(sender->passcode_name))))) {
      spy_count++;
    }
    if (sender == recip) {
      return spy_count;
    }
    sender = sender->next_member_ptr;
    if (sender == NULL) {
      return MESSAGE_NOT_SENT;
    }
  }
  return MESSAGE_NOT_SENT;
} /* send_message() */
