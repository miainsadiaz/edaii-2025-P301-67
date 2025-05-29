#include "document.h"
#include <assert.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// document.h
#ifndef DOCUMENT_H
#define DOCUMENT_H

#endif // DOCUMENT_H

// FUNCIONS DE LINK !!!

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

// afegir un link només si no existeix ja a la llista
void afegir_link_si_no_existeix(Link **head, int id) {
  Link *temp = *head;
  while (temp != NULL) {
    if (temp->id == id) {
      return; // ja existeix, no fem res
    }
    temp = temp->next;
  }
  afegir_link(head, id); // no s'ha trobat, l'afegim al final
}

// FUNCIONS DE DOCUMENTS !!!

// Funció que deserialitza un fitxer de text i retorna un punter a un Document
Document *document_desserialize(char *path) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    printf("Error al obrir el fitxer");
    return NULL;
  }; // Comprovem que el fitxer s'ha obert correctament

  Document *document =
      (Document *)malloc(sizeof(Document)); // Reservem memòria pel document

  char buffer[262144]; // Buffer per emmagatzemar el contingut del fitxer
  int bufferSize = 262144;
  int bufferIdx = 0;
  char ch;

  // Llegim l’ID del document (fins a salt de línia)
  while ((ch = fgetc(f)) != '\n') {
    assert(bufferIdx < bufferSize); // comprova si x no sobrepassa límit (assert
                                    // diu si es true o false)
    buffer[bufferIdx++] = ch;
  }
  assert(bufferIdx < bufferSize);
  buffer[bufferIdx++] = '\0'; // Tanquem la cadena
  document->id =
      atoi(buffer); // Convertim a enter i l’assignem (atoi converteix una
                    // cadena de caràcters (char *) a un enter (int).)

  // Llegim el títol del document
  bufferIdx = 0;
  while ((ch = fgetc(f)) != '\n') {
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = ch;
  }
  assert(bufferIdx < bufferSize);
  buffer[bufferIdx++] = '\0';       // Tanquem la cadena
  document->title = strdup(buffer); // Assignem el títol (còpia de la cadena)

  // Llegim el cos del document i extraïm enllaços (links)
  char linkBuffer[64];
  int linkBufferSize = 64;
  int linkBufferIdx = 0;
  bool parsingLink = false;  // Indica si estem dins d’un enllaç
  Link *links = LinksInit(); // Inicialitzem la llista d’enllaços

  bufferIdx = 0;
  while ((ch = fgetc(f)) != EOF) {
    assert(bufferIdx < bufferSize);
    buffer[bufferIdx++] = ch;

    if (parsingLink) {
      if (ch == ')') { // Fi de l’enllaç
        parsingLink = false;
        assert(linkBufferIdx < linkBufferSize);
        linkBuffer[linkBufferIdx++] = '\0';
        int linkId = atoi(linkBuffer); // Convertim l’ID de l’enllaç

        // Afegim l’enllaç a la llista si no existeix
        afegir_link_si_no_existeix(&links, linkId);

        linkBufferIdx = 0;    // Reiniciem el buffer
      } else if (ch != '(') { // Ometem la primera parèntesi
        assert(linkBufferIdx < linkBufferSize);
        linkBuffer[linkBufferIdx++] = ch;
      }
    } else if (ch == ']') { // Detectem l’inici d’un enllaç [text](ID)
      parsingLink = true;
    }
  }

  assert(bufferIdx < bufferSize);
  buffer[bufferIdx++] = '\0'; // Tanquem la cadena del body

  // Copiem el cos del document
  char *body = (char *)malloc(sizeof(char) * bufferIdx);
  strcpy(body, buffer);

  // Assignem el body i els enllaços al document
  document->body = body;
  document->links = links;

  fclose(f);       // Tanquem el fitxer
  return document; // Retornem el document
}

// Funció que carrega tots els documents d’un directori i els retorna com a
// llista
DocumentNode *loadAllDocuments(const char *path) {
  DIR *dir = opendir(path); // DIR --> directori
  struct dirent *entry;
  if (dir == NULL) {
    printf("Error al obrir el directori");
    return NULL;
  }

  DocumentNode *head = NULL;
  DocumentNode *current = NULL;

  // Recorrem tots els fitxers del directori
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue; // Ometem els directoris especials
    }

    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);

    Document *doc =
        document_desserialize(file_path); // Deserialitzem el document
    if (doc == NULL) {
      fprintf(stderr, "Error al deserialitzar el fitxer: %s\n", file_path);
      continue;
    }

    DocumentNode *new_node = (DocumentNode *)malloc(
        sizeof(DocumentNode)); // Node nou per a la llista
    if (new_node == NULL) {
      perror("Error al reservar memòria pel node");
      closedir(dir);
      return NULL;
    }
    new_node->doc = doc;
    new_node->next = NULL;

    if (head == NULL) {
      head = new_node; // Primer node
    } else {
      current->next = new_node; // Afegeix al final
    }
    current = new_node;
  }

  closedir(dir); // Tanquem el directori
  return head;   // Retornem la capçalera de la llista
}

// Funció per imprimir un document
void print_document(Document *doc) {
  if (doc != NULL) {
    printf("\n");
    printf("\nID:\n%d\n", doc->id);
    printf("\n");
    printf("TÍTOL:\n%s\n", doc->title);
    printf("\n");
    printf("COS:\n%s\n", doc->body);
    printf("\n");
    // Imprimim els enllaços
    Link *current = doc->links;
    printf("ENLLAÇOS: \n");
    while (current != NULL) {
      printf("%d, ", current->id);
      current = current->next;
    }
    printf("\n");
    printf("-------------------------------------------------------------------"
           "-------------------------------------------------------------------"
           "---------------------------------\n");
  } else {
    printf("El document és NULL\n");
  }
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
    while (getchar() != '\n')
      ;
    return;
  }

  // netejar \n restant del buffer
  while (getchar() != '\n')
    ;

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

// allibera document
void free_document(Document *doc) {
  if (doc == NULL)
    return;
  free(doc->title);
  free(doc->body);
  free_links(doc->links);
  free(doc);
}

// arribera llista de documents
void free_documents_list(DocumentNode *docs) {
  DocumentNode *current = docs;
  while (current != NULL) {
    DocumentNode *next = current->next;
    free_document(current->doc);
    free(current);
    current = next;
  }
}

// Divideix una cadena per separadors
void split_string(const char *str, char delimiter) {
  int start = 0;
  int end = 0;
  int length = strlen(str);

  while (end <= length) {

    if (str[end] == delimiter || str[end] == '\0') {
      if (end > start) {
        char token[end - start + 1];
        strncpy(token, &str[start],
                end - start); // strncpy:copiar la subcadena sin el delimitador
                              // y aseguramos que el token esté correctamente
                              // terminado con un carácter nulo ('\0').

        token[end - start] = '\0';
        printf("Token: %s\n", token);
      }
      start = end + 1;
    }
    end++; // anar al seguent caracter
  }
}
