#include "hashmap.h"


//crear node (key + document)
HNode *create_node(const char *key, Document *document) {       
    HNode *new_node = (HNode *)malloc(sizeof(HNode));           // reservem memòria pel node
    new_node->key = strdup(key);        // copiem la clau
    DocumentNode *doc_list = (DocumentNode *)malloc(sizeof(DocumentNode));  // creem un node de document i l'assignem al node del hashmap
    doc_list->doc = document;
    doc_list->next = NULL;

    new_node->document = doc_list; // inicialitzar document
    new_node->next = NULL; // inicialitzar següent node
    return new_node;
}

//inicialitzar hashmap
void init_hashmap(HashMap *map) {
    for (int i = 0; i < 100; i++) {
        map->array[i] = NULL;
    }
}

//insertar o actualitzar clau
void put(HashMap *map, const char *key, Document *document) {
    unsigned int index = hash(key); //obtenim la posició
    HNode *current = map->array[index]; 

    while(current){
        if(strcmp(current->key, key) == 0){ //si la clau ja existeix, actualitzem
            DocumentNode *dl = current->document;
            while(dl){
                if(dl->doc == document){ //si el document ja existeix
                    return; //no fem res
                }
                dl = dl->next;
            }

            //si no hi és el document, afegim-lo
            DocumentNode *new_node = malloc(sizeof(DocumentNode));
            new_node->doc = document;
            new_node->next = current->document;
            current->document = new_node; //afegim el document a la llista
            return;
        }
        current = current->next;
    }

    //si la clau no existeix, creem
    HNode *new_node = create_node(key, document);
    new_node->next = map->array[index];
    map->array[index] = new_node;
}

//obtenir un valor per la clau
Document *get(HashMap *map, const char *key) {
    unsigned int index = hash(key);
    HNode *current = map->array[index];

    while (current) {
        if (strcmp(current->key, key) == 0) { //si la clau existeix
            return current->document->doc; //retornem el document
        }
        current = current->next;
    }
    return NULL; //si no existeix la clau
}

//eliminar clau
void remove_key(HashMap *map, const char *key) {
    unsigned int index = hash(key);
    HNode *current = map->array[index];
    HNode *prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) { //si la clau existeix
            if (prev) {
                prev->next = current->next; //eliminar node
            } else {
                map->array[index] = current->next; //eliminar node
            }
            //alliberem
            free(current->key); 
            free(current->document);    
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

//alliberar hashmap
void free_hashmap(HashMap *map) {
    for (int i = 0; i < 100; i++) {
        HNode *current = map->array[i];
        while (current) {
            HNode *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->document);
            free(temp);
        }
    }
}

//funcio hash
unsigned int hash(const char *key) {
    unsigned int hash = 5381;
    int c;

    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % 100; //modul per limitar a 100
}

void search_by_word(HashMap *map, const char *word){
    unsigned int index = hash(word);
    HNode *current = map->array[index];

    while (current) {
        if (strcmp(current->key, word) == 0) { //si la clau existeix
            printf("Documents que contenen la paraula %s:\n", word);
            DocumentNode *doc_list = current->document;
            while (doc_list) {
                print_document(doc_list->doc); //imprimim el document
                doc_list = doc_list->next; //passem al següent document
            }
            return;
        }
        current = current->next;
    }
    printf("No s'ha trobat la paraula %s\n", word);
}



//funcions pel reverse index
bool alafanumeric(char c){
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');  //comprovem si és alfanumèric
}

char to_min(char c){
    if(c >= 'A' && c <= 'Z'){
        return c + ('a' - 'A'); //convertim a minúscula
    }
    return c;
}

void construir_reverse_index(HashMap *map, DocumentNode *docs) {    //assigna cada paraula als documents que la contenen
    DocumentNode *current = docs;
    while (current) {
        Document *doc = current->doc;
        char *body = strdup(doc->body); // copiem el cos del document

        int len = strlen(body);
        char word[256]; // array per guardar les paraules
        int word_count = 0;

        for (int i = 0; i <= len; i++) {
            if (alafanumeric(body[i])) {
                word[word_count++] = to_min(body[i]); // convertim a minúscula
            } else {
                if (word_count > 0) {
                    word[word_count] = '\0'; // tanquem la paraula
                    normalize_word(word);
                    put(map, word, doc); // afegim al hashmap
                    word_count = 0; // reiniciem el contador de paraules
                }
            }
        }
        free(body); // alliberem la memòria
        current = current->next; // passem al següent document
    }
}
