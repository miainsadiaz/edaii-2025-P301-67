#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "document.h"



void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

int main() {
  printf("*****************\nWelcome to EDA 2!\n*****************\n");

  // how to import and call a function
  printf("Factorial of 4 is %d\n", fact(4));

  //Imprimir un document
  /*
  Document* doc = document_desserialize("./datasets/wikipedia12/0.txt");

  printf("Document ID: %d\n", doc->id);
  printf("Document Title: %s\n", doc->title);
  printf("Document Body: %s\n", doc->body);
  printf("Document Links:\n");
  Link* current_link = doc->links;
  print_link_list(current_link);

  alliberar_links(doc->links); 
  free(doc->title);
  free(doc->body);
  free(doc);*/

  
  const char* path = "./datasets/wikipedia12";
  DocumentNode* docs = loadAllDocuments(path);

  DocumentNode* current_doc = docs;
  while (current_doc != NULL) {
      print_document(current_doc->doc);
      printf("Document Links:\n");
      Link* current_link = current_doc->doc->links;
      print_link_list(current_link);
      current_doc = current_doc->next;
  }
  
    while (current_doc != NULL) {
        DocumentNode *temp = current_doc;
        free_links(current_doc->doc->links);
        current_doc = current_doc->next;
        free(temp);
    }



  // uncomment and run "make v" to see how valgrind detects memory leaks
  // createaleak();

  return 0;
}
