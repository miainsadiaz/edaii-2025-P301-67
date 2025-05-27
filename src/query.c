#include "query.h"
#include "document.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_QUERIES 3

QueryQueue *init_query_queue() {
    QueryQueue *queue = (QueryQueue *)malloc(sizeof(QueryQueue));
    if (queue == NULL) {
        fprintf(stderr, "Error allocating memory for query queue\n");
        return NULL;
    }
    
    queue->front = 0;
    queue->rear = -1;
    for (int i = 0; i < MAX_QUERIES; i++) {
        queue->queries[i] = NULL;
    }
    return queue;
}

void enqueue_query(QueryQueue *queue, Query *query) {
    if (queue == NULL || query == NULL) return;

    // Increment rear circularly
    queue->rear = (queue->rear + 1) % MAX_QUERIES;

    // Free old query if overwriting
    if (queue->queries[queue->rear] != NULL) {
        free_query(queue->queries[queue->rear]);
    }

    queue->queries[queue->rear] = query;

    // Adjust front if queue is full
    if (queue->rear == (queue->front - 1 + MAX_QUERIES) % MAX_QUERIES) {
        queue->front = (queue->front + 1) % MAX_QUERIES;
    }
}

void print_last_queries(QueryQueue *queue) {
    if (queue == NULL) return;

    printf("\n******* recent searches ********\n");

    int i = queue->front;
    int count = 0;

    while (count < MAX_QUERIES && queue->queries[i] != NULL) {
        printf("* ");

        QueryItem *item = queue->queries[i]->head;
        while (item != NULL) {
            if (item->is_exclusion) printf("!");
            printf("%s", item->word);
            if (item->next != NULL) printf(" ");
            item = item->next;
        }

        printf(" *\n");
        i = (i + 1) % MAX_QUERIES;
        count++;
    }

    printf("********************************\n");
}


void normalize_word(char*word){
    //elimina la puntuacion inciial moviendo la palabra hacia la izq
    while(*word&&ispunct((unsigned char)*word)){ //con ispunct se verifica si ese caracter es un signo de puntuacion
    memmove(word,word+1,strlen(word)); //así se mueve toda la cadena una posicion a la izq
        //mmove lo qe hace es mover la cadena de caracteres a la izquierda, así se elimina el primer carácter
    }
    
    //elimina la putnuacion final
    char *end=word+strlen(word)-1;
    while(end>word&&ispunct((unsigned char)*end)){
        *end='\0';
        end--;
    }
        // converteix a minúscula
        for (char *p = word; *p; ++p) {
        *p = tolower((unsigned char)*p);  
}

}
    


Query *parseQuery(const char *input) {
    if (input == NULL) return NULL;

    Query *query = (Query *)malloc(sizeof(Query));
    if (query == NULL) {
        fprintf(stderr, "Error allocating memory for query\n");
        return NULL;
    }
    query->head = NULL;

    char *copy = strdup(input);
    if (copy == NULL) {
        free(query);
        return NULL;
    }

    char *token = strtok(copy, " ");
    QueryItem *last = NULL;

    while (token != NULL) {
        QueryItem *item = (QueryItem *)malloc(sizeof(QueryItem));
        if (item == NULL) {
            free_query(query);
            free(copy);
            return NULL;
        }

        item->is_exclusion = (token[0] == '!');
        item->word = strdup(item->is_exclusion ? token + 1 : token);
        if (item->word == NULL) {
            free(item);
            free_query(query);
            free(copy);
            return NULL;
        }

        item->next = NULL;

        if (query->head == NULL) {
            query->head = item;
        } else {
            last->next = item;
        }
        last = item;

        token = strtok(NULL, " ");
    }

    free(copy);
    return query;
}

void free_query_queue(QueryQueue *queue) {
    if (queue == NULL) return;

    for (int i = 0; i < MAX_QUERIES; i++) {
        if (queue->queries[i] != NULL) {
            free_query(queue->queries[i]);
        }
    }
    free(queue);
}

bool contains_word(const char *text, const char *word) {
    if (text == NULL || word == NULL) return false;
    return strstr(text, word) != NULL;
}

DocumentNode *filterDocuments(DocumentNode *docs, Query *query) {
    if (docs == NULL || query == NULL) return NULL;

    DocumentNode *result = NULL;
    DocumentNode *last = NULL;
    int count = 0;

    for (DocumentNode *cur = docs; cur != NULL && count < 5; cur = cur->next) {
        bool valid = true;

        for (QueryItem *item = query->head; item != NULL; item = item->next) {
            bool found = contains_word(cur->doc->body, item->word);

            if ((item->is_exclusion && found) || (!item->is_exclusion && !found)) {
                valid = false;
                break;
            }
        }

        if (valid) {
            DocumentNode *new_node = malloc(sizeof(DocumentNode));
            if (new_node == NULL) {
                fprintf(stderr, "Error allocating memory for document node\n");
                break;
            }

            new_node->doc = cur->doc;
            new_node->next = NULL;

            if (result == NULL) {
                result = new_node;
            } else {
                last->next = new_node;
            }
            last = new_node;
            count++;
        }
    }

    return result;
}

void free_query(Query *query) {
    if (query == NULL) return;

    QueryItem *current = query->head;
    while (current != NULL) {
        QueryItem *next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
    free(query);
}