#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum ValueType {
  OPERAND,
  OPERATOR
};

struct Node {
  union {
    int operand;
    char operator;
  };
  enum ValueType type;
  struct Node* left;
  struct Node* right;
};

struct Tuple {
  struct Node* node;
  char* expression;
};

void free_tree(struct Node* node)
{
  if (node == NULL) return;

  free_tree(node->left);
  free_tree(node->right);

  free(node);
}

struct Node* create_node(int operand, char operator_char, enum ValueType type)
{
  struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
  if (newNode == NULL)
  {
    return NULL;
  }

  if (type == OPERAND)
  {
    newNode->operand = operand;
  }
  else
  {
    newNode->operator = operator_char;
  }
  newNode->type = type;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

struct Tuple parse_numeric(char* expression)
{
  struct Tuple t = { NULL, expression };
  if (!expression || !*expression) return t;

  if (!isdigit(*expression))
  {
    return t;
  }

  int value = 0;
  while (isdigit(*expression))
  {
    value = value * 10 + (*expression - '0');
    expression++;
  }

  t.node = create_node(value, '\0', OPERAND);
  t.expression = expression;
  return t;
}

struct Tuple parse_parenthesis(char* expression);
struct Tuple parse_add_sub(char* expression);
struct Tuple parse_mult_div_mod(char* expression);
struct Tuple parse_exp_rad(char* expression);

struct Tuple parse_parenthesis(char* expression)
{
  if (!expression || !*expression)
  {
    struct Tuple t = { NULL, expression };
    return t;
  }

  if (*expression == '(')
  {
    expression++;
    struct Tuple result = parse_add_sub(expression);
    if (result.expression && *result.expression == ')') {
      result.expression++;
    }
    return result;
  }
  return parse_numeric(expression);
}

struct Tuple parse_mult_div_mod(char* expression)
{
  struct Tuple t = parse_exp_rad(expression);
  struct Node* left = t.node;
  expression = t.expression;

  while (*expression == '/' || *expression == '*' || *expression == '%')
  {
    char op = *expression;
    expression++;

    struct Tuple right_result = parse_exp_rad(expression);
    struct Node* right = right_result.node;
    expression = right_result.expression;

    struct Node* node = create_node(0, op, OPERATOR);
    node->left = left;
    node->right = right;
    left = node;
  }

  struct Tuple result_exp = { left, expression };
  return result_exp;
}

struct Tuple parse_exp(char* expression)
{
  struct Tuple t = parse_parenthesis(expression);
  struct Node* left = t.node;
  expression = t.expression;

  while (*expression == '^')
  {
    char op = *expression;
    expression++;

    struct Tuple right_result = parse_parenthesis(expression);
    struct Node* right = right_result.node;
    expression = right_result.expression;

    struct Node* node = create_node(0, op, OPERATOR);
    node->left = left;
    node->right = right;
    left = node;
  }

  struct Tuple result_exp = { left, expression };
  return result_exp;
}

struct Tuple parse_add_sub(char* expression)
{
  struct Tuple t = parse_mult_div_mod(expression);
  struct Node* left = t.node;
  expression = t.expression;

  while (*expression == '+' || *expression == '-')
  {
    char op = *expression;
    expression++;

    struct Tuple right_result = parse_mult_div_mod(expression);
    struct Node* right = right_result.node;
    expression = right_result.expression;

    struct Node* node = create_node(0, op, OPERATOR);
    node->left = left;
    node->right = right;
    left = node;
  }

  struct Tuple result = { left, expression };
  return result;
}

struct Node* parse_exp(char* expression)
{
  if (expression == NULL)
  {
    return NULL;
  }

  printf("Parsing expression: %s\n", expression);

  struct Tuple t = parse_add_sub(expression);
  if (t.node == NULL || t.expression == NULL)
  {
    if (t.node != NULL) {
      free_tree(t.node);
    }
    return NULL;
  }

  return t.node;
}

double resolve_tree(struct Node* root)
{
  if (root == NULL) return 0;

  if (root->type == OPERAND) return root->operand;

  double left_result = resolve_tree(root->left);
  double right_result = resolve_tree(root->right);

  double result;
  if (root->type == OPERATOR)
  {
    switch (root->operator)
    {
    case '+':
      result = left_result + right_result;
      break;
    case '-':
      result = left_result - right_result;
      break;;
    case '*':
      result = left_result * right_result;
      break;
    case '%':
      result = (int) left_result % (int) right_result;
      break;
    case '^':
      int base = left_result;
      for (int i = 0; i < right_result; i++)
      {
        left_result = left_result * base;
      }
      result = left_result;
      break;
    case '/':
      if (right_result != 0)
      {
        result = left_result / right_result;
      }
      else
      {
        printf("Error: Division by zero!\n");
        result = 0;
      }
      break;
    default:
      printf("Error: Unknown operator '%c'\n", root->operator);
      result = 0;
      break;
    }
  }
  return result;
}

void print_tree(struct Node* node)
{
  if (node == NULL) return;
  if (node->type == OPERAND)
  {
    printf("%d", node->operand);
  }
  else
  {
    printf("(");
    print_tree(node->left);
    printf(" %c ", node->operator);
    print_tree(node->right);
    printf(")");
  }
}

/*The expression must be passed without spaces*/
int main(void)
{
  printf("Initializing program...\n");

  char* expression = "3^2+2";
  printf("Expression loaded: %s\n", expression);

  struct Node* result = parse_exp(expression);

  if (result != NULL)
  {
    printf("Result tree: ");
    print_tree(result);
    printf("\n");

    double op_result = resolve_tree(result);
    printf("Resolved tree: %.2lf\n", op_result);

    free_tree(result);
  }
  else
  {
    printf("Failed to parse expression\n");
  }

  return 0;
}
