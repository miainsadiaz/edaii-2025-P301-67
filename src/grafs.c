#include <stdio.h>
#include "grafs.h"

//INICIALITZACIÓ GRAFS
void init_graph(DocumentGraph *graph, DocumentNode *docs) {
    graph->documents = docs;
}


//CONTA ELS NODES Q APUNTEN A CADA NODE PER SABER RELLEVANCIA
int get_indegree(DocumentGraph *graph, int target_id) {
    int count = 0;
    DocumentNode *current = graph->documents;
    while (current) {
        Link *link = current->doc->links;
        while (link) {
            if (link->id == target_id) {
                count++;
            }
            link = link->next;
        }
        current = current->next;
    }
    return count;
}

//CALCUL RELLEVANCIA 
void calcular_relevancia_indegree(DocumentGraph *graph) {
    DocumentNode *current = graph->documents;
    while (current) {
        current->doc->relevance = (float)get_indegree(graph, current->doc->id);
        current = current->next;
    }
}

//PRINTEJA RELLEVANCIA 
void print_relevancies(DocumentGraph *graph) {
    DocumentNode *current = graph->documents;
    while (current) {
        printf("Doc %s (id=%d) -> Relevance: %.2f\n",
               current->doc->title, current->doc->id, current->doc->relevance);
        current = current->next;
    }
}

// Ordenació bàsica per rellevància BUBBLE SORT 
void ordenar_per_relevancia(DocumentNode **docs) {
    if (*docs == NULL) return;

    int swapped = 1;

    while (swapped) {
        swapped = 0;
        DocumentNode *current = *docs;

        while (current->next != NULL) {
            if (current->doc->relevance < current->next->doc->relevance) {
                // Intercanvi simple de documents
                Document *temp = current->doc;
                current->doc = current->next->doc;
                current->next->doc = temp;
                swapped = 1;
            }
            current = current->next;
        }
    }
}

