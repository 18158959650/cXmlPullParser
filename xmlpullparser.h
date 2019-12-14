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
#define NODE_TEXT_MAX_SIZE 1024
#define MAX_DEPTH 5

#ifdef __cplusplus
extern "C" {
#endif

    enum _event_type {
        NONE,
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
        char name[NAME_MAX_SIZE + 1];
        char value[DEFAULT_TEXT_SIZE + 1];
        struct _pair *next;
    } Pair;

    typedef struct {
        int eventType;
        int next_eventType;
        // 节点名称栈 
        Stack *names_stack;
        char tagName[NAME_MAX_SIZE + 1];
        char *text;
        char *nextText;
        Pair head_pair;
        // 指向当前已解析到的字符
        char *workp;
        // 数据源指针
        char *string;
    } XmlParser;

    XmlParser *xml_parser_new(char *read_buffer);
    int getStartEventType(XmlParser *parser);
    int getXmlEventType(XmlParser *parser);
    int getNext(XmlParser *parser);
    void xml_parser_free(XmlParser *parser);
    int terminate(XmlParser *parser, char *last_position);

    typedef struct _xmlnode {
        char tagName[NAME_MAX_SIZE + 1];
        // 属性链表
        Pair head_pair;
        // 除起止tag之外的所有文本
        char *text;
        int text_len;
        // 是否有子节点
        bool has_child;
        int depth;
    } XmlNode;

    int *xml_node_print(XmlNode *node, char *buffer);
    XmlNode xml_element_create(char *name, char *text);
    XmlNode xml_node_create(char *name, int size, int depth);
    bool xml_node_add_attr(XmlNode *node, char *attr_name, char *attr_value);
    int xml_add_noattr_element(XmlNode *parent_node, char *name, char *text);
    void xml_node_free(XmlNode node);
    int xml_add_node_to_parent(XmlNode *child, XmlNode *parent);
    void xml_set_print_format(bool fmt);

#ifdef __cplusplus
}
#endif

#endif /* XMLPULLPARSER_H */