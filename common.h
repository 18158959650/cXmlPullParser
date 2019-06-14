/* 
 * File:   common.h
 * Author: zhanglei
 *
 * Created on 2019年5月21日, 下午10:16
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

typedef int bool;

#define true 1
#define false 0

#ifdef _VAR_DEFINED_
#define EXTERN
#else
#define EXTERN extern
#endif

// 日志记录
EXTERN int (*print) (const char*, ...);
EXTERN void *(*xml_malloc)(size_t sz);
EXTERN void (*xml_free)(void *ptr);
EXTERN void println(const char*, ...);

EXTERN char *skip_space(char *string);
EXTERN int strcmp_part(const char *source, const char *target);
EXTERN char *skip_white_space(char *string);
EXTERN bool isValidNameStartChar(char _name_start_char);
EXTERN bool isValidNameChar(char _name_char);
EXTERN bool isValidAttrStartChar(char _attr_start_char);

#endif /* COMMON_H */

