
#include "../src/hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
  HashMap map;
  init_hashmap(&map);

  // Crea documentos de prueba
  Document doc1 = {.id = 1, .body = "Hola mundo"};
  Document doc2 = {.id = 2, .body = "Adios mundo"};

  // Test 1: Insertar y obtener
  put(&map, "hola", &doc1);
  Document *result = get(&map, "hola");
  assert(result != NULL && result->id == 1);

  // Test 2: Insertar dos claves y eliminar una
  put(&map, "adios", &doc2);
  remove_key(&map, "hola");
  assert(get(&map, "hola") == NULL);
  assert(get(&map, "adios") != NULL && get(&map, "adios")->id == 2);

  // Test 3: Buscar clave inexistente
  assert(get(&map, "noexiste") == NULL);

  printf("Todos los tests de hashmap pasaron correctamente.\n");
  free_hashmap(&map);
  return 0;
}
