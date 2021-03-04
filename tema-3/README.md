[include]: include/
[src]: src/

[Data-h]: include/Data.h
[Data-c]: src/Data.c

[Graph-h]: include/Graph.h
[Graph-c]: src/Graph.c

[Heap-h]: include/Heap.h
[Heap-c]: src/Heap.c

[BSTree-h]: include/BSTree.h
[BSTree-c]: src/BSTree.c

[List-h]: include/List.h
[List-c]: src/List.c

[pb-1-main-c]: src/pb-1/main.c

[pb-1-task1-c]: src/pb-1/task1.c
[bfs]: https://en.wikipedia.org/wiki/Breadth-first_search

[pb-1-task2-c]: src/pb-1/task2.c

[pb-1-task3-c]: src/pb-1/task3.c
[tarjan]: https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm

[pb-1-bonus-c]: src/pb-1/bonus.c
[bron-kerbosch]: https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm

[pb-2-main-c]: src/pb-2/main.c

[pb-2-path-finding-c]: src/pb-2/path-finding.c
[dijkstra]: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

# Algoritmi de prelucrare a grafurilor
Acest subdirector conține **Tema 3** din cadrul cursului _Structuri de date_ ce presupune lucrul cu grafuri.
Programul este împărțit în mai multe componente/fișiere, fiecare din acestea rezolvând o anumită problemă.

## Structura codului sursă
În fișierele antet, aflate în [directorul include][include], se află declarațiile/definițiile de constante, structuri, funcții ce sunt folosite în cadrul fișierelor sursă. Unde este cazul, există și comentarii ce însoțesc declarațiile/definițiile menite să ușureze înțelegerea scopului acestora.

În fișierele sursă, aflate în [directorul src][src], se află implementările, fiind comentate părțile mai importante din logica programului.

## Încapsularea datelor ([Data.h][Data-h], [Data.c][Data-c])
Pentru a putea obține o implementare generică a tipurilor de date abstracte (în cazul de față liste înlănțuite, arbori, movile și grafuri), care poate stoca orice fel de date, se folosește o încapsulare într-o structură ce va conține atât un _pointer_ la zona de memorie unde se află de fapt datele propriu-zise, cât și dimensiunea datelor. Pentru accesarea datelor la nevoie se folosesc _cast_-uri de la `void *`.

## Implementarea grafului ([Graph.h][Graph-h], [Graph.c][Graph-c])
S-a încercat o implementare a grafului într-un mod cât mai generic cu putință, astfel încât să fie suportat atât lucrul cu grafuri orientate, cât și lucrul cu grafuri neorientate. Structura de graf urmează paradigma _Object Oriented_ utilizată și în **Temele 1** și **2**.

### Notă
Algoritmii ce operează pe grafuri în cadrul temei folosesc câmpul `metadata` al nodurilor grafurilor pentru a-și stoca informațiile de care au nevoie în cadrul rulării acestora. Câmpulul metadata este implementat ca listă înlănțuită pentru a permit stocarea unui număr variabil de date.

## Implementarea movilei ([Heap.h][Heap-h], [Heap.c][Heap-c])
La fel ca în cazul grafului, este vorba de o implementare cât mai generică cu putință.

## Implementarea arborelui binar de căutare ([BSTree.h][BSTree-h], [BSTree.c][BSTree-c])
Ca și în cazul **Temei 2**, s-a încercat implementarea arborelui  binar de căutare într-un mod _Object Oriented_. Pentru reținerea copiilor unui nod din arbore se folosește o listă înlănțuită.

## Implementarea listei înlănțuite ([List.h][List-h], [List.c][List-c])
Implementarea de listă înlănțuită este cea folosită și în cadrul **Temelor 1** și **2**.

## Problema 1 ([main.c][pb-1-main-c])
Reprezintă partea ce se ocupă de apelarea funcțiilor corespunzătoare efectuării sarcinilor de lucru.

### Rezolvarea sarcinilor de lucru
1. determinarea numărului de producții independente existente în colecția de filme ([task1.c][pb-1-task1-c])
   * s-a folosit parcurgerea [BFS][bfs] pentru determinarea numărului de componente conexe
2. determinarea gradului de înrudire dintre doi actori ([task2.c][pb-1-task2-c])
   * s-a folosit, de asemenea, o parcurgere [BFS][bfs] până la găsirea unui drum de la unul dintre actori la celălalt
3. determinarea punctelor de articulație în graf ([task3.c][pb-1-task3-c])
   * s-a folosit [algoritmul lui Tarjan][tarjan] pentru determinarea nodurilor critice
4. determinarea clicii maximale ([bonus.c][pb-1-bonus-c])
   * s-a folosit [algoritmul Bron Kerbosch][bron-kerbosch] cu reprezentarea mulțimilor ca arbori binari de căutare pentru determinarea clicilor
   * pentru a extrage clica maximă, toate clicle obținute din algoritm se introduc într-un _maxheap_ și la final se extrage prima clică din _heap_

## Problema 2 ([main.c][pb-2-main-c])
Reprezintă partea ce se ocupă de apelarea funcțiilor corespunzătoare efectuării sarcinilor de lucru.

### Rezolvarea sarcinilor de lucru
1. determinarea drumului minim în graf ([path-finding.c][pb-2-path-finding-c])
   * s-a folosit [algoritmul lui Dijkstra][dijkstra] la care se adaugă în plus timpul de așteptare într-o cameră dacă este cazul, prin adăugarea timpului așteptat la distanța până la un anumit nod
