#include "sample_lib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "document.h"
<<<<<<< HEAD
#include "link.h"
=======


>>>>>>> c8bff65d00de1fb22abbe31610b472e9597a3b13

void createaleak() {
  char *foo = malloc(20 * sizeof(char));
  printf("Allocated leaking string: %s", foo);
}

int main() {
<<<<<<< HEAD
    printf("*****************\nWelcome to EDA 2!\n*****************\n");
  
  Document *doc = document_desserialize("./datasets/wikipedia12/2.txt");
  if (doc != NULL) {
        print_document(doc);

        // Liberar memoria después de imprimir
        free(doc->title);
        free(doc->body);
        free_links(doc->links);
        free(doc);
    }else{
      printf("No s'ha pogut carregar\n");
  }

    return 0;

  // how to import and call a function
  //printf("Factorial of 4 is %d\n", fact(4));

  // uncomment and run "make v" to see how valgrind detects memory leaks
  createaleak();

    return 0;
=======
  printf("*****************\nWelcome to EDA 2!\n*****************\n");

  // how to import and call a function
  printf("Factorial of 4 is %d\n", fact(4));

  
  Document* doc = document_desserialize("./datasets/wikipedia12/8.txt");

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
>>>>>>> c8bff65d00de1fb22abbe31610b472e9597a3b13
}