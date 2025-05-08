#ifndef QUERY_H
#define QUERY_H

#include "document.h"
#include <stdbool.h>

typedef struct QueryItem {
  char *word;
  bool is_exclusion;
  struct QueryItem *next;
} QueryItem;

typedef struct Query {
  QueryItem *head;
} Query;

typedef struct QueryQueue {
  Query *queries[3];
  int front, rear;
} QueryQueue;

Query *parseQuery(const char *input);
bool contains_word(const char *text, const char *word);
DocumentNode *filterDocuments(DocumentNode *docs, Query *query);
void free_query(Query *query);
QueryQueue *init_query_queue();
void enqueue_query(QueryQueue *queue, Query *query);
void print_last_queries(QueryQueue *queue);
void free_query_queue(QueryQueue *queue);

#endif // QUERY_H
