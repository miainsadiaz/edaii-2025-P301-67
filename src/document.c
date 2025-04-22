#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "document.h"

/*1 Document* create_document_from_file(const char* filepath, int id);
Llegeix un fitxer

Retorna un Document* creat

2 void add_document_to_list(DocumentsList* list, Document* doc);
Afegeix un Document* a la llista DocumentsList

3 void load_documents_from_folder(DocumentsList* list, const char* folder);
Obre el directori i, per cada fitxer, crida les dues funcions de dalt

*/


Document* create_document_from_file(const char* nom_document, int id) {
FILE* file= fopen(nom_document, "r");
if(file==NULL){
    printf("error obrint fitxer");
    return NULL;
}

Document document;

}



