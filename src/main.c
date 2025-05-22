#include "document.h"
#include "query.h"
#include "sample_lib.h"
#include "hashmap.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "grafs.h"
#include <ctype.h>  // per tolower()

void to_lowercase(char *str) {
    for (; *str; ++str) {
        *str = tolower((unsigned char)*str);
    }
}


void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

int main() {
  /*
  printf("*****************\nWelcome to EDA 2!\n*****************\n");
  printf("Factorial of 4 is %d\n", fact(4));

  //QUERY
  QueryQueue *query_history = init_query_queue();
  const char *path = "./datasets/wikipedia12";
  DocumentNode *docs = loadAllDocuments(path);

  char input[256];

  while (1) {
    printf("Enter query (or empty to quit): ");
    fgets(input, 256, stdin);

    if (input[0] == '\n')
      break;

    input[strcspn(input, "\n")] = '\0';

    Query *query = parseQuery(input);
    DocumentNode *results = filterDocuments(docs, query);

    DocumentNode *current_result = results;
    while (current_result != NULL) {
      print_document(current_result->doc);
      current_result = current_result->next;
    }

    DocumentNode *temp;
    while (results != NULL) {
      temp = results;
      results = results->next;
      free(temp);
    }

    enqueue_query(query_history, query);
    print_last_queries(query_history);
  }

  free_documents_list(docs);
  free_query_queue(query_history);

  return 0;*/
  // Inicialitza el graf amb documents
  // Carreguem documents i inicialitzem el graf
    DocumentNode *docs = loadAllDocuments("./datasets/wikipedia12");
    if (!docs) {
        printf("Error loading documents\n");
        return 1;
    }

    DocumentGraph graph;
    init_graph(&graph, docs);
    calcular_relevancia_indegree(&graph);
    ordenar_per_relevancia(&docs);

    DocumentNode *curr = docs;
    while (curr != NULL) {
        printf("Doc: %s (ID %d) - Relevance: %.2f\n",
               curr->doc->title, curr->doc->id, curr->doc->relevance);
        curr = curr->next;
    }

    HashMap reverse_map;
    init_hashmap(&reverse_map);
    construir_reverse_index(&reverse_map, docs);

    char input[256];
    while (1) {
        printf("Enter word to search (or empty to quit): ");
        if (!fgets(input, 256, stdin) || input[0] == '\n') break;

        input[strcspn(input, "\n")] = '\0';
        to_lowercase(input);

        search_by_word(&reverse_map, input);
    }

    free_documents_list(docs);
    free_hashmap(&reverse_map);

    return 0;
}