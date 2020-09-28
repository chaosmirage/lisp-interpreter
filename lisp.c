#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct node* parse(char *str) {
  char *current_char;

  struct node *first = NULL, *last = NULL, *tmp;

  for (current_char = str; *current_char; current_char++) {
    tmp = calloc(1, sizeof(struct node));

    if (*current_char == ')') {
      break;
    }

    if (*current_char == '(') {
      tmp->type = LIST;
      tmp->list = parse(current_char + 1);
      tmp->next = NULL;
    }

    if (*current_char != '(' && *current_char != ')') {
      tmp->type = SYMBOL;
      tmp->value = current_char;
      tmp->next = NULL;
    }

    if (!last) {
      first = tmp;
      last = tmp;
    }

    if (last) {
      last->next = tmp;
      last = tmp;
    }
  }

  return first;
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
  } else {
    printf("  %s = %s \n", get_node_type_name(item->type), item->value);
  }

  if (item->type == LIST) {
    print_list_item(item->list);
  }
}

void traverse_linked_list (struct node *linkedList, void (*handle)(struct node*)) {
  handle(linkedList);

  if (linkedList->next != NULL) {
    traverse_linked_list(linkedList->next, handle);
  }
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  char input[] = "(+ 1 (+ 2 1))";

  struct node *parsed = parse(input);

  traverse_linked_list(parsed, &print_list_item);

  return 0;
}
