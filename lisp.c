#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>

#ifdef __LP64__
typedef u_int64_t value_t;
typedef int64_t number_t;
#else
typedef u_int32_t value_t;
typedef int32_t number_t;
#endif

#define N_STACK 49152
static value_t Stack[N_STACK];
static u_int32_t SP = 0;
#define PUSH(v) (Stack[SP++] = (v))
#define POP() (Stack[--SP])

#define ADD_OPERATOR '+'

#define BUFFER_SIZE 256

char *current_char;

enum node_type
{
  LIST,
  SYMBOL,
  INTEGER,
};

struct node
{
  struct node *next;
  enum node_type type;
  union
  {
    char *value;
    struct node *list;
  };
};

int is_terminator(int c)
{
  return c == '\0' || isspace(c) || c == '(' || c == ')';
};

int is_number(int c)
{
  return c >= '0' && c <= '9';
}

char *read_value()
{
  int length = 0;
  char buffer[BUFFER_SIZE + 1];

  while (!is_terminator(*current_char) && length < BUFFER_SIZE)
  {
    buffer[length] = *current_char;

    current_char += 1;
    length += 1;
  }

  buffer[length] = '\0';
  char *str = malloc((length + 1) * sizeof(char));

  strcpy(str, buffer);

  return str;
}

struct node *parse()
{
  struct node *first = NULL, *last = NULL;

  while (*current_char)
  {
    struct node *tmp = NULL;

    if (*current_char == ')')
    {
      break;
    }

    if (*current_char == '(')
    {
      tmp = calloc(1, sizeof(struct node));
      tmp->type = LIST;
      tmp->next = NULL;

      current_char += 1;

      tmp->list = parse();
    }

    if (!is_terminator(*current_char))
    {
      tmp = calloc(1, sizeof(struct node));

      if (is_number(*current_char))
      {
        tmp->type = INTEGER;
        tmp->next = NULL;
        tmp->value = read_value();
      }
      else
      {
        tmp->type = SYMBOL;
        tmp->next = NULL;
        tmp->value = read_value();
      }
    }

    if (tmp != NULL)
    {
      if (first == NULL)
      {
        first = last = tmp;
      }
      else
      {
        last = last->next = tmp;
      }
    }

    if (*current_char != '\0')
    {
      current_char += 1;
    }
  }

  return first;
}

int to_integer(char *str)
{
  int number = 0;

  while (*str)
  {
    number = number * 10 + (*str - '0');
    str += 1;
  }

  return number;
}

int compute_sum(struct node *linked_list)
{
  number_t result;

  if (linked_list == NULL)
  {
    result = 0;
    while (SP)
    {
      result += POP();
    }

    return result;
  }

  if (linked_list->type == INTEGER)
  {
    PUSH(to_integer(linked_list->value));
    return compute_sum(linked_list->next);
  }

  return 0;
}

int eval(struct node *linked_list)
{
  if (linked_list->type == SYMBOL && *linked_list->value == ADD_OPERATOR)
  {
    return compute_sum(linked_list->next);
  }

  if (linked_list->type == LIST)
  {
    return eval(linked_list->list);
  }

  return 0;
}

char *get_node_type_name(enum node_type type)
{
  switch (type)
  {
  case LIST:
    return "List";
  case SYMBOL:
    return "Symbol";
  case INTEGER:
    return "Integer";
  }
}

void print_list_item(struct node *item, int is_last_item)
{
  static int list_count = 0;
  char *node_type_name = get_node_type_name(item->type);
  int node_type_name_length = strlen(node_type_name);

  if (item->type == LIST)
  {
    printf("%*s\n", node_type_name_length + list_count, node_type_name);
    list_count += 1;
  }
  else
  {
    printf("%*s", node_type_name_length + list_count * 2, node_type_name);
    printf(" = %s \n", item->value);
  }

  if (is_last_item)
  {
    list_count = 0;
  }
}

void traverse_linked_list(struct node *linkedList, void (*handler)(struct node *, int))
{
  int is_last_item = linkedList->next == NULL && linkedList->type != LIST;
  handler(linkedList, is_last_item);

  if (linkedList->next != NULL)
  {
    traverse_linked_list(linkedList->next, handler);
  }

  if (linkedList->next == NULL && linkedList->type == LIST)
  {
    traverse_linked_list(linkedList->list, handler);
  }
}

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  current_char = "(+ 150 200 350)";
  struct node *parsed_example_1 = parse();
  traverse_linked_list(parsed_example_1, *print_list_item);

  int computed_sum = eval(parsed_example_1);
  printf("computed sum = %d\n", computed_sum);

  return 0;
}
