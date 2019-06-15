/*
 * Copyright (c) 2019 Zhang Lei
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */

#include "xmlpullparser.h"

// 以下函数为内部使用函数,考虑到可能在一些嵌入式
// 系统平台上无法使用static, 故不加static修饰符
void set_text(XmlParser *parser, char *begin, int size);
void set_next_text(XmlParser *parser, char *begin, int size);
int parse_declaration(XmlParser *parser);
int parse_comment(XmlParser *parser);
int parse_cdata(XmlParser *parser);
int parse_starttag(XmlParser *parser);
int parse_endtag(XmlParser *parser);
int parse_text(XmlParser *parser);
int parse_attribute(XmlParser *parser, char *p_text_start);
bool add_attr_value_list(XmlParser *parser, char *name, char *value);
int check_end(XmlParser *parser);
void clear_attr(XmlParser *xml_parser);

XmlParser *newXmlParser(char *read_buffer)
{
    XmlParser *parser = xml_malloc(sizeof (XmlParser));
    if (parser == NULL)
    {
        println("newXmlParser fail!");
    }
    else
    {
        memset(parser, 0, sizeof (XmlParser));
        parser->eventType = END_DOCUMENT;
        parser->string = read_buffer;
        parser->workp = read_buffer;
        parser->text = xml_malloc(DEFAULT_TEXT_SIZE);
        parser->nextText = xml_malloc(DEFAULT_TEXT_SIZE);
        parser->names_stack = newStack();
    }

    return parser;
}

int getXmlEventType(XmlParser *parser)
{
    println("eventType = %d", parser->eventType);
    return parser->eventType;
}

/**
 * START_TAG 之后的文本
 */
char *nextText(XmlParser *parser)
{
    char *next_text = (char *) NULL;
    char *p = parser->workp;
    p = skip_white_space(p);

    int eventType;
    if (*p != 0 && *p != '<')
    {
        eventType = parse_text(parser);
        if (eventType == TEXT)
        {
            next_text = parser->nextText;
        }
    }
    return next_text;
}

int getStartEventType(XmlParser *parser)
{
    char *p = skip_white_space(parser->string);
    if (*p == '<')
    {
        // 有声明
        if (*(p + 1) == '?')
        {
            parser->workp = p;
            return parse_declaration(parser);
        }
        // 无声明
        parser->eventType = START_DOCUMENT;
        parser->workp = p;
    }

    return parser->eventType;
}

int getNext(XmlParser *parser)
{
    clear_attr(parser);

    int eventType = parser->eventType;
    char *p = parser->workp;

    if (eventType == START_TAG)
    {
        if (*p != '<')
        {
            parser->workp = p;
            return parse_text(parser);
        }
    }
    else
    {
        p = skip_white_space(p);
    }

    if (*p == 0)
    {
        return check_end(parser);
    }

    if (*p == '<')
    {
        if (*++p == '!')
        {
            if (!strcmp_part(p + 1, "--"))
            {
                p += 3;
                parser->workp = p;
                return parse_comment(parser);
            }
            else if (!strcmp_part(p + 1, "[CDATA["))
            {
                p += strlen("[CDATA[");
                parser->workp = p;
                return parse_cdata(parser);
            }
        }
        else if (*p == '/')
        {
            p++;
            parser->workp = p;
            // 元素终结
            return parse_endtag(parser);
        }
        else if (isValidNameStartChar(*p))
        {
            parser->workp = p;
            return parse_starttag(parser);
        }
    }

    return terminate(parser, p);
}

/**
 * 解析XML声明
 * @param parser
 * @return 
 */
int parse_declaration(XmlParser *parser)
{
    char *begin;
    char *p = parser->workp;
    if (p != parser->string)
    {
        print("cannot start a declaration with a blank space!\n");
        return terminate(parser, p);
    }
    begin = p;
    // 跳过<?
    p = p + 2;
    bool isVaildVersion = false;

    if (!strcmp_part(p, "xml"))
    {
        p = skip_space(p + 3);
        // 判断版本
        if (!strcmp_part(p, "version"))
        {
            p += strlen("version");
            p = skip_space(p);
            if (*p == '=')
            {
                p = skip_space(p + 1);
                if (!strcmp_part(p, "\"1.0\""))
                {
                    isVaildVersion = true;
                    p += strlen("\"1.0\"");
                }
            }
        }

        if (isVaildVersion)
        {
            while (*p != 0 && *p != '<')
            {
                if (*p == '?')
                {
                    // 声明结束
                    if (*(p + 1) == '>')
                    {
                        p = p + 2;
                        set_text(parser, begin, p - begin);
                        parser->eventType = DOCDECL;
                        parser->workp = p;
                        return DOCDECL;
                    }
                    break;
                }
                p++;
            }
        }
    }

    return terminate(parser, p);
}

/**
 * 解析XML注释
 * @param parser
 * @return 
 */
int parse_comment(XmlParser *parser)
{
    // 当前工作指针在<!--之后
    char *p = parser->workp;
    while (*p != 0 && *p != '<')
    {
        if (!strcmp_part(p, "-->"))
        {
            set_text(parser, parser->workp, p - parser->workp);
            parser->eventType = COMMENT;
            parser->workp = p + 3;
            return COMMENT;
        }
        p++;
    }

    return terminate(parser, p);
}

/**
 * 解析CDATA
 * @param parser
 * @return 
 */
int parse_cdata(XmlParser *parser)
{
    // 当前工作指针在<![CDATA[之后
    char *p = parser->workp;
    char *begin = p;

    while (*p != 0)
    {
        if (!strcmp_part(p, "]]>"))
        {
            // 设置text值    
            set_text(parser, begin, p - begin + 1);
            parser->eventType = CDSECT;
            parser->workp = p + 3;
            return CDSECT;
        }
        p++;
    }

    return terminate(parser, p);
}

int parse_starttag(XmlParser *parser)
{
    // 指向tag name 第一个字符
    char *p = parser->workp;
    char *name_begin = p;
    char tag_name[NAME_MAX_SIZE + 1] = {0};
    int len = 0;
    bool has_no_end_tag = false;

    while (*p != 0)
    {
        if (isValidNameChar(*p)) p++;
        else if (*p == ' ')
        {
            len = p - name_begin;
            if (len < NAME_MAX_SIZE)
            {
                memcpy(tag_name, name_begin, len);
                memcpy(parser->tagName, tag_name, len);
                parser->tagName[len] = 0;
                parser->eventType = START_TAG;
            }
            else
            {
                print("The length of node's name is too long:len = %d\n", len);
                break;
            }
            // 解析属性
            p = skip_space(p + 1);
            if (isValidAttrStartChar(*p))
            {
                parser->workp = p;
                return parse_attribute(parser, name_begin - 1);
            }
            else break;
        }
        else if (*p == '>')
        {
            len = p - name_begin;
            if (has_no_end_tag)
            {
                // <XX/> p 指向> 所以要减1
                len = len - 1;
            }
            if (len > NAME_MAX_SIZE)
            {
                print("The length of node's name is too long:len = %d\n", len);
                break;
            }
            else
            {
                memcpy(tag_name, name_begin, len);
                memcpy(parser->tagName, tag_name, len);
                parser->tagName[len] = 0;
                parser->eventType = START_TAG;
                set_text(parser, name_begin - 1, p - name_begin + 2);

                // 修改工作指针
                parser->workp = ++p;

                // 没有单独的结束标签
                // 只有有单独的闭合标签才需要入栈
                if (has_no_end_tag == false)
                {
                    // tagName入栈 在解析到end tag时 跟栈顶的name比较
                    // 如果一直node 的开闭是正确的
                    Data data;
                    memset(&data, 0, sizeof (Data));
                    memcpy(data.string, tag_name, len);
                    push(parser->names_stack, &data);
                }

                return START_TAG;
            }
        }
        else if (*p == '/')
        {
            // 判断下一个字符是否是'>'
            // 不是就终止解析
            if (*++p != '>') break;
            else has_no_end_tag = true;
        }
        else
        {
            break;
        }
    }

    return terminate(parser, p);
}

int parse_endtag(XmlParser *parser)
{
    char tag_name[NAME_MAX_SIZE + 1] = {0};
    char *p = parser->workp;
    char *begin = p;
    int len = 0;
    bool has_get_tag_name = false;

    while (*p != 0 && *p != '<')
    {
        if (*p == '>')
        {
            len = p - parser->workp;
            if (len > NAME_MAX_SIZE)
            {
                print("The length of node's name is too long:len = %d\n", len);
                return terminate(parser, parser->workp);
            }

            memcpy(tag_name, parser->workp, len);
            has_get_tag_name = true;
            break;
        }
        p++;
    }

    if (has_get_tag_name)
    {
        // 获取开始节点name
        Data data = pop(parser->names_stack);

        if (!strcmp(tag_name, data.string))
        {
            memcpy(parser->tagName, tag_name, len);
            parser->tagName[len] = 0;
            parser->eventType = END_TAG;
            set_text(parser, begin - 2, p - begin + 3);
            parser->workp = ++p;
            return END_TAG;
        }
        else
        {
            if (parser->names_stack->size > 0)
            {
                print("error:endtag[%s] is not equal starttag[%s]\n",
                      tag_name, data.string);
            }
            else
            {
                print("error:endtag[%s] has no starttag\n", tag_name);
            }
            return terminate(parser, begin);
        }
    }

    return terminate(parser, p);
}

bool is_entity_ref(char *string);

int parse_text(XmlParser *parser)
{
    char *begin = parser->workp;
    char *p = begin;
    while (*p != 0)
    {
        // 下一个TAG的开始
        if (*p == '<')
        {
            set_text(parser, begin, p - begin);
            set_next_text(parser, begin, p - begin);
            parser->eventType = TEXT;
            parser->workp = p;
            return TEXT;
        }
        else if (*p == '&')
        {
            // 检查是否有实体引用
            // 不是 即终止
            if (!is_entity_ref(p)) break;
        }
        p++;
    }

    return check_end(parser);
}

int parse_attribute(XmlParser *parser, char *p_text_start)
{
    char *p = parser->workp;
    char name[NAME_MAX_SIZE] = {0}, value[NAME_MAX_SIZE] = {0};
    char *name_begin = p, *name_end = NULL, *tmp;
    char *value_begin;
    int name_len = 0, value_len = 0;
    char next_char;

    while (*p != 0)
    {
        if (isValidNameChar(*p)) p++;
        else if (*p == '=')
        {
            // 说明 = 前没有空格
            if (name_end == NULL)
            {
                name_end = p;
            }
            name_len = name_end - name_begin;
            if (name_len >= NAME_MAX_SIZE)
            {
                print("attr name %20s... is too long\n", name_begin);
                return terminate(parser, name_begin);
            }
            memcpy(name, name_begin, name_len);
            name[name_len] = 0;

            // 寻找value
            p = skip_space(p + 1);
            // value 值开始
            if (*p == '\"')
            {
                value_begin = ++p;
                while (*p != 0)
                {
                    if (isValidNameChar(*p)) p++;
                        // 第2个" 找到value值
                    else if (*p == '\"')
                    {
                        value_len = p - value_begin;
                        if (value_len >= NAME_MAX_SIZE)
                        {
                            print("attr value %20s... is too long\n", value_begin);
                            return terminate(parser, value_begin);
                        }
                        memcpy(value, value_begin, value_len);
                        value[value_len] = 0;
                        // 判断当前name是否有重复 并将name value
                        // 加入到属性链表中
                        if (!add_attr_value_list(parser, name, value))
                        {
                            print("name:%s is repeat!\n", name);
                            return terminate(parser, name_begin);
                        }
                        p++;
NEXT_CHAR:
                        // value 之后的字符 如果是空格 可能还有属性
                        next_char = *p;
                        if (next_char == ' ')
                        {
                            p = skip_space(p + 1);
                            if (isValidAttrStartChar(*p))
                            {
                                parser->workp = p;
                                return parse_attribute(parser, p_text_start);
                            }
                            else
                            {
                                goto NEXT_CHAR;
                            }
                        }
                        else if (next_char == '>')
                        {
                            set_text(parser, p_text_start, p - p_text_start + 1);
                            // start tag 结束
                            parser->workp = p + 1;
                            // tagName入栈 在解析到end tag时 跟栈顶的name比较
                            // 如果一致 node的开闭是正确的
                            Data data;
                            memset(&data, 0, sizeof (Data));
                            memcpy(data.string, parser->tagName, strlen(parser->tagName));
                            push(parser->names_stack, &data);
                            return START_TAG;
                        }
                        else if (next_char == '/')
                        {
                            // 不是 "/>" 解析终止 
                            if (*++p != '>') break;
                            else
                            {
                                set_text(parser, p_text_start, p - p_text_start + 1);
                                parser->workp = p + 1;
                                return START_TAG;
                            }
                        }
                        else
                        {
                            return terminate(parser, p);
                        }
                    }
                    else
                    {
                        return terminate(parser, p);
                    }
                }
            }
            else
            {
                return terminate(parser, p);
            }
        }
        else if (*p == ' ')
        {
            tmp = p;
            p = skip_space(p + 1);
            // 跳过空格后不是= 解析终止
            if (*p != '=') break;
            else
            {
                // 保存name 终止指针
                name_end = tmp;
            }
        }
        else break;
    }

    return terminate(parser, p);
}

char *getName(XmlParser *parser)
{
    return parser->tagName;
}

char *getText(XmlParser *parser)
{
    return parser->text;
}

void xmlFree(XmlParser *parser)
{
    // 释放属性链表
    clear_attr(parser);
    xml_free(parser->names_stack);
    xml_free(parser->text);
    xml_free(parser->nextText);
    xml_free(parser);
}

int terminate(XmlParser *parser, char *last_position)
{
    if (strlen(last_position) > 20)
    {
        print("error position:%-20.20s...\n", last_position);
    }
    else
    {
        print("error position:%s\n", last_position);
    }
    parser->eventType = END_DOCUMENT;
    return END_DOCUMENT;
}

void set_text(XmlParser *parser, char *begin, int size)
{
    if (size >= DEFAULT_TEXT_SIZE)
    {
        xml_free(parser->text);
        parser->text = xml_malloc(size + 1);
    }
    memcpy(parser->text, begin, size);
    parser->text[size] = 0;
}

void set_next_text(XmlParser *parser, char *begin, int size)
{
    if (size >= DEFAULT_TEXT_SIZE)
    {
        xml_free(parser->nextText);
        parser->nextText = xml_malloc(size + 1);
    }
    memcpy(parser->nextText, begin, size);
    parser->nextText[size] = 0;
}

bool add_attr_value_list(XmlParser *parser, char *name, char *value)
{
    Pair *pre = &parser->head_pair;
    Pair *pair = pre->next;

    while (pair)
    {
        if (!strcmp(name, pair->name)) return false;
        pre = pair;
        pair = pair->next;
    }

    // 没重复name 添加到链表
    Pair *new_pair = (Pair *) xml_malloc(sizeof (Pair));
    memset(new_pair, 0, sizeof (Pair));
    strcpy(new_pair->name, name);
    strcpy(new_pair->value, value);
    new_pair->next = NULL;

    // 将新节点加入到链尾
    pre->next = new_pair;

    return true;
}

bool is_entity_ref(char *string)
{
    char *entity_refs[5] = {
        "&lt;", "&gt;", "&quot;", "&apos;", "&amp;"
    };

    int i = 0;
    for (; i < 5; i++)
    {
        if (!strcmp_part(string, entity_refs[i]))
            return true;
    }

    return false;
}

/**
 * 校验是否正常结束
 * @param parser
 * @return 
 */
int check_end(XmlParser *parser)
{
    // 判断start tag 栈是否为空
    if (parser->names_stack->size == 0)
    {
        parser->eventType = END_DOCUMENT;
    }
    else
    {
        // 取栈顶数据
        Data data = pop(parser->names_stack);
        print("has no end node: %s\n", data.string);
    }
    return END_DOCUMENT;
}

void clear_attr(XmlParser *xml_parser)
{
    Pair *pair = xml_parser->head_pair.next;
    Pair *next;
    while (pair)
    {
        next = pair->next;
        memset(pair, 0, sizeof (Pair));
        xml_free(pair);
        pair = next;
    }

    memset(&xml_parser->head_pair, 0, sizeof (Pair));
}
