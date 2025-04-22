#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "link.h"   /* perquè fem servir Link* dins Document */


typedef struct Document {
    char   *title;      
    int     id;         
    Link   *links;      
    float   relevance; 
    char   *body;       
    struct Document *next; 
} Document;

typedef struct {
    Document *head;     /* primer element; NULL si la llista és buida */
} DocumentsList;

#endif /* DOCUMENT_H */
