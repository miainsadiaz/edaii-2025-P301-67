#include "query.h"
#include "document.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funció que converteix una cadena d'entrada en una struct Query
// La cadena és separada en paraules i es crea una llista vinculada de QueryItem
Query *parseQuery(const char *input) {
  // Reservem memòria per la struct Query principal
  Query *query = malloc(sizeof(Query));
  if (query == NULL) {
    printf("Error al reservar memòria per al node");
    return NULL;
  }

  query->head = NULL;
  //(MODIFICAT)
  // Copiem el text d’entrada perquè strtok el modifica
  char *copy = strdup(input);
  if (copy == NULL) {
    printf("Error al reservar memòria per a la còpia");
    free(query);
    return NULL;
  }

  QueryItem *last = NULL;          // Punter per afegir nous items
  char *token = strtok(copy, " "); // Separem pel primer espai

  while (token != NULL) {
    // Reservem memòria per cada paraula de la query
    QueryItem *item = malloc(sizeof(QueryItem));
    if (item == NULL) {
      printf("Error al reservar memòria per al node");
      free_query(query); // MODIFICAT
      free(copy);
      return NULL;
    }

    // Si comença per '-', és una paraula exclosa
    item->is_exclusion = (token[0] == '-');
    item->word = strdup(
        token + (item->is_exclusion
                     ? 1
                     : 0)); // això fa que si es true es reotrni 1 y sino 0

    // Comprovem si la paraula és exclosa (comença amb '-')
    if (item->is_exclusion) {
      item->word = strdup(token + 1); // Ignorem el primer caràcter ('-')
    } else {
      item->word = strdup(token); // Agafem la paraula tal qual
    }
    if (item->word == NULL) {
      fprintf(stderr,
              "Hi ha hagut un error al reservar memòria per a la paraula");
      free(item);
      free_query(query);
      free(copy);
      return NULL;
    }

    item->next = NULL;

    // Afegim l’item a la llista
    if (last == NULL) {
      query->head = item; // Primer element
    } else {
      last->next = item; // Afegim al final
    }

    last = item; // Actualitzem l'últim

    token = strtok(NULL, " "); // Anem al següent mot
  }

  free(copy); // Alliberem la còpia temporal
  return query;
}

// Funció auxiliar per veure si un text conté una paraula
// Retorna 'true' si el text conté la paraula, 'false' en cas contrari
bool contains_word(const char *text, const char *word) {
  return strstr(text, word) != NULL; // Busca substring
}

// Filtra una llista de documents segons la query
// Retorna una llista de documents que compleixen amb totes les condicions de la
// query
DocumentNode *filterDocuments(DocumentNode *docs, Query *query) {
  DocumentNode *result = NULL; // Capçalera de resultats
  DocumentNode *last = NULL;   // Punter al final de resultats
  int count = 0;               // Comptador per limitar a 5

  for (DocumentNode *cur = docs; cur != NULL && count < 5; cur = cur->next) {
    bool valid = true; // Suposem que el document és vàlid

    // Recorrem tots els mots de la query
    for (QueryItem *item = query->head; item != NULL; item = item->next) {
      bool found = contains_word(cur->doc->body, item->word);

      // Si és exclusió i la trobem → descartem
      if ((item->is_exclusion && found) || (!item->is_exclusion && !found)) {
        valid = false;
        break;
      }
    }

    // Si el document és vàlid, l’afegim a la nova llista
    if (valid) {
      DocumentNode *copy = malloc(sizeof(DocumentNode));
      copy->doc = cur->doc;
      copy->next = NULL;

      if (result == NULL)
        result = copy;
      else
        last->next = copy;

      last = copy;
      count++;
    }
  }

  return result; // Retornem la llista filtrada
}

// Allibera la memòria de la query, incloent els elements de la llista vinculada
void free_query(Query *query) {
  QueryItem *current = query->head;
  while (current != NULL) {
    QueryItem *next = current->next;
    free(current->word); // Alliberem la memòria de la paraula
    free(current);       // Alliberem la memòria del node
    current = next;
  }
  free(query); // Alliberem la memòria de la struct Query
}

// Inicialitza una nova cua de consultes (QueryQueue) amb 3 llocs disponibles
QueryQueue *init_query_queue() {
  QueryQueue *queue = malloc(sizeof(QueryQueue));

  if (queue == NULL) {
    fprintf(stderr, "Hi ha hagut un error al reservar memòria per a la cua");
    return NULL;
  }

  queue->front = 0;
  queue->rear = 0;
  return queue;
}

// Afegeix una nova consulta a la cua. Si la cua està plena, elimina la consulta
// més antiga
void enqueue_query(QueryQueue *queue, Query *query) {
  if ((queue->rear + 1) % 3 == queue->front) {
    // Si la cua està plena, eliminem la consulta més antiga
    free_query(
        queue->queries[queue->front]); // Alliberar la memòria de la consulta
    queue->front = (queue->front + 1) % 3;
  }
  queue->queries[queue->rear] = query;
  queue->rear = (queue->rear + 1) % 3;
}

// Imprimeix les últimes consultes de la cua (fins a un màxim de 3)
void print_last_queries(QueryQueue *queue) {
  int i = queue->front;
  int count = 0;
  while (count < 3 && i != queue->rear) {
    printf("Query: ");
    QueryItem *current_item = queue->queries[i]->head;
    while (current_item != NULL) {
      printf("%s ", current_item->word);
      current_item = current_item->next;
    }
    printf("\n");
    i = (i + 1) % 3;
    count++;
  }
}
