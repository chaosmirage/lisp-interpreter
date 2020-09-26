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
  char value;
};

// Выделить память под буфер: длина исходной строки + длина строки замены
// Найти позицию заменяемого символа
// Скопировать в буфер всё что есть слева от искомого символа
// Вставить в буфер замену
// Скопировать в буфер всё что есть справа от искомого символа

void replace_all_char_by_str(char *str, int char_to_be_replaced, const char replace_str[]) {
  int buf_size = strlen(str) + strlen(replace_str);
  char *buf = (char*)malloc(buf_size);
  char *occurrence_pointer = strchr(str, char_to_be_replaced);


  buf = strncpy(buf, str, occurrence_pointer - str);

  int buf_length = strlen(buf);

  strcpy(buf + buf_length, replace_str);
  buf_length += strlen(replace_str);
  strcpy(buf + buf_length, str + 1);

   // printf("occurrence_pointer = %s\n", occurrence_pointer);

  printf("buf = %s\n", buf);

  strcpy(str, buf);
  free(buf);
}

void tokenize(char *str) {
  char *strCopy = strdup(str);
  char delim[] = " ";

  replace_all_char_by_str(strCopy, '(', " ( ");
  // replace_all_char_by_str(strCopy, ')', " ) ");

  // printf("%s\n", strCopy);

  char *token = strtok(strCopy, delim);

  while(token != NULL) {
    // printf("%s\n", token);
    token = strtok(NULL, delim);
  }
}

struct node* parse(char *str) {
  // TODO: стоит назвать current_char_pointer?
  char *current_char;

  struct node *first = NULL, *last = NULL, *tmp;

  for (current_char = str; *current_char; current_char++) {
    tmp = calloc(1, sizeof(struct node));

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

  char input[] = "(+ 1 (+ 2 1))";

  struct node *parsed = parse(input);

  // traverse_linked_list(parsed, &print_list_item);

  tokenize(input);
  return 0;
}
