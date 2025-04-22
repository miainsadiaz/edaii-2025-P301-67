#include <stdlib.h>   //pel malloc
#include "link.h"

/* LAB 1
   Carregar documents des de fitxers, mostrar-los per pantalla,
   deixar que l’usuari en triï un, mostrar-ne el contingut,
   i assegurar-te que el teu codi és net i ben provat.
*/

Link* crear_link(int id)
{
    Link *nou = malloc(sizeof(Link));
    if (nou == NULL) {
        return NULL;      
    }

    nou->id   = id;
    nou->next = NULL;
    nou->prev = NULL;   

    return nou;
}
