/* 
 * File:   common.c
 * Author: zhanglei
 *
 * Created on 2019年5月21日, 下午10:30
 */

#define _VAR_DEFINED_
#include "common.h"

// 不用的应用平台修改对应的函数指针
int (*print) (const char*, ...) = printf;
void *(*xml_malloc)(size_t sz) = malloc;
void *(*xml_realloc)(void *__ptr, size_t __size) = realloc;
void (*xml_free)(void *ptr) = free;

void println(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    print(fmt, args);
    va_end(args);
    print("\n");
}

char *skip_space(char *string)
{
    char *p = string;
    while (*p == 0x20) p++;
    return p;
}

int strcmp_part(const char *source, const char *target)
{
    return memcmp(source, target, strlen(target));
}

/**
 * 跳过不可现字符
 * @param string 
 */
char *skip_white_space(char *string)
{
    char *p = string;
    while (*p != 0 && *p <= 0x20)
    {
        p++;
    }
    return p;
}

/**
 * TODO
 * 暂不全面
 * 先设置常用的
 * @param _name_char
 * @return 
 */
bool isValidNameChar(char _name_char)
{
    bool isNumber = _name_char >= '0' && _name_char <= '9';
    return isNumber || isValidNameStartChar(_name_char);
}

/**
 * TODO
 * 暂不全面
 * 先设置常用的
 * @param _name_start_char
 * @return 
 */
bool isValidNameStartChar(char _name_start_char)
{
    bool isAlphabet = _name_start_char >= 'A' && _name_start_char <= 'Z'
            || (_name_start_char >= 'a' && _name_start_char <= 'z');
    bool isValidSign = _name_start_char == '-';

    return isAlphabet || isValidSign;
}

/**
 * TODO
 * 暂不全面
 * 先设置常用的
 * @param _name_start_char
 * @return 
 */
bool isValidAttrStartChar(char _attr_start_char)
{
    bool isAlphabet = _attr_start_char >= 'A' && _attr_start_char <= 'Z'
            || (_attr_start_char >= 'a' && _attr_start_char <= 'z');
    return isAlphabet;
}
