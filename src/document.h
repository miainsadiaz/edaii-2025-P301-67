#include "link.h"

typedef struct{
    char* title;
    int id;
    Link* links;
    float relevance;
    char* body;
    Document* next;
}Document;


typedef struct DocumentsList {
    Document* head;
} DocumentsList;