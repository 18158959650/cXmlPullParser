/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Stack.h
 * Author: landi
 *
 * Created on 2019年5月21日, 下午9:54
 */

#ifndef STACK_H
#define STACK_H

#include "common.h"

// 栈空间默认大小
#define DEFAUT_SIZE 10
#define NAME_MAX_SIZE 50

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        char string[NAME_MAX_SIZE];
    } Data;

    typedef struct {
        Data data[DEFAUT_SIZE];
        int size;
    } Stack;

    Stack *newStack(void);
    Data pop(Stack *stack);
    void push(Stack *stack, Data *data);
    bool isStackEmpty(Stack *stack);

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */

