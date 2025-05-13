#include "document.h"
#include "query.h"
#include "sample_lib.h"
#include "hashmap.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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

  //HASHMAP
  HashMap reverse_map;
  init_hashmap(&reverse_map);
  DocumentNode *docs = loadAllDocuments("./datasets/wikipedia12");
  construir_reverse_index(&reverse_map, docs);
  while(1){
    char input[256];
    printf("Enter word to search (or empty to quit): ");
    fgets(input, 256, stdin);

    if (input[0] == '\n')
      break;

    input[strcspn(input, "\n")] = '\0';

    search_by_word(&reverse_map, input);
  }
  free_documents_list(docs);
  free_hashmap(&reverse_map);
  return 0;
  
}