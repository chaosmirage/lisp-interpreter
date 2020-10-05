#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *current_char;

enum node_type {
  LIST,
  SYMBOL
};

struct node {
  struct node *next;
  enum node_type type;
  union {
    char value[1];
    struct node *list;
  };
};

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

    if (!isspace(*current_char) && *current_char != '(' && *current_char != ')') {
      tmp = calloc(1, sizeof(struct node));
      char buffer[2];
      buffer[1] = '\0';

      tmp->type = SYMBOL;
      tmp->next = NULL;

      strncpy(buffer, current_char, 1);

      strcpy(tmp->value, buffer);
    }

    if (tmp != NULL) {
      if (head == NULL) {
        head = tail = tmp;
      } else {
        tail = tail->next = tmp;
      }
    }

    current_char += 1;
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

  current_char = "(+ 1 (+ 2 1))";

  struct node *parsed = parse();

  traverse_linked_list(parsed, &print_list_item);

  return 0;
}
