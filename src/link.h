#ifndef LINK_H
#define LINK_H

// Forward declaration del tipus "Link", per declarar punters a "Link" abans que el tipus estigui completament definit
typedef struct Link Link;

// node d'una llista enllaçada simple.
struct Link {
    int id;         // identificador del link 
    Link* next;     // punter al següent node de la llista (o NULL si és l'últim)
};


typedef struct {
    Link *head;     // Punter al primer node (Link) de la llista. Si la llista és buida, val NULL.
} LinkList;

Link* crear_link(int id); //declarem la funció 


#endif


