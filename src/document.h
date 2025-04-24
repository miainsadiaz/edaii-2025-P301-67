typedef struct Link{
    int id;
    struct Link* next;
    struct Link* prev;
}Link;

typedef struct Document{
    char* title;
    int id;
    Link* links;
    float relevance;
    char* body;
}Document;

typedef struct DocumentNode {
    Document *doc;
    struct DocumentNode *next;
} DocumentNode;



DocumentNode *loadAllDocuments(const char* path);
Document *document_desserialize(char *path);
Link *LinksInit();
Link *crear_link(int id);
void print_link_list(Link *head);
void afegir_link(Link **head, int id);
void afegir_link_al_principi(Link **head, int id);
void afegir_link_pos(Link *prev_link, int id);
void eliminar_ultim_link(Link** head);
void free_links(Link *head);
void eliminar_primer_link(Link** head);
void eliminar_link_pos(Link** head, int pos);
void print_document(Document *doc);
void free_document(Document* doc);
void free_documents_list(DocumentNode *docs);

