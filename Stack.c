#include "Stack.h"

Stack *stack_new(void)
{
    Stack *stack = xml_malloc(sizeof (Stack));
    stack->size = 0;
    memset(&stack->data, 0, sizeof (stack->data));

    if (stack == NULL)
    {
        print("newStack fail!\n");
    }

    return stack;
}

Data stack_pop(Stack *stack)
{
    Data data;
    memset(&data, 0, sizeof (Data));
    if (!stack_is_empty(stack))
    {
        data = stack->data[stack->size - 1];
        stack->size--;
    }
    return data;
}

void stack_push(Stack *stack, Data *data)
{
    int size = stack->size + 1;
    if (size > STACK_DEFAUT_SIZE)
    {
        print("stack size too large!\n");
        return;
    }

    stack->data[stack->size++] = *data;
}

bool stack_is_empty(Stack *stack)
{
    return stack->size == 0;
}
