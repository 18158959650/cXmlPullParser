# cXmlPullParser
XmlPullParser C版本

#usage

```
#include <stdio.h>
#include <stdlib.h>
#include "xmlpullparser.h"

void processNode(XmlParser *xpp, char *node_type);

int main(int argc, char** argv)
{
    char *sample_xml =
            "<?xml version=\"1.0\"?>\n"
            "\n"
            "<poem a= \"1\" b= \"2\" c=\"123\" >"
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
            processNode(xml_parser, "start");
            break;
        case END_TAG:
            processNode(xml_parser, "end");
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

void processNode(XmlParser *xml_parser, char *node_type)
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
```