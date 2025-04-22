#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "document.h"

// Funció per extreure enllaços del cos del document
Link* parse_links(const char* body) {
    Link* head = NULL;
    Link* tail = NULL;

    const char* start = body;
    while ((start = strstr(start, "[")) != NULL) { // Busquem el primer '['
        const char* end = strstr(start, "]");  // Busquem el primer ']'
        if (end == NULL) break; // Si no hi ha ']', sortim

        const char* link_start = end + 1; // El primer caràcter després de ']'
        if (*link_start != '(') break; // Esperem que després de ']' vingui '('

        const char* link_end = strstr(link_start, ")"); // Busquem el tancament ')'
        if (link_end == NULL) break;

        // Extracte l'ID de l'enllaç
        char link_id_str[16];
        int len = link_end - link_start - 1; // La longitud de l'ID dins del parèntesi
        strncpy(link_id_str, link_start + 1, len);
        link_id_str[len] = '\0';

        int link_id = atoi(link_id_str); // Converteix a enter

        // Crear un nou node Link
        Link* new_link = crear_link(link_id);

        // Afegir a la llista d'enllaços
        if (head == NULL) {
            head = new_link;
            tail = new_link;
        } else {
            tail->next = new_link;
            tail = new_link;
        }

        start = link_end + 1; // Continuem després de la ')'
    }

    return head;
}

/*LAB1: Aquesta funció es fa càrrec de llegir el fitxer i
 carregar tota la informació (títol, ID, cos del document) 
 dins d'una estructura Document.*/


// Funció per deserialitzar un document des d'un fitxer
Document* document_desserialize(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error obrint fitxer: %s\n", filepath);
        return NULL;
    }

    // Reservem memòria per al Document
    Document* doc = malloc(sizeof(Document));
    if (doc == NULL) {
        fclose(file);
        return NULL;
    }

    char buffer[1024]; // buffer per llegir línies del fitxer

    // Llegim el títol (1a línia)
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0;  // eliminem \n si n'hi ha
        doc->title = strdup(buffer);
    } else {
        doc->title = strdup("Sense títol");
    }

    // Llegim l'ID (2a línia)
    if (fgets(buffer, sizeof(buffer), file) != NULL) {
        doc->id = atoi(buffer);
    } else {
        doc->id = -1; // Valor per defecte si no hi ha ID
    }

    // Llegim el body (la resta del fitxer)
    char* body = malloc(10000); // mida arbitrària gran
    if (body == NULL) {
        fclose(file);
        free(doc);
        return NULL;
    }
    body[0] = '\0'; // inicialitzar cadena buida

    while (fgets(buffer, sizeof(buffer), file)) {
        strcat(body, buffer);
    }

    doc->body = strdup(body);
    free(body);

    // Processar els enllaços
    doc->links = parse_links(doc->body);

    // Inicialitzem altres camps
    doc->relevance = 0.0;
    doc->next = NULL;

    fclose(file);
    return doc;
}

