#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <assert.h>
#include "document.h"


//crear node llista
Link *crear_link(int id){
    Link *new_link = malloc(sizeof(Link));
    if (new_link == NULL) {
        return NULL;
    }
    new_link->id = id;
    new_link->next = NULL;
    new_link->prev = NULL;
    return new_link;
}
//links init
Link *LinksInit(){
    return NULL;
}

//afegir node al final de la llista
void afegir_link(Link **head, int id){
    Link *new_link = crear_link(id);
    Link *temp = *head;

    if(*head == NULL){
        new_link->prev = NULL;
        *head = new_link;
        return;
    }
    while(temp->next != NULL){
        temp = temp->next;
    }

    temp->next = new_link;
    new_link->prev = temp;
}

//afegir node al principi de la llista
void afegir_link_al_principi(Link **head, int id){
    Link *new_link = crear_link(id);
    new_link->next = *head;
    new_link->prev = NULL;

    if(*head != NULL){
        (*head)->prev = new_link;
    }

    *head = new_link;

}

//afegir node a la posicio indicada
void afegir_link_pos(Link *prev_link, int id){
    if(prev_link == NULL){
        return;
    }
    Link *new_link = crear_link(id);
    new_link->next = prev_link->next;
    prev_link->next = new_link;
    new_link->prev = prev_link;

    if(new_link->next != NULL){
        new_link->next->prev = new_link;
    }
}

void alliberar_links(Link *head){
    Link *temp;
    while(head != NULL){
        temp = head;
        head = head->next;
        free(temp);
    }
}

//eliminar el primer node de la llista
void eliminar_primer_link(Link** head){
    if(*head == NULL){
        return;
    }

    Link *temp = *head;
    *head = (*head)->next;

    if(*head != NULL){
        (*head)->prev = NULL;
    }

    free(temp);
}

//eliminar l'ultim node de la llista
void eliminar_ultim_link(Link** head){
    if(*head == NULL){
        return;
    }

    Link *temp = *head;

    while(temp->next != NULL){
        temp = temp->next;
    }

    if(temp->prev != NULL){
        temp->prev->next = NULL;
    } else {
        *head = NULL;
    }

    free(temp);
}

//eliminar node de la llista d'una posicio indicada
void eliminar_link_pos(Link** head, int pos){
    if(*head==NULL){
        return;
    }

    Link *temp = *head;
    for(int i=0; i<pos && temp!=NULL; i++){
        temp = temp->next;
    }
    if(temp == NULL){
        return;
    }
    if(temp== *head){
        *head = temp->next;
        if(*head != NULL){
            (*head)->prev = NULL;
        }
    }
    else{
        if(temp->prev != NULL){
            temp->prev->next = temp->next;
        }
        if(temp->next != NULL){
            temp->next->prev = temp->prev;
        }
    }

    free(temp);
}


void print_link_list(Link *head) {
    Link *current = head;
    while (current != NULL) {
        printf("Link ID: %d\n", current->id);
        current = current->next;
    }
}

Document *document_desserialize(char *path) {
    FILE *f = fopen(path, "r");
    assert(f != NULL);
    
    Document *document = (Document *)malloc(sizeof(Document));
    
    char buffer[262144];
    int bufferSize = 262144;
    int bufferIdx = 0;
    char ch;

    // parse id
    while ((ch = fgetc(f)) != '\n') {
        assert(bufferIdx < bufferSize);
        buffer[bufferIdx++] = ch;
    }
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = '\0';
    document->id = atoi(buffer);

    // parse title
    bufferIdx = 0;
    while ((ch = fgetc(f)) != '\n') {
        assert(bufferIdx < bufferSize);
        buffer[bufferIdx++] = ch;
    }
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = '\0';
    document->title = strdup(buffer);

    // parse body
    char linkBuffer[64];
    int linkBufferSize = 64;
    int linkBufferIdx = 0;
    bool parsingLink = false;
    Link *links = LinksInit();
    
    bufferIdx = 0;
    while ((ch = fgetc(f)) != EOF) {
        assert(bufferIdx < bufferSize);
        buffer[bufferIdx++] = ch;
        if (parsingLink) {
            if (ch == ')') { // end of link
                parsingLink = false;
                assert(linkBufferIdx < linkBufferSize);
                linkBuffer[linkBufferIdx++] = '\0';
                int linkId = atoi(linkBuffer);

                // TODO add to links
                afegir_link(&links, linkId);

                linkBufferIdx = 0;
            } else if (ch != '(') { // skip first parenthesis of the link
                assert(linkBufferIdx < linkBufferSize);
                linkBuffer[linkBufferIdx++] = ch;
            } 
        } else if (ch == ']') { // found beginning of link id, e.g.: [my link text](123)
          parsingLink = true;
        }
    }
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = '\0';
    
    char *body = (char *)malloc(sizeof(char) * bufferIdx);
    strcpy(body, buffer);

    // TODO
    document->body = body;
    document->links = links;

    fclose(f);
    return document;
}



