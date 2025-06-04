# Report: Building a search engine like Google

Paola Gonzalez, Mia Insa i Anna Vilacís

## 1. C4 Component Diagram

El sistema es divideix en 4 components principals:

- **Parser de documents**: llegeix fitxers `.txt` i crea structs en memòria (`Document`, `Link`). 🟩 🟦 
- **Índex invers (reverse index)**: un `HashMap` que associa paraules a llistes de documents.🟦
- **Graf de documents**: un graf dirigit on els nodes són documents i les arestes són enllaços.🟦
- **Motor de cerca (Search Engine)**: processa consultes, filtra documents i els ordena per rellevància.🟦

🟦 = en memòria (RAM), 🟩 = a disc


## 2. Anàlisi de Complexitat

| Descripció                                                                 | Big-O          | Justificació                                                                 |
|----------------------------------------------------------------------------|----------------|------------------------------------------------------------------------------|
| Parsing d’un document (amb links)                                          | O(n)           | Recorre cada caràcter del fitxer una sola vegada per llegir-lo i analitzar-lo. |
| Parsing d’una consulta (query)                                             | O(k)           | Divideix la consulta en k paraules mitjançant `strtok()`.                   |
| Comptar nodes adjacents/veïns (links) de TOTS els documents del graf       | O(n·l)         | Cada document pot tenir l enllaços; es fa un doble bucle per comptar-los.   |
| Comptar quants apunten a un sol document (`get_indegree`)                  | O(n·l)         | Cal revisar tots els enllaços de tots els documents.                        |
| Buscar documents per una paraula (amb índex invers)                        | O(1)           | Accés directe en el `HashMap` per clau.                                     |
| Buscar documents que compleixen TOTES les paraules d’una consulta          | O(k·d)         | Per cada paraula (k), es recorre una llista de d documents.                 |
| Ordenar els documents per rellevància (`ordenar_per_relevancia`)          | O(n²)          | Es fa Bubble Sort sobre n documents.                                        |



## 3. Gràfics i Discussió

### 3.1 Temps de cerca amb / sense "reverse index"

![Temps de cerca amb/sense índex invers](image-4.png)

> Amb índex invers la cerca és gairebé immediata. Sense índex invers, el temps creix més o meny de manera lineal, menys en un punt, ja que en el dataset de 540 s'han trobat més cerques de la paraula introduïda que en el de 270. 


### 3.2 Temps d’inicialització amb diferents mides de `HashMap`

![Temps d’inicialització amb diferents mides de HashMap](image-5.png)
> L’inicialització és lleugerament més ràpida amb mides de taula més grans perquè es redueixen col·lisions.


### 3.3 Temps de cerca amb diferents mides de `HashMap`

![Temps de cerca amb diferents mides de HashMap](image-6.png)

> La cerca millora lleugerament amb mides més grans del `HashMap` gràcies a una menor profunditat a la llista de col·lisions.


## 4. Millora proposada per l’índex invers

**Proposta:**  
Dividir l’índex invers en dos subíndexs diferenciats:  
- **Short Index**: conté només les paraules del títol de cada document.  
- **Full Index**: conté totes les paraules del document (incloent-hi el títol i el cos).  

Aquesta divisió permet:  
Prioritzar els documents més rellevants (els que tenen la paraula clau al títol).  
Reduir el temps de cerca, ja que es pot consultar primer el *Short Index* i després completar amb el *Full Index*.  
Millorar la qualitat del rànquing de resultats, donant més pes als títols.


**Impacte esperat:**  

| Factor                     | Canvi esperat                                                        |
|----------------------------|----------------------------------------------------------------------|
| **Ús de memòria**          | Lleuger augment: hi ha dues estructures en lloc d’una sola.          |
| **Temps d’inicialització** | Pot ser una mica més alt, ja que es construeixen dos índexs.         |
| **Complexitat**            | Manté O(n·l) (on n = documents, l = paraules per document). La càrrega afegida és constant i insignificant. |

