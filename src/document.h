#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "link.h" 

// Forward declaration de la struct Document (igual q a link) per declarar punters a "Document" abans que el tipus estigui completament definit
typedef struct Document Document;

// estructura "Document"
struct Document {
    char* title;     
    int id;          
    Link* links;      
    float relevance;  
    char* body;       
    Document* next;   
};

// llista de documents
typedef struct DocumentsList {
    Document* head;   // punter al primer document de la llista (si és NULL, la llista està buida)
} DocumentsList;

Document* document_desserialize(const char* filepath);

#endif

