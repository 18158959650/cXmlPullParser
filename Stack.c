#include "Stack.h"

Stack *newStack(void)
{
    Stack *stack = xml_malloc(sizeof (Stack));
    stack->size = 0;
    memset(&stack->data, 0, sizeof (stack->data));

    if (stack == NULL)
    {
        println("newStack fail!");
    }

    return stack;
}

Data pop(Stack *stack)
{
    Data data;
    memset(&data, 0, sizeof (Data));
    if (!isStackEmpty(stack))
    {
        data = stack->data[stack->size - 1];
        stack->size--;
    }
    return data;
}

void push(Stack *stack, Data *data)
{
    int size = stack->size + 1;
    if (size > DEFAUT_SIZE)
    {
        println("stack size too large!");
        return;
    }

    stack->data[stack->size++] = *data;
}

bool isStackEmpty(Stack *stack)
{
    return stack->size == 0;
}
