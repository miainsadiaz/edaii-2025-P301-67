#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "document.h"

//node
typedef struct HNode {
    char *key;
    DocumentNode *document;
    struct HNode *next;
} HNode;

//hashmap
typedef struct HashMap{
    HNode *array[100];
}HashMap;

HNode *create_node(const char *key, Document *document);
void init_hashmap(HashMap *map);
void put(HashMap *map, const char *key, Document *document);
Document *get(HashMap *map, const char *key);
void remove_key(HashMap *map, const char *key);
void free_hashmap(HashMap *map);
unsigned int hash(const char *key);
void search_by_word(HashMap *map, const char *word);
bool alafanumeric(char c);
char to_min(char c);
void construir_reverse_index(HashMap *map, DocumentNode *docs);



