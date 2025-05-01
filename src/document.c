#include "document.h"
#include <assert.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// crear node llista
Link *crear_link(int id) {
  Link *new_link = malloc(sizeof(Link)); // Allocate memòria per al node
  if (new_link == NULL) {                // Control d'errors
    return NULL;
  }
  new_link->id = id;     // Assignar id al node
  new_link->next = NULL; // Inicialitzar el següent node a NULL
  new_link->prev = NULL; // Inicialitzar el node anterior a NULL
  return new_link;       // Retornar el node creat
}
// links init (inicialitzar llista buida)
Link *LinksInit() { return NULL; }

// afegir node al final de la llista
void afegir_link(Link **head, int id) {
  Link *new_link = crear_link(id); // Creem un nou node
  Link *temp = *head;              // punter per recorre la llista

  if (*head == NULL) {
    new_link->prev = NULL; // Si la llista és buida, el nou node és el primer
    *head = new_link;      // i últim node
    return;
  }
  while (temp->next !=
         NULL) { // si la llista no està buida, es buca l'últim node
    temp = temp->next;
  }

  temp->next = new_link; // Assignem el nou node com a següent de l'actual
  new_link->prev = temp; // Assignem l'actual com a node anterior del nou
}

// afegir node al principi de la llista
void afegir_link_al_principi(Link **head, int id) {
  Link *new_link = crear_link(id); // Creem un nou node
  new_link->next = *head;          // El nou apunta al que era el primer
  new_link->prev = NULL;           // No té anterior (és el primer)

  if (*head != NULL) {
    (*head)->prev =
        new_link; // Si hi havia un node, ara el nou serà l'anterior d'aquest
  }

  *head = new_link; // el nou es converteix en el 1r
}

// afegir node a la posicio indicada
void afegir_link_pos(Link *prev_link, int id) {
  if (prev_link == NULL) {
    return;
  }
  Link *new_link = crear_link(id);  // Creem un nou node
  new_link->next = prev_link->next; // El nou apunta al següent de l'anterior
  prev_link->next = new_link;       // L'anterior apunta al nou
  new_link->prev = prev_link;       // El nou apunta a darrere a l'anterior

  if (new_link->next != NULL) { // si el nou node té un següent, aquest ha de
                                // saber que ara l'anteior és el nou
    new_link->next->prev = new_link;
  }
}

// alliberar llista (borra nodes)
void free_links(Link *head) {
  Link *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}

// eliminar el primer node de la llista
void eliminar_primer_link(Link **head) {
  if (*head == NULL) {
    return;
  }

  Link *temp = *head;    // guardem 1r node
  *head = (*head)->next; // 2n es converteix en 1r

  if (*head != NULL) {
    (*head)->prev = NULL; // el nou primer no té anterior
  }

  free(temp); // borrem el node
}

// eliminar l'ultim node de la llista
void eliminar_ultim_link(Link **head) {
  if (*head == NULL) {
    return;
  }

  Link *temp = *head;

  while (temp->next != NULL) { // ens situem a l'últim node
    temp = temp->next;
  }

  if (temp->prev != NULL) {
    temp->prev->next = NULL; // l'anterior ja no té següent
  } else {
    *head =
        NULL; // si no hi havia anterior, signnifica que només hi havia un node
  }

  free(temp); // borrem l'últim node
}

// eliminar node de la llista d'una posicio indicada
void eliminar_link_pos(Link **head, int pos) {
  if (*head == NULL) {
    return;
  }

  Link *temp = *head;
  for (int i = 0; i < pos && temp != NULL;
       i++) { // avancem fins a la posició indicada
    temp = temp->next;
  }
  if (temp == NULL) { // si no arriba a la posició indicada, no fem res
    return;
  }
  if (temp == *head) { // si el que volem eliminar és el 1r
    *head = temp->next;
    if (*head != NULL) {
      (*head)->prev = NULL;
    }
  } else {
    if (temp->prev != NULL) {
      temp->prev->next = temp->next; // saltem el node borrat
    }
    if (temp->next != NULL) {
      temp->next->prev = temp->prev;
    }
  }

  free(temp); // borrem el node
}

// imprimir llista (mostra id de cada node)
void print_link_list(Link *head) {
  Link *current = head;
  while (current != NULL) {
    printf("Link ID: %d\n", current->id); // mostrem id
    current = current->next;              // passem al següent
  }
}

Document *document_desserialize(char *path) {
  FILE *f = fopen(path, "r");
  assert(f != NULL);

  Document *document = (Document *)malloc(sizeof(Document));

  char buffer[262144];
  int bufferSize = 262144;
  int bufferIdx = 0;
  char ch;

  // parse id
  while ((ch = fgetc(f)) != '\n') {
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = ch;
  }
  assert(bufferIdx < bufferSize);
  buffer[bufferIdx++] = '\0';
  document->id = atoi(buffer);

  // parse title
  bufferIdx = 0;
  while ((ch = fgetc(f)) != '\n') {
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = ch;
  }
  assert(bufferIdx < bufferSize);
  buffer[bufferIdx++] = '\0';
  document->title = strdup(buffer);

  // parse body
  char linkBuffer[64];
  int linkBufferSize = 64;
  int linkBufferIdx = 0;
  bool parsingLink = false;
  Link *links = LinksInit();

  bufferIdx = 0;
  while ((ch = fgetc(f)) != EOF) {
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = ch;
    if (parsingLink) {
      if (ch == ')') { // end of link
        parsingLink = false;
        assert(linkBufferIdx < linkBufferSize);
        linkBuffer[linkBufferIdx++] = '\0';
        int linkId = atoi(linkBuffer);

        // TODO add to links
        afegir_link(&links, linkId);

        linkBufferIdx = 0;
      } else if (ch != '(') { // skip first parenthesis of the link
        assert(linkBufferIdx < linkBufferSize);
        linkBuffer[linkBufferIdx++] = ch;
      }
    } else if (ch ==
               ']') { // found beginning of link id, e.g.: [my link text](123)
      parsingLink = true;
    }
  }
  assert(bufferIdx < bufferSize);
  buffer[bufferIdx++] = '\0';

  char *body = (char *)malloc(sizeof(char) * bufferIdx);
  strcpy(body, buffer);

  // TODO
  document->body = body;
  document->links = links;

  fclose(f);
  return document;
}

DocumentNode *loadAllDocuments(const char *path) {
  DIR *dir = opendir(path);
  struct dirent *entry;
  if (dir == NULL) {
    perror("Error al abrir el directorio");
    return NULL;
  }

  DocumentNode *head = NULL;
  DocumentNode *current = NULL;

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

    Document *doc = document_desserialize(file_path);
    if (doc == NULL) {
      fprintf(stderr, "Error al deserializar el archivo: %s\n", file_path);
      continue;
    }

    DocumentNode *new_node = (DocumentNode *)malloc(sizeof(DocumentNode));
    if (new_node == NULL) {
      perror("Error al reservar memòria per al node");
      closedir(dir);
      return NULL;
    }
    new_node->doc = doc;
    new_node->next = NULL;

    if (head == NULL) {
      head = new_node;
    } else {
      current->next = new_node;
    }
    current = new_node;
  }

  closedir(dir);
  return head;
}

// imprimir documents
void print_document(Document *doc) {
  if (doc != NULL) {
    printf("ID: %d\n", doc->id);
    printf("Title: %s\n", doc->title);
    printf("Body: %s\n", doc->body);
    // imprimir los links
    Link *current = doc->links;
    printf("Links: ");
    while (current != NULL) {
      printf("%d", current->id);
      current = current->next;
    }
    printf("\n");
  } else {
    printf("Docment is NULL \n");
  }
}

void free_document(Document *doc) {
  if (doc == NULL)
    return;
  free(doc->title);
  free(doc->body);
  free_links(doc->links);
  free(doc);
}

void free_documents_list(DocumentNode *docs) {
    DocumentNode *current = docs;
  while (current != NULL) {
      DocumentNode *next = current->next;
      free_document(current->doc);
      free(current);
      current = next;
  }
}
