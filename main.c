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

/* 
 * File:   main.c
 * Author: zhanglei
 *
 * Created on 2019年5月21日, 下午3:04
 */

#include <stdio.h>
#include <stdlib.h>
#include "xmlpullparser.h"

void test_xml_packet(void);
void test_xml_parser(void);
void process_node(XmlParser *xpp, char *node_type);

int main(int argc, char** argv)
{
    //test_xml_parser();
    test_xml_packet();
    //printf("<xml>\n\t<node1>\n\t\t<node2>");
    return (EXIT_SUCCESS);
}

void test_xml_packet(void)
{
    XmlNode root_node = xml_node_create("xml", 500, 1);

    //xml_set_print_format(false);

    //xml_node_add_attr(&root_node, "version", "1.0");
    //xml_node_add_attr(&root_node, "attr1", "123444");

    XmlNode node1 = xml_node_create("node1", 200, 2);
    xml_node_add_attr(&node1, "attr1", "123");
    xml_node_add_attr(&node1, "attr2", "456");

    //xml_add_noattr_element(&node1, "element1", "11111111");
    XmlNode element1 = xml_element_create("element1", "11111111");
    xml_node_add_attr(&element1, "attr1", "123");
    xml_add_node_to_parent(&element1, &node1);

    xml_add_noattr_element(&node1, "element2", "22222222");

    XmlNode node11 = xml_node_create("node11", 200, 3);
    xml_add_noattr_element(&node11, "element1", "11111111");
    xml_add_node_to_parent(&node11, &node1);

    xml_add_node_to_parent(&node1, &root_node);

    char print_buffer[500] = {0};
    xml_node_print(&root_node, print_buffer);
    printf(print_buffer);

    xml_node_free(root_node);
    xml_node_free(node1);
}

void test_xml_parser(void)
{
    char *sample_xml =
            "<?xml version=\"1.0\"?>\n"
            "\n"
            "<poem a= \"1\" b= \"2\" c=\"123\" >"
            "<title>Roses are Red</title>\n"
            //"<ll/>"    
            "<l>Roses are red,</l>\n"
            "<l1><![CDATA[OK1234567]]></l1>"
            "</poem>";
    //"<l>Violets are blue;</l>\n"
    //"<l>Sugar is sweet,</l>\n"
    //"<l>And I love you.</l>\n";
    //"</poem>";

    XmlParser *xml_parser = xml_parser_new(sample_xml);
    int eventType = getStartEventType(xml_parser);

    while (eventType != END_DOCUMENT)
    {
        switch (eventType)
        {
        case START_DOCUMENT:
            printf("current eventType:START_DOCUMENT\n");
            break;
        case START_TAG:
            process_node(xml_parser, "start");
            break;
        case END_TAG:
            process_node(xml_parser, "end");
            break;
        case END_DOCUMENT:
            printf("current eventType:END_DOCUMENT\n");
            break;
        case CDSECT:
            printf("Tag: %s CDSECT DATA: %s\n\n", xml_parser->tagName, xml_parser->text);
            break;
        case TEXT:
            printf("TEXT: %s\n\n", xml_parser->text);
            break;
        default:
            break;
        }

        eventType = getNext(xml_parser);
    }

    xml_parser_free(xml_parser);
}

void process_node(XmlParser *xml_parser, char *node_type)
{
    printf("%s tag: %s\n", node_type, xml_parser->tagName);
    printf("text: %s\n", xml_parser->text);
    // 获取所有属性
    Pair *pair = xml_parser->head_pair.next;
    while (pair)
    {
        printf("attr name = %s, value = %s\n", pair->name, pair->value);
        pair = pair->next;
    }

    char *next_text = nextText(xml_parser);
    if (next_text != NULL)
    {
        printf("nextText: %s\n", next_text);
    }
    printf("\n");
}
