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

  
  Document* doc = document_desserialize("./datasets/wikipedia12/2.txt");
  if (doc == NULL) {
        printf("Error: No se pudo deserializar el documento.\n");
        return 1;
  }
  printf("Document ID: %d\n", doc->id);
  printf("Document Title: %s\n", doc->title);
  printf("Document Body: %s\n", doc->body);
  printf("Document Links:\n");
  Link* current_link = doc->links;
  print_link_list(current_link);

  alliberar_links(doc->links); 
  free(doc->title);
  free(doc->body);
  free(doc);


  // uncomment and run "make v" to see how valgrind detects memory leaks
  // createaleak();

  return 0;
}
