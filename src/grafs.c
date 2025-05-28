#include <stdio.h>
#include "grafs.h"

//INICIALITZACIÓ GRAFS
/*Inicialitza el graf de documents. 
Simplement assigna la llista de documents (docs) al camp documents de l’estructura DocumentGraph.*/
void init_graph(DocumentGraph *graph, DocumentNode *docs) {
    graph->documents = docs;
}


//CONTA ELS NODES Q APUNTEN A CADA NODE PER SABER RELLEVANCIA
/*Calcula el grau d’entrada (indegree) d’un document amb identificador id_obj. 
És a dir, quants altres documents l’enllacen.*/
int get_indegree(DocumentGraph *graph, int id_obj) {
    int count = 0;
    DocumentNode *current = graph->documents;
    while (current) {
        Link *link = current->doc->links;
        while (link) {
            if (link->id == id_obj) {
                count++;
            }
            link = link->next;
        }
        current = current->next;
    }
    return count;
}

//CALCUL RELLEVANCIA 
/*Calcula i assigna la rellevància a cada document segons el seu indegree.*/
void calcular_relevancia_indegree(DocumentGraph *graph) {
    DocumentNode *current = graph->documents;
    while (current) { //fins q current no es NULL, es a dir recorre tota la llista
        current->doc->relevance = (float)get_indegree(graph, current->doc->id);
        current = current->next;
    }
}

//PRINTEJA RELLEVANCIA 
/*Mostra per pantalla la rellevància de cada document amb el format*/
void print_relevancies(DocumentGraph *graph) {
    DocumentNode *current = graph->documents;
    while (current) {
        printf("Document %s (amb l'id=%d) -> Relevance: %.2f\n",
               current->doc->title, current->doc->id, current->doc->relevance);
        current = current->next;
    }
}

// Ordenació bàsica per rellevància BUBBLE SORT 
/*Ordena la llista de documents de més rellevant a menys rellevant, 
fent servir Bubble Sort.*/
void ordenar_per_relevancia(DocumentNode **docs) {
    if (*docs == NULL) return;
    int canvi = 1;

    while (canvi==1) { //quan no es 1 -> no s'ha fet cap canvi aleshores està ordenat
        canvi = 0;
        DocumentNode *current = *docs;

        while (current->next != NULL) {
            if (current->doc->relevance < current->next->doc->relevance) {
                // Intercanvi simple de documents
                Document *temp = current->doc;
                current->doc = current->next->doc;
                current->next->doc = temp;
                canvi = 1;
            }
            current = current->next;
        }
    }
}

