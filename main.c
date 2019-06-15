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

void processStartElement(XmlParser *xpp);
void processEndElement(XmlParser *xpp);

int main(int argc, char** argv)
{
    char *sample_xml =
            "<?xml version=\"1.0\"?>\n"
            "\n"
            "<poem a= \"1\" b= \"2\" >"
            "<title>Roses are Red</title>\n"
    //"<ll/>"    
    "<l>Roses are red,</l>\n"
    "</poem>";
    //"<l>Violets are blue;</l>\n"
    //"<l>Sugar is sweet,</l>\n"
    //"<l>And I love you.</l>\n";
    //"</poem>";

    XmlParser *xml_parser = newXmlParser(sample_xml);
    int eventType = getStartEventType(xml_parser);

    while (eventType != END_DOCUMENT)
    {
        switch (eventType)
        {
        case START_DOCUMENT:
            printf("current eventType:START_DOCUMENT\n");
            break;
        case START_TAG:
            processStartElement(xml_parser);
            break;
        case END_TAG:
            processEndElement(xml_parser);
            break;
        case END_DOCUMENT:
            printf("current eventType:END_DOCUMENT\n");
            break;
        case TEXT:
            printf("TEXT: %s\n\n", xml_parser->text);
            break;
        default:
            break;
        }

        eventType = getNext(xml_parser);
    }

    xmlFree(xml_parser);

    return (EXIT_SUCCESS);
}

void processStartElement(XmlParser *xml_parser)
{
    printf("start tag: %s\n", xml_parser->tagName);
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

void processEndElement(XmlParser *xpp)
{
    printf("end tag:%s\n", xpp->tagName);
    printf("text:%s\n\n", xpp->text);
}
