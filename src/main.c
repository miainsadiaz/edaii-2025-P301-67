#include <stdio.h>
#include "document.h"
#include "link.h"
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>  // Incloure per la funció strstr()

// Funció per carregar tots els documents d'un directori
void load_documents_from_folder(const char* folder_path) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;  // Observar el tipus de fitxer

    if ((dir = opendir(folder_path)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            // Construir la ruta completa del fitxer
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", folder_path, entry->d_name);

            // Comprovar el tipus del fitxer utilitzant stat()
            if (stat(filepath, &statbuf) == 0) {
                // Si és un fitxer regular i té l'extensió .txt
                if (S_ISREG(statbuf.st_mode) && strstr(entry->d_name, ".txt") != NULL) {
                    // Cridar document_desserialize per a cada fitxer
                    Document* doc = document_desserialize(filepath);
                    if (doc != NULL) {
                        printf("Loaded document: %s\n", doc->title);
                        printf("ID: %d\n", doc->id);
                        printf("Body: %s\n", doc->body);

                        // Mostrar els enllaços
                        Link* link = doc->links;
                        while (link != NULL) {
                            printf("Link ID: %d\n", link->id);
                            link = link->next;
                        }

                        // Alliberar memòria
                        free(doc->title);
                        free(doc->body);
                        free(doc);
                    }
                }
            } else {
                printf("Error obtenint informació del fitxer: %s\n", filepath);
            }
        }
        closedir(dir);  // Tancar el directori
    } else {
        printf("No es pot obrir el directori: %s\n", folder_path);
    }
}

int main() {
    printf("*****************\nWelcome to EDA 2!\n*****************\n");

    // Carregar tots els documents de la carpeta
    load_documents_from_folder("./datasets/wikipedia12");

    return 0;
}

