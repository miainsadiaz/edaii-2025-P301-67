#include "document.h"
#include "query.h"
#include "sample_lib.h"
#include "hashmap.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h> 
#include "grafs.h"

void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}


int main() {
  printf("*****************************\nWelcome to the Search Engine!\n*****************************\n");
  printf("\n");

  // Inicialitza el graf amb documents
  DocumentNode *docs = loadAllDocuments("./datasets/wikipedia12");
  if (!docs) {
      printf("Error loading documents\n");
      return 1;
  }

  
  DocumentGraph graph;
  init_graph(&graph, docs);

  // Calcula rellevància per indegree
  calcular_relevancia_indegree(&graph);

  // Ordena documents per rellevància (decreixent)
  ordenar_per_relevancia(&docs);

  /*
  // Imprimeix documents amb rellevància
  DocumentNode *curr = docs;
  while (curr != NULL) {
      printf("Doc: %s (ID %d) - Relevance: %.2f\n", curr->doc->title, curr->doc->id, curr->doc->relevance);
      curr = curr->next;
  } //--> AIXO NO SE SI CAAL !!
   */

  // QUERY
  QueryQueue *query_history = init_query_queue();
  char input[256];

  while (1) {
    printf("\n");
    printf("Enter query (or empty to quit): ");
    fgets(input, 256, stdin);

    if (input[0] == '\n') break;

    input[strcspn(input, "\n")] = '\0'; // elimina \n final

    Query *query = parseQuery(input);
    DocumentNode *results = filterDocuments(docs, query);

    print_search_results(query, results);
    select_document_and_print(results);

    enqueue_query(query_history, query);
    print_last_queries(query_history);

    // neteja memòria temporal
    DocumentNode *temp;
    while (results != NULL) {
        temp = results;
        results = results->next;
        free(temp);
    }
  }

 /*
  // HASHMAP
HashMap reverse_map;
init_hashmap(&reverse_map);

construir_reverse_index(&reverse_map, docs);

while(1){
    char input[256];
    printf("Enter word to search (or empty to quit): ");
    fgets(input, 256, stdin);

    if (input[0] == '\n')
      break;

    input[strcspn(input, "\n")] = '\0';  // elimina salt de línia

    normalize_word(input);              // normalitza la paraula (important)

    search_by_word(&reverse_map, input);
}*/


  free_documents_list(docs);
  free_query_queue(query_history);

  return 0;
}

