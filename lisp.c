#include <stdio.h>
#include <stdlib.h>

enum node_type {
  LIST,
  SYMBOL
};

struct node {
  struct node *next;
  enum node_type type;
  char value;
};

struct node* parse(char *str) {
  // TODO: стоит назвать current_char_pointer?
  char *current_char;

  struct node *first = NULL, *last = NULL, *tmp;

  for (current_char = str; *current_char; current_char++) {
    tmp = malloc(sizeof(struct node));

    tmp->type = SYMBOL;
    tmp->value = *current_char;
    tmp->next = NULL;

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
  printf("LIST ITEM\n");
  printf("type = %s \n", get_node_type_name(item->type));
  printf("value = %c \n", item->value);
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

  char input[] = "(+ 1 (+ 1 1))";

  struct node *parsed = parse(input);

  traverse_linked_list(parsed, &print_list_item);

  return 0;
}
