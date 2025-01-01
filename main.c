#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

const char* integer_pattern = "^[+-]?([0-9]*[.])?[0-9]+$";
const char* operator_pattern = "^[\\+\\-\\*/%]$";

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
  struct node* left;
  struct node* right;
};

struct Node* createNode(int value, enum ValueType type)
{
  struct Node* newNode = (struct node*)malloc(sizeof(struct Node));
  if (type == OPERAND) {
    newNode->operand = value;
  }
  else {
    newNode->operator = (char) value;
  }
  newNode->type = type;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

int isValid(int data, int bind)
{
  regex_t regex;
  int ret;

  ret = (bind) ? regcomp(&regex, integer_pattern, REG_EXTENDED) : regcomp(&regex, operator_pattern, REG_EXTENDED);

  if (ret)
  {
    printf("Error compiling regex\n");
    return 0;
  }

  ret = regexec(&regex, data, 0, NULL, 0);
  regfree(&regex);

  return (ret == 0);
}

struct Node* insertNode(struct Node* root, int data)
{
  if (root == NULL)
  {
    return createNode(data, OPERATOR);
  }

  struct Node* queue[100];
  int front = 0, rear = 0;

  queue[rear++] = root;

  while (front < rear)
  {
    struct Node* curr = queue[front++];


  }
}

int main(unsigned int argc, char** argv)
{

  return 0;
}

// 1  + 1 + 1


/*
    +
  +   1
1   1
*/