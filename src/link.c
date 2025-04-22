#include "link.h"
#include <stdlib.h> //per poder usar malloc

// funció per crear un nou Link (node)


/*LAB1: Aquesta funció crea un enllaç nou de tipus Link i l'assigna un ID. 
A més, inicialitza el punter next com a NULL, indicant que aquest enllaç 
no té més nodes vinculats.*/


Link* crear_link(int id) {
    Link* nou = malloc(sizeof(Link));
    if (nou == NULL){
        return NULL;}
    nou->id = id;
    nou->next = NULL;
    return nou;
}

