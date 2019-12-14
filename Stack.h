/* 
 * File:   Stack.h
 * Author: zhanglei
 *
 * Created on 2019年5月21日, 下午9:54
 */

#ifndef STACK_H
#define STACK_H

#include "common.h"

// 栈空间默认大小
#define STACK_DEFAUT_SIZE 10
#define NAME_MAX_SIZE 50

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        char string[NAME_MAX_SIZE + 1];
    } Data;

    typedef struct {
        Data data[STACK_DEFAUT_SIZE];
        int size;
    } Stack;

    Stack *stack_new(void);
    Data stack_pop(Stack *stack);
    void stack_push(Stack *stack, Data *data);
    bool stack_is_empty(Stack *stack);

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */

