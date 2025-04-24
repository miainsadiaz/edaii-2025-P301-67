/* LAB 1
Carregar documents des de fitxers, mostrar-los per pantalla, 
deixar que l’usuari en triï un, mostrar-ne el contingut, 
i assegurar-te que el teu codi és net i ben provat.
*/

#include "link.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


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
//alliberar memoria de les linked lists dins de docuemnt
void free_links(Link *head){
    while(head != NULL){
        Link *tmp=head;
        head=head->next;
        free(tmp);
    }
}
