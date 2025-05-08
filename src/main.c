#include "document.h"
#include "query.h"
#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

int main() {
  printf("*****************\nWelcome to EDA 2!\n*****************\n");

  // how to import and call a function
  printf("Factorial of 4 is %d\n", fact(4));

  // Imprimir un document

  /*
  Document* doc = document_desserialize("./datasets/wikipedia12/0.txt");

  printf("Document ID: %d\n", doc->id);
  printf("Document Title: %s\n", doc->title);
  printf("Document Body: %s\n", doc->body);
  printf("Document Links:\n");
  Link* current_link = doc->links;
  print_link_list(current_link);

  free_links(doc->links);
  free(doc->title);
  free(doc->body);
  free(doc);*/

  const char *path = "./datasets/wikipedia12";
  DocumentNode *docs = loadAllDocuments(path);

  DocumentNode *current_doc = docs;
  while (current_doc != NULL) {
    print_document(current_doc->doc);
    printf("Document Links:\n");
    Link *current_link = current_doc->doc->links;
    print_link_list(current_link);
    current_doc = current_doc->next;
  }


  char input[256];

  // Carreguem tots els documents al principi
  //DocumentNode *docs = loadAllDocuments("./datasets/wikipedia12");

  while (1) {
    printf("Enter query (or empty to quit): ");
    fgets(input, 256, stdin);

    if (input[0] == '\n')
      break; // Si l’usuari prem enter, sortim

    input[strcspn(input, "\n")] = '\0'; // Eliminem el \n final

    // Convertim el text en estructura Query
    Query *query = parseQuery(input);

    // Apliquem el filtre
    DocumentNode *results = filterDocuments(docs, query);

    // Imprimim els resultats (MODIFICAT)
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
    // TODO: alliberar memòria de Query i resultats (MODIFICAT)
    free_query(query);
    free_documents_list(docs);


    // Alliberem tots els documents (MODIFICAT)
    // free_documents_list(docs);

    // uncomment and run "make v" to see how valgrind detects memory leaks
    // createaleak();

    return 0;
  }
