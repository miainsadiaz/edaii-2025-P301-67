# Report: Building a search engine like Google

## 1. C4 Component Diagram

El sistema es divideix en 4 components principals:

- **Parser de documents**: llegeix fitxers `.txt` i crea structs en memòria (`Document`, `Link`).
- **Índex invers (reverse index)**: un `HashMap` que associa paraules a llistes de documents.
- **Graf de documents**: un graf dirigit on els nodes són documents i les arestes són enllaços.
- **Motor de cerca (Search Engine)**: processa consultes, filtra documents i els ordena per rellevància.

🟦 = en memòria (RAM), 🟩 = a disc


---

## 2. Anàlisi de Complexitat

| Descripció                                                                 | Big-O          | Justificació                                                                 |
|----------------------------------------------------------------------------|----------------|------------------------------------------------------------------------------|
| Parsing d’un document (amb links)                                          | O(n)           | Recorre cada caràcter del fitxer una sola vegada per llegir-lo i analitzar-lo |
| Parsing d’una consulta (query)                                             | O(k)           | Divideix la consulta en k paraules mitjançant `strtok()`                     |
| Comptar nodes adjacents (links) de TOTS els documents del graf                       | O(n·l)         | Cada document pot tenir l enllaços → es fa doble bucle                        |
| Comptar quants apunten a un sol document (`get_indegree`)                 | O(n·l)         | Cal revisar tots els enllaços de tots els documents                          |
| Buscar documents per una paraula (amb índex invers)                        | O(1)           | Accés directe en el `HashMap` per clau                                       |
| Buscar documents que compleixen TOTES les paraules d’una consulta         | O(k·d)         | Per cada paraula (k), es recorre una llista de d documents                   |
| Ordenar els documents per rellevància (`ordenar_per_relevancia`)          | O(n²)          | Es fa Bubble Sort sobre n documents         






3. Parsing a document into the struct:  O(n+m)    ya que parsear un dpcumento es O(n) donde n es el número de caracteres que hay en el documento. m es el numero de links encontrados por cada busqueda. Entonces cada vez que se añade un link a la linked list, la complejidad es de O(1), así que oara añadir todos los links, la complejidad es de O(m).Así es que en total la complejidad es la suma de las dos .        |

4. Parsing a query intro the struct: Como para hacer esto se han de leer todos los carácteres de la búqeda introducida por el usuario y después almacenarlos en la struct, entenderemos n como el número de estos carácteres y tendrá una complejidad lineal: O(n)

5. analysis of counting the neighbours in the graph: Para contar los vecinos de un nodo en un grafo se han de recorrest todos sus vértices adyacentes. Así que siendo n los vecinos del nodo, su complejidad es lineal O(n)
---

## 3. Gràfics i Discussió

### 3.1 Temps de cerca amb / sense "reverse index"

![grafica1.png](grafica1.png)

> Amb índex invers la cerca és gairebé immediata. Sense índex, el temps creix linealment amb el nombre de documents.

---

### 3.2 Temps d’inicialització amb diferents mides de `HashMap`

![grafica2.png](grafica2.png)

> L’inicialització és lleugerament més ràpida amb mides de taula més grans perquè es redueixen col·lisions.

---

### 3.3 Temps de cerca amb diferents mides de `HashMap`

![grafica3.png](grafica3.png)

> La cerca millora lleugerament amb mides més grans del `HashMap` gràcies a una menor profunditat a la llista de col·lisions.

---

## 4. Millora proposada pel "reverse index"

**Proposta:** Separar l’índex en dos:

- *Short index*: només per les paraules dels títols
- *Full index*: per les paraules del cos del document

**Avantatges:**
- Prioritza documents més rellevants (el títol pesa més)
- Millora la qualitat de la cerca

**Impacte:**

| Factor                 | Canvi             |
|------------------------|------------------|
| Memòria                | Lleuger augment     |
| Temps d’inicialització | Igual o lleugerament més alt |
| Complexitat            | O(n·l) (mateixa)  |

---

