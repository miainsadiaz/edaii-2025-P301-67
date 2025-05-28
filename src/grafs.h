#ifndef GRAFS_H
#define GRAFS_H

#include "document.h"

// Estructura que representa tot el graf
typedef struct {
    DocumentNode *documents;  // llista de documents
} DocumentGraph;

// Funcions del graf
void init_graph(DocumentGraph *graph, DocumentNode *docs);
int get_indegree(DocumentGraph *graph, int id_obj);
void calcular_relevancia_indegree(DocumentGraph *graph);
void print_relevancies(DocumentGraph *graph);
void ordenar_per_relevancia(DocumentNode **docs);

#endif // GRAFS_H
