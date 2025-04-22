
#ifndef LINK_H          // evita inclusions múltiples
#define LINK_H

typedef struct Link {
    int  id;
    struct Link *next;
    struct Link *prev;
} Link;

typedef struct {
    Link *head;
} LinkList;

#endif /* LINK_H */


