/* 
 * File:   xmlpullparser.h
 * Author: zhanglei
 *
 * Created on 2019年5月21日, 下午3:18
 */

#ifndef XMLPULLPARSER_H
#define XMLPULLPARSER_H

#include "common.h"
#include "Stack.h"

#define DEFAULT_TEXT_SIZE 50
#define NAME_MAX_SIZE 50

#ifdef __cplusplus
extern "C" {
#endif

    enum _event_type {
        START_DOCUMENT,
        END_DOCUMENT,
        START_TAG,
        END_TAG,
        TEXT,
        CDSECT,
        ENTITY_REF,
        IGNORABLE_WHITESPACE,
        PROCESSING_INSTRUCTION,
        COMMENT,
        DOCDECL
    };
    
    /**
     * 元素属性键值对
     */
    typedef struct _pair {
        char name[50];
        char value[50];
        struct _pair *next;
    } Pair;
    
    typedef struct {
        int eventType;
        // 节点名称栈 
        Stack *names_stack;
        char tagName[NAME_MAX_SIZE];
        char *text;
        char *nextText;
        Pair head_pair;
        // 指向当前已解析到的字符
        char *workp;
        // 数据源指针
        char *string;
    } XmlParser;
    
    XmlParser *newXmlParser(char *read_buffer);
    int getStartEventType(XmlParser *parser);
    int getXmlEventType(XmlParser *parser);
    int getNext(XmlParser *parser);
    void xmlFree(XmlParser *parser);
    int terminate(XmlParser *parser, char *last_position);

#ifdef __cplusplus
}
#endif

#endif /* XMLPULLPARSER_H */