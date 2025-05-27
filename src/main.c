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

void select_document_and_print(DocumentNode *results) {
    if (results == NULL) {
        printf("No documents to select.\n");
        return;
    }

    printf("Data Structures and Algorithms II Ed. 2024/25\n");
    printf("Select document: ");

    int selection;
    if (scanf("%d", &selection) != 1) {
        printf("Invalid input.\n");
        // netejar stdin
        while (getchar() != '\n');
        return;
    }

    // netejar \n restant del buffer
    while (getchar() != '\n');

    int index = 0;
    DocumentNode *current = results;
    while (current != NULL) {
        if (index == selection) {
            printf("ID\n%d\n", current->doc->id);
            printf("TITLE\n%s\n", current->doc->title);
            printf("RELEVANCE SCORE\n%.0f\n", current->doc->relevance);
            printf("BODY\n%s\n", current->doc->body);
            return;
        }
        current = current->next;
        index++;
    }

    printf("Invalid selection. No document at index %d.\n", selection);
}

void print_search_results(Query *query, DocumentNode *results) {
    // Imprimir la consulta
    printf("Search: ");
    QueryItem *item = query->head;
    while (item) {
        if (item->is_exclusion) printf("!");
        printf("%s", item->word);
        if (item->next) printf(" ");
        item = item->next;
    }
    printf("\n");

    // Imprimir documents trobats
    int count = 0;
    DocumentNode *current = results;
    while (current != NULL && count < 5) {
        printf("(%d) %s\n", count, current->doc->title);
        printf("---\n");
        int printed = 0;
        for (char *c = current->doc->body; *c != '\0' && printed < 300; c++, printed++) {
            putchar(*c);
        }
        if (strlen(current->doc->body) > 300) {
            printf("...");
        }
        printf("\n---\n");
        printf("relevance score: %.0f\n", current->doc->relevance);
        current = current->next;
        count++;
    }
    printf("[%d results]\n", count);
    printf("-----------------------------\n");
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

  // Imprimeix documents amb rellevància
  /*DocumentNode *curr = docs;
  while (curr != NULL) {
      printf("Doc: %s (ID %d) - Relevance: %.2f\n", curr->doc->title, curr->doc->id, curr->doc->relevance);
      curr = curr->next;
  }*/ //--> AIXO NO SE SI CAAL !!

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

  free_documents_list(docs);
  free_query_queue(query_history);

  return 0;
}

