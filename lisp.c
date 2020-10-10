#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 256

char *current_char;

enum node_type {
  LIST,
  SYMBOL
};

struct node {
  struct node *next;
  enum node_type type;
  union {
    char *value;
    struct node *list;
  };
};

int is_term (int c) {
  return c == '\0' || isspace(c) || c == '(' || c == ')';
};

char *read_value() {
  int length = 0;
  char buffer[BUFFER_SIZE + 1];

  while (!is_term(*current_char) && length < BUFFER_SIZE) {
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

    if (!is_term(*current_char)) {
      tmp = calloc(1, sizeof(struct node));

      tmp->type = SYMBOL;
      tmp->next = NULL;
      tmp->value = read_value();
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
  }
}

void print_list_item(struct node *item) {
  if (item->type == LIST) {
    printf("%s\n", get_node_type_name(item->type));
  }

  if (item->type == SYMBOL) {
    printf("  %s = %s \n", get_node_type_name(item->type), item->value);
  }
}

void traverse_linked_list (struct node *linkedList, void (*handle)(struct node*)) {
  handle(linkedList);

  if (linkedList->next != NULL) {
    traverse_linked_list(linkedList->next, handle);
  }

  if (linkedList->next == NULL && linkedList->type == LIST) {
    traverse_linked_list(linkedList->list, handle);
  }
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  current_char = "(+ 10000 (+ 200 301))";

  struct node *parsed = parse();

  traverse_linked_list(parsed, &print_list_item);

  return 0;
}
