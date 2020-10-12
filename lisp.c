#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 256

char *current_char;

enum node_type {
  LIST,
  SYMBOL,
  INTEGER,
};

struct node {
  struct node *next;
  enum node_type type;
  union {
    char *value;
    struct node *list;
  };
};

int is_terminator (int c) {
  return c == '\0' || isspace(c) || c == '(' || c == ')';
};

int is_number (int c) {
  return c >= '0' && c <= '9';
}

char *read_value() {
  int length = 0;
  char buffer[BUFFER_SIZE + 1];

  while (!is_terminator(*current_char) && length < BUFFER_SIZE) {
    buffer[length] = *current_char;

    current_char += 1;
    length += 1;
  }

  buffer[length] = '\0';
  char *str = malloc((length + 1) * sizeof(char));

  strcpy(str, buffer);

  return str;
}

struct node* parse() {
  struct node *head = NULL, *tail = NULL;

  while(*current_char) {
    struct node *tmp = NULL;

    if (*current_char == ')') {
      break;
    }

    if (*current_char == '(') {
      tmp = calloc(1, sizeof(struct node));
      tmp->type = LIST;
      tmp->next = NULL;

      current_char += 1;

      tmp->list = parse();
    }

    if (!is_terminator(*current_char)) {
      tmp = calloc(1, sizeof(struct node));

      if(is_number(*current_char)) {
        tmp->type = INTEGER;
        tmp->next = NULL;
        tmp->value = read_value();
      } else {
        tmp->type = SYMBOL;
        tmp->next = NULL;
        tmp->value = read_value();
      }
    }

    if (tmp != NULL) {
      if (head == NULL) {
        head = tail = tmp;
      } else {
        tail = tail->next = tmp;
      }
    }

    if (*current_char != '\0') {
      current_char += 1;
    }
  }

  return head;
}

char* get_node_type_name(enum node_type type) {
  switch (type) {
    case LIST: return "List";
    case SYMBOL: return "Symbol";
    case INTEGER: return "Integer";
  }
}

void print_list_item(struct node *item, int print_offset) {
  char *node_type_name = get_node_type_name(item->type);
  int node_type_name_length = strlen(node_type_name);

  if (item->type == LIST) {
    printf("%*s\n", node_type_name_length + print_offset, node_type_name);
  } else {
    printf("%*s", node_type_name_length + print_offset * 2, node_type_name);
    printf(" = %s \n", item->value);
  }
}

void traverse_linked_list (struct node *linkedList) {
  static int list_count = 0;

  print_list_item(linkedList, list_count);

  if (linkedList->next != NULL) {
    traverse_linked_list(linkedList->next);
  }

  if (linkedList->next == NULL && linkedList->type == LIST) {
    list_count += 1;
    traverse_linked_list(linkedList->list);
  }

  if (linkedList->next == NULL) {
    list_count = 0;
  }
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  current_char = "(+ 10000 (+ 200 301) (car '(1 2)))";

  struct node *parsed = parse();

  traverse_linked_list(parsed);

  return 0;
}
